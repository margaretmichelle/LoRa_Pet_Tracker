#ifndef M20071_GPS_H
#define M20071_GPS_H

#include <stm32wlxx_hal.h>
#include <stm32wlxx_hal_uart.h>
#include <stm32wlxx_hal_gpio.h>
#include <stm32wlxx_hal_rcc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _NMEA_DATA_H
#define _NMEA_DATA_H

// Define nmea_float_t type
#ifndef NMEA_FLOAT_T
#define NMEA_FLOAT_T float
#endif
typedef NMEA_FLOAT_T nmea_float_t;

#endif // _NMEA_DATA_H

#define GPS_UART_BUFFER_SIZE 128    // Adjust based on expected message length
#define GPS_CMD_TIMEOUT      2000   // UART polling timeout in ms

typedef struct {
    nmea_float_t latitudeDegrees;  ///< Latitude in decimal degrees
    nmea_float_t longitudeDegrees; ///< Longitude in decimal degrees
    uint8_t lat;   // 0 = North, 1 = South
    uint8_t lon;   // 0 = East, 1 = West

    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    uint8_t year;          ///< GMT year
    uint8_t month;         ///< GMT month
    uint8_t day;           ///< GMT day

    bool fix;
} M20071_GPS_Data;

typedef struct {
    UART_HandleTypeDef *huart;        // UART handle
    uint8_t txBuffer[GPS_UART_BUFFER_SIZE];   // Buffer for outgoing commands
    uint8_t rxBuffer[GPS_UART_BUFFER_SIZE];   // Buffer for incoming GPS responses
    M20071_GPS_Data data;
} M20071_GPS_HandleTypeDef;

void M20071_GPS_Init(M20071_GPS_HandleTypeDef *gps, UART_HandleTypeDef *huart);
HAL_StatusTypeDef M20071_GPS_SendCommand(M20071_GPS_HandleTypeDef *gps, const char *cmd);
HAL_StatusTypeDef M20071_GPS_Receive(M20071_GPS_HandleTypeDef *gps);

bool GPS_powerOn(M20071_GPS_HandleTypeDef *gps);
bool GPS_powerOff(M20071_GPS_HandleTypeDef *gps);
bool GPS_systemReboot(M20071_GPS_HandleTypeDef *gps);
bool GPS_setNMEARate(M20071_GPS_HandleTypeDef *gps, uint8_t type, uint8_t rate);
bool GPS_setPeriodicMode(M20071_GPS_HandleTypeDef *gps, uint8_t mode, uint8_t firstRun, uint8_t firstSleep, uint8_t secondRun, uint8_t secondSleep);
bool GPS_enterRTC(M20071_GPS_HandleTypeDef *gps);
bool GPS_openIOPort(M20071_GPS_HandleTypeDef *gps, uint8_t portIndex);
bool GPS_closeIOPort(M20071_GPS_HandleTypeDef *gps, uint8_t portIndex);
bool GPS_testIOPort(M20071_GPS_HandleTypeDef *gps);

int GPS_Parse(M20071_GPS_HandleTypeDef *gps);
bool GPS_ParseNMEA(char *nmea, M20071_GPS_HandleTypeDef *gps);

void GPS_FormatData(uint8_t dataGPS[], M20071_GPS_HandleTypeDef *gps);

void addChecksum(char *msg);
bool validateChecksum(const uint8_t *msg);

bool isEmpty(char *pStart);
bool parseTime(char *p, M20071_GPS_Data *data);
bool parseFix(char *p, M20071_GPS_Data *data);
bool parseLatLon(char *p, bool latFlag, M20071_GPS_Data *data);

// Output Flags
#define GNSS_IO_FLAG_OUT_NMEA        (0x01)  // Output NMEA sentences (GPS data)
#define GNSS_IO_FLAG_OUT_LOG         (0x02)  // Output log data (e.g., stored position history)
#define GNSS_IO_FLAG_OUT_CMD_RSP     (0x04)  // Output command response (acknowledgment, errors)
#define GNSS_IO_FLAG_OUT_DATA_RSP    (0x08)  // Output data response (specific data, e.g., position)
#define GNSS_IO_FLAG_OUT_RTCM        (0x10)  // Output RTCM correction data (for RTK positioning)

// Input Flags
#define GNSS_IO_FLAG_IN_CMD          (0x20)  // Input command (e.g., configuration or control commands)
#define GNSS_IO_FLAG_IN_DATA         (0x40)  // Input data (e.g., sensor data, configuration updates)
#define GNSS_IO_FLAG_IN_RTCM         (0x80)  // Input RTCM correction data (for RTK positioning)

#endif // M20071_GPS_H
