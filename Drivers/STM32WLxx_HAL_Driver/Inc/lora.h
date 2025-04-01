// Lora Driver Header File using stm32wlxx_hal_subghz.h
#ifndef LORA_H
#define LORA_H

#include "stm32wlxx_hal_subghz.h"

#include "stm32wlxx_hal_def.h"

/* Include low level driver */
#include "stm32wlxx_ll_spi.h"

#include <stdbool.h>
#include <stdint.h>

// declare the handle from stm32wlxx_hal_subghz.h
extern SUBGHZ_HandleTypeDef hsubghz;

// initializes the SubGHz peripheral and puts the radio into LoRa sleep mode
bool lora_init();

// Sends a data packet of 'len' bytes over LoRa
bool lora_send(const uint8_t* data, uint8_t len);

// Recieves a data packet of 'len' bytes over LoRa
bool lora_recieve(const uint8_t* data, uint8_t len);

// Waits for the packet to be fully sent, with timeout in milliseconds
bool lora_waitPacketSent(uint32_t timeout);

// Waits until a packet is available to read or until timeout expires
bool lora_waitAvailableTimeout(uint32_t timeout);

// Sets the transmit frequency in MHz (e.g., 868.0 or 915.0)
bool lora_setFrequency(float center);

// sets the transmit power level
bool lora_setTxPower(uint8_t power);

// Checks whether a packet is available (RX done)
bool lora_available();

#endif 
