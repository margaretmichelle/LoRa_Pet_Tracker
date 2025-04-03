// Lora Driver Spirce File using stm32wlxx_hal_subghz.c
#include <lora.h>

SUBGHZ_HandleTypeDef hsubghz;

bool lora_init()
{
    // initialize SubGHz with correct prescaler
    // #define LL_SPI_BAUDRATEPRESCALER_DIV4      (SPI_CR1_BR_0)  /*!< BaudRate control equal to fPCLK/4   */
    hsubghz.Init.BaudratePrescaler = SUBGHZSPI_BAUDRATEPRESCALER_8; // #define SPI_CR1_BR_1                (0x2UL << SPI_CR1_BR_Pos)                  /*!< 0x00000010 */ from the wle5
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

    // set frequency to  921.2 MHz and set TX Power to 14 dBm since thats what the node code does
    float freq = 921.2;
    bool setFreq = lora_setFrequency(freq);
    bool setPower = lora_setTxPower(14);
    if (!setPower || !setFreq) return false;

    // set modulation parameters (SF7, BW 125 kHz, CR 4/4)
    if (!lora_setSF(0x09)) return false;

    // set packet parameters (preamble = 8, header = explicit, CRC on, IQ standard)
    uint8_t pktParams[9] = {
        0x00, 0x08, // Preamble length = 8
        0x00,       // Explicit header
        0x10,       // Payload length = 16 bytes
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
    // write payload to FIFO (starting at offset 0)
    // only need to write to FIFO via HAL_SUBGHZ_WriteBuffer(), dont need to manually set the FIFO address every time
    uint8_t fifo_base[2] = { 0x00, 0x00 }; // TX base, RX base
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_BUFFERBASEADDRESS, fifo_base, 2) != HAL_OK)
        return false;

    // write payload to FIFO starting at offset 0x00
    if (HAL_SUBGHZ_WriteBuffer(&hsubghz, 0x00, (uint8_t*)data, len) != HAL_OK)
        return false;

    // trigger transmission
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_TX, NULL, 0) != HAL_OK)
        return false;

    return true;
}

bool lora_receive(uint8_t* data, uint8_t len)
{

    if (!lora_available()) return false;

    HAL_StatusTypeDef status;
    uint8_t packet_len = 0;

    // read the RX buffer status (get payload length)
    // No more manually skipping the 4 headers that are at the beginning of the rxBuf
    status = HAL_SUBGHZ_ExecGetCmd(&hsubghz, RADIO_GET_RXBUFFERSTATUS, &packet_len, 1);
    if (status != HAL_OK || packet_len == 0 || packet_len > len)
    {
        return false;
    }

    // read the actual data from the radio RX buffer
    // this does the buffer thingy
    status = HAL_SUBGHZ_ReadBuffer(&hsubghz, 0x00, data, packet_len);
    if (status != HAL_OK)
    {
        return false;
    }

    return true;
}


bool lora_waitPacketSent(uint32_t timeout_ms)
{
    uint8_t irq_status[2];
    uint32_t start = HAL_GetTick();

    while ((HAL_GetTick() - start) < timeout_ms)
    {
        if (HAL_SUBGHZ_ExecGetCmd(&hsubghz, RADIO_GET_IRQSTATUS, irq_status, 2) == HAL_OK)
        {
            if (irq_status[1] & 0x0001) // Bit 0 = TX_DONE
            {
                uint8_t clear_all[] = { 0xFF, 0xFF };
                HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_CLR_IRQSTATUS, clear_all, 2);
                return true;
            }
        }
    }
    return false;
}

bool lora_waitAvailableTimeout(uint32_t timeout)
{
    uint32_t start = HAL_GetTick();
    uint8_t irqStatus[2];

    while ((HAL_GetTick() - start) < timeout)
    {
        HAL_SUBGHZ_ExecGetCmd(&hsubghz, RADIO_GET_IRQSTATUS, irqStatus, 2);
        if (irqStatus[1] & 0x0002) // RX_DONE is bit 2
        {
            return true;
        }
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

    return HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_RFFREQUENCY, frf_bytes, 3) == HAL_OK;
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
    uint8_t irq_status[2];

    // get the current IRQ flags (non-destructive)
    if (HAL_SUBGHZ_ExecGetCmd(&hsubghz, RADIO_CLR_IRQSTATUS, irq_status, 2) != HAL_OK) return false;

    uint16_t irq = ((uint16_t)irq_status[0] << 8) | irq_status[1];

    // RX_DONE is bit 1 = 0x0002
    return (irq & 0x0002);
}


bool lora_setSF(uint8_t sf)
{
    // i made this into boolean just in case
    // we want sf between 7 and 12
    if (sf < 7 || sf > 12) return false;

    // Set_ModulationParams(Sf, Bw, Cr, Ldro) is used to configure the LoRa modulation parameters for the sub-GHz radio.
    uint8_t modParams[4] = {
        sf, // sf
        0x04, // bw 0x04: bandwidth 125 (125 kHz)
        0x0, // cr coding rate 0x0: no forward error correction coding rate 4/4
        0, // lr 0 low data rate optimization disabled, unsure if base station can set this
    };

    return HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_MODULATIONPARAMS, modParams, 4) == HAL_OK;
}
