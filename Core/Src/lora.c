// Lora Driver Spirce File using stm32wlxx_hal_subghz.c
#include <lora.h>

SUBGHZ_HandleTypeDef hsubghz;

bool lora_init()
{
    // Initalize all configured peripherials
    hsubghz.Init.BaudratePrescaler = SUBGHZSPI_BAUDRATEPRESCALER_8; // #define SPI_CR1_BR_1                (0x2UL << SPI_CR1_BR_Pos)                  /*!< 0x00000010 */ from the wle5
    if (HAL_SUBGHZ_Init(&hsubghz) != HAL_OK)
        return false;

    // set to sleep to wakeup cleanly
    uint8_t sleepParam = 0x00; // Sleep with cold start (use 0x01 for warm)
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_SLEEP, &sleepParam, 1) != HAL_OK)
        return false;

    HAL_Delay(1); // give time for sleep to take effect

    // set standby mode
    uint8_t standby = 0x00; // STDBY_RC
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_STANDBY, &standby, 1) != HAL_OK)
        return false;

    // set packet type to LoRa
    uint8_t packetType = 0x01; // 0x01 = LoRa
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_PACKETTYPE, &packetType, 1) != HAL_OK)
        return false;

    // set frequency to  921.2 MHz and set TX Power to 14 dBm since thats what the node code does
    bool setFreq = lora_setFrequency(921.2);
    bool setPower = lora_setTxPower(14);
    if (!setPower || !setFreq) return false;

    // set paConfig
    uint8_t paConfig[4] = { 0x04, 0x00, 0x01, 0x01 };
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_PACONFIG, paConfig, 4) != HAL_OK)
        return false;

    // set modulation parameters (SF7, BW 125 kHz, CR 4/5)
    if (!lora_setSF(9)) return false;

    // set packet parameters (preamble = 8, header = explicit, CRC on, IQ standard)
    uint8_t pktParams[6] = {
        0x00, 0x08, // Preamble length = 8
        0x00,       // Explicit header
        0x10,       // Payload length = 16 bytes
        0x01,       // CRC on
        0x00        // Standard IQ
    };
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_PACKETPARAMS, pktParams, 6) != HAL_OK)
        return false;

    // set buffer base address
    uint8_t bufBase[2] = { 0x00, 0x00 };
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_BUFFERBASEADDRESS, bufBase, 2) != HAL_OK)
        return false;

    // enable relevant IRQs (TX_DONE, RX_DONE, TIMEOUT, CRC_ERR)
    uint8_t irqParams[8] = {
        0x02, 0x0D, // IRQ mask
        0x02, 0x0D, // IRQ enable
        0x01,       // DIO1 = all IRQs
        0x00,       // DIO2
        0x00,       // DIO3
        0x00
    };
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_CFG_DIOIRQ, irqParams, 8) != HAL_OK)
        return false;

    uint8_t syncWord[2] = { 0x34, 0x44 };
    HAL_SUBGHZ_WriteRegisters(&hsubghz, 0x0740, syncWord, 2);

    // radio is now initialized for LoRa operation
    return true;
}


/**
  * @brief SUBGHZ MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hsubghz: SUBGHZ handle pointer
  * @retval None
  */
//void HAL_SUBGHZ_MspInit(SUBGHZ_HandleTypeDef* hsubghz)
//{
//    /* USER CODE BEGIN SUBGHZ_MspInit 0 */
//
//    /* USER CODE END SUBGHZ_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_SUBGHZSPI_CLK_ENABLE();
//    /* USER CODE BEGIN SUBGHZ_MspInit 1 */
//    LL_RCC_HSE_EnableTcxo();
//
//	LL_RCC_HSE_Enable();
//
//	while (LL_RCC_HSE_IsReady() == 0)
//	{}
//
//    /* USER CODE END SUBGHZ_MspInit 1 */
//
//}

/**
  * @brief SUBGHZ MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hsubghz: SUBGHZ handle pointer
  * @retval None
  */
//void HAL_SUBGHZ_MspDeInit(SUBGHZ_HandleTypeDef* hsubghz)
//{
//    /* USER CODE BEGIN SUBGHZ_MspDeInit 0 */
//
//    /* USER CODE END SUBGHZ_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_SUBGHZSPI_CLK_DISABLE();
//    /* USER CODE BEGIN SUBGHZ_MspDeInit 1 */
//
//    /* USER CODE END SUBGHZ_MspDeInit 1 */
//
//}

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

    uint8_t packetType = 0x01; // 0x01 = LoRa
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_PACKETTYPE, &packetType, 1) != HAL_OK) return false;

    // set packet params
    uint8_t packetParams[6] = {
        0x00, 0x08,  // preamble length = 8
        0x00,        // explicit header
        len,      // payload length
        0x01,        // CRC on
        0x00         // standard IQ
    };
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_PACKETPARAMS, packetParams, 6) != HAL_OK) return false;


    //set frquency
    lora_setFrequency(921.2);

    // set pa
    uint8_t paConfig[4] = { 0x04, 0x00, 0x01, 0x01 }; // HP PA on, duty cycle 1
    if ( HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_PACONFIG, paConfig, 4) != HAL_OK) return false;

    // set tx power
    lora_setTxPower(14);

    if (!lora_setSF(9)) return false;

    uint8_t irqParams[8] = {
        0x02, 0x05, // IRQ mask: TX_DONE + TIMEOUT
        0x02, 0x05, // IRQ enable: same
        0x01,       // DIO1 = all
        0x00,       // DIO2 = none
        0x00,       // DIO3 = none
        0x00
    };
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_CFG_DIOIRQ, irqParams, 8) != HAL_OK) return false;


    // trigger transmission
    uint8_t tx_timeout[3] = { 0x00, 0x01, 0xF4 }; // 0x01F4 = 500 decimal
    if (HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_TX, tx_timeout, sizeof(tx_timeout)) != HAL_OK)
        return false;

    // wait to tx done
    uint8_t irqStatus[2] = { 0 };
    uint32_t start = HAL_GetTick();
    while ((HAL_GetTick() - start) < 3000) // 3 sec timeout
    {
        if (HAL_SUBGHZ_ExecGetCmd(&hsubghz, RADIO_GET_IRQSTATUS, irqStatus, 2) == HAL_OK)
        {
            uint16_t irq = (irqStatus[0] << 8) | irqStatus[1];
            printf("IRQ Status: 0x%02X %02X\r\n", irqStatus[0], irqStatus[1]);
            if (irq & SUBGHZ_IT_TX_CPLT)
                break;
            else if (irq & SUBGHZ_IT_RX_TX_TIMEOUT)
                return false;
        }
    }

    uint8_t clearIrq[] = { 0xFF, 0xFF };
    HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_CLR_IRQSTATUS, clearIrq, 2);

    // Set sync word (at address 0x0740)
    uint8_t syncWord[2] = { 0x34, 0x44 };
    HAL_SUBGHZ_WriteRegisters(&hsubghz, 0x0740, syncWord, 2);

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
    uint8_t fifo_base[2] = { 0x00, 0x00 };
    HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_BUFFERBASEADDRESS, fifo_base, 2);
    status = HAL_SUBGHZ_ReadBuffer(&hsubghz, 0x00, data, packet_len);
    if (status != HAL_OK)
    {
        return false;
    }

    // Set sync word (at address 0x0740)
    uint8_t syncWord[2] = { 0x34, 0x44 };
    HAL_SUBGHZ_WriteRegisters(&hsubghz, 0x0740, syncWord, 2);

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
        	if (irq_status[1] & 0x01) // Bit 0 = TX_DONE
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

bool lora_setFrequency(float freq_mhz)
{
    // Convert MHz to Hz
    float freq_hz = freq_mhz * 1e6;

    // Calculate frequency steps
    uint32_t rfFreq = (uint32_t)(freq_hz / 61.03515625); // 2^25 / 32e6 = 61.03515625

    // Convert rfFreq to 4-byte big endian
    uint8_t rfFreqBytes[4] = {
        (rfFreq >> 24) & 0xFF,
        (rfFreq >> 16) & 0xFF,
        (rfFreq >> 8) & 0xFF,
        rfFreq & 0xFF
    };

    // Send Set_RfFrequency command (0x86)
    return HAL_SUBGHZ_ExecSetCmd(&hsubghz, 0x86, rfFreqBytes, 4) == HAL_OK;
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
    if (HAL_SUBGHZ_ExecGetCmd(&hsubghz, RADIO_GET_IRQSTATUS, irq_status, 2) != HAL_OK) return false;

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
        0x06, // bw 0x06: bandwidth 125 (125 kHz)
        0x01, // cr coding rate 0x0: no forward error correction coding rate 4/4
        0x00, // lr 0 low data rate optimization disabled, unsure if base station can set this
    };

    return HAL_SUBGHZ_ExecSetCmd(&hsubghz, RADIO_SET_MODULATIONPARAMS, modParams, 4) == HAL_OK;
}
