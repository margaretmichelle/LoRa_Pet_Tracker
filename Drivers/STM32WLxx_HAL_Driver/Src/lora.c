// Lora Driver Spirce File using stm32wlxx_hal_subghz.c
#include "lora.h"

SUBGHZ_HandleTypeDef hsubghz;

bool lora_init()
{
    // initialize SubGHz with correct prescaler
    // #define LL_SPI_BAUDRATEPRESCALER_DIV4      (SPI_CR1_BR_0)  /*!< BaudRate control equal to fPCLK/4   */
    hsubghz.Init.BaudratePrescaler = SUBGHZSPI_BAUDRATEPRESCALER_8;
    if (HAL_SUBGHZ_Init(&hsubghz) != HAL_OK)
        return false;

    // put radio in standby (RC oscillator) --> i googles this
    uint8_t standby = 0x00; // STDBY_RC
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_STANDBY, &standby, 1) != HAL_OK)
        return false;

    // set packet type to LoRa
    uint8_t packetType = 0x01; // 0x01 = LoRa
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_PACKETTYPE, &packetType, 1) != HAL_OK)
        return false;

    // set frequency to  434.0 MHz and set TX Power to 13 dBm
    float freq = 434.0;
    bool setFreq = lora_setFrequency(freq);
    bool setPower = lora_setTxPower(13);
    if (!setPower || !setFreq) return false;

    // set modulation parameters (SF7, BW 125 kHz, CR 4/5)
    uint8_t modParams[4] = {
        0x07, // SF7
        0x04, // BW 125 kHz
        0x01, // CR 4/5
        0x00  // Low Data Rate Optimization off
    };
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_MODULATIONPARAMS, modParams, 4) != HAL_OK)
        return false;

    // set packet parameters (preamble = 8, header = explicit, CRC on, IQ standard)
    uint8_t pktParams[9] = {
        0x00, 0x08, // Preamble length = 8
        0x00,       // Explicit header
        0x10,       // Payload length = 16 bytes (adjust as needed)
        0x01,       // CRC on
        0x00        // Standard IQ
    };
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_PACKETPARAMS, pktParams, 7) != HAL_OK)
        return false;

    // radio is now initialized for LoRa operation
    return true;
}


bool lora_send(const uint8_t* data, uint8_t len)
{
    // max message length?

    // set up fifo
    HAL_SUBGHZ_WriteRegister(&hsubghz, SUBGHZ_RADIO_WRITE_BUFFER, 0x00); // FIFO_TX_BASE_ADDR = 0
    HAL_SUBGHZ_WriteRegister(&hsubghz, SUBGHZ_RADIO_WRITE_REGISTER, 0x00); // FIFO_ADDR_PTR = 0
    HAL_SUBGHZ_WriteRegister(&hsubghz, 0x22, len);  // PAYLOAD_LENGTH, what is 0x22 lol

    if (HAL_SUBGHZ_WriteBuffer(&hsubghz, 0x00, (uint8_t*)data, len) != HAL_OK)  // 0x00 is the fifo data access register
        return false;

    // RADIO_SET_TX = 0x83u
    return HAL_SUBGHZ_WriteRegister(&hsubghz, 0x01, RADIO_SET_TX) == HAL_OK; // RH_RF95_MODE_TX | RH_RF95_LONG_RANGE_MODE
}

bool lora_waitPacketSent(uint32_t timeout)
{
    uint32_t start = HAL_GetTick();
    uint8_t irq_flags;

    while ((HAL_GetTick() - start) < timeout)
    {
        HAL_SUBGHZ_ReadRegister(&hsubghz, RADIO_GET_IRQSTATUS, &irq_flags); // RH_RF95_REG_12_IRQ_FLAGS
        if (irq_flags & 0x08) // RH_RF95_TX_DONE
        {
            HAL_SUBGHZ_WriteRegister(&hsubghz, RADIO_GET_IRQSTATUS, 0xFF); // Clear IRQs
            return true;
        }
    }
    return false;
}

bool lora_waitAvailableTimeout(uint32_t timeout)
{
    uint32_t start = HAL_GetTick();
    uint8_t irq_flags;

    while ((HAL_GetTick() - start) < timeout)
    {
        HAL_SUBGHZ_ReadRegister(&hsubghz, RADIO_GET_IRQSTATUS, &irq_flags); // RH_RF95_REG_12_IRQ_FLAGS
        if (irq_flags & 0x40) // RH_RF95_RX_DONE
            return true;
    }
    return false;
}

bool lora_setFrequency(float center)
{
    uint32_t frf = (uint32_t)((center * 1000000.0) / 61.03515625); // RH_RF95_FSTEP
    uint8_t frf_bytes[3] = {
        (uint8_t)(frf >> 16),
        (uint8_t)(frf >> 8),
        (uint8_t)(frf)
    };

    return HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_RFFREQUENCY, frf_bytes, 4) == HAL_OK;
}


bool lora_setTxPower(uint8_t power)
{
    // i made this into boolean just in case
    // TX power: 0–22 dBm (limited by hardware)
    // We dont need to manually set rfo on stm32

    uint8_t rampTime = 0x04; // default ramp time: 20us (safe default)
    
    if (power > 22) // max output power is 22dBm according to datasheet
        power = 22;

    if (power < 0)
        power = 0;

    uint8_t txParams[2] = {
        power, // TX power configuration
        rampTime  // Ramp time code
    };

    return HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_TXPARAMS, txParams, 2) == HAL_OK;
}

bool lora_available()
{
    uint8_t irq_flags;
    HAL_SUBGHZ_ReadRegister(&hsubghz, RADIO_GET_IRQSTATUS, &irq_flags); // RH_RF95_REG_12_IRQ_FLAGS
    return (irq_flags & 0x40); // RH_RF95_RX_DONE
}