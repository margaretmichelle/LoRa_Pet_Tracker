// Lora Driver Spirce File using stm32wlxx_hal_subghz.c
#include "lora.h"

SUBGHZ_HandleTypeDef hsubghz;

bool lora_init()
{
    // #define LL_SPI_BAUDRATEPRESCALER_DIV4      (SPI_CR1_BR_0)  /*!< BaudRate control equal to fPCLK/4   */
    hsubghz.Init.BaudratePrescaler = LL_SPI_BAUDRATEPRESCALER_DIV8; // What is out baud rate?
    if (HAL_SUBGHZ_Init(&hsubghz) != HAL_OK)
        return false;

    // No mutex

    // Put radio into sleep and LoRa mode
    HAL_SUBGHZ_WriteRegister(&hsubghz, 0x01, 0x80); // RH_RF95_MODE_SLEEP | RH_RF95_LONG_RANGE_MODE, line 74
    HAL_Delay(10);

    uint8_t mode; // RH_RF95_REG_01_OP_MODE equivalent
    HAL_SUBGHZ_ReadRegister(&hsubghz, 0x01, &mode); // reads the value of a register from the LoRa module.
    return (mode & 0x80) == 0x80; // makes sure we have a mode by checking most significant digit of mode
}
