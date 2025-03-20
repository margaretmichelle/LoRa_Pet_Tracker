#ifndef M20071_GPS_H
#define M20071_GPS_H

#include "stm32wlxx_hal.h"
#include <stdint.h>

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
    UART_HandleTypeDef *huart;        // UART handle
    uint8_t txBuffer[GPS_UART_BUFFER_SIZE];   // Buffer for outgoing commands
    uint8_t rxBuffer[GPS_UART_BUFFER_SIZE];   // Buffer for incoming GPS responses
} M20071_GPS_HandleTypeDef;

void M20071_GPS_Init(M20071_GPS_HandleTypeDef *gps, UART_HandleTypeDef *huart);
HAL_StatusTypeDef M20071_GPS_SendCommand(M20071_GPS_HandleTypeDef *gps, const char *cmd);
HAL_StatusTypeDef M20071_GPS_Receive(M20071_GPS_HandleTypeDef *gps);

bool GPS_powerOn(M20071_GPS_HandleTypeDef *gps);
bool GPS_powerOff(M20071_GPS_HandleTypeDef *gps);
bool GPS_systemReboot(M20071_GPS_HandleTypeDef *gps);
bool GPS_setNMEARate(M20071_GPS_HandleTypeDef *gps, uint8_t type, uint8_t rate);
bool GPS_setPeriodicMode(M20071_GPS_HandleTypeDef *gps, uint8_t mode, uint8_t firstRun, uint8_t firstSleep, uint8_t secondRun, uint8_t secondSleep);
bool GPS_openIOPort(M20071_GPS_HandleTypeDef *gps, uint8_t portIndex, uint8_t baudRate);
bool GPS_closeIOPort(M20071_GPS_HandleTypeDef *gps, uint8_t portIndex);
bool GPS_testIOPort(M20071_GPS_HandleTypeDef *gps);

void M20071_GPS_Parse(M20071_GPS_HandleTypeDef *gps);
void M20071_GPS_ParseNMEA(M20071_GPS_HandleTypeDef *gps);

private {
    void addChecksum(char *msg);
    bool validateChecksum(const uint8_t *msg);

    bool isEmpty(char *pStart);
    bool parseTime(char *p);
    bool parseFix(char *p);
    bool parseLatLon(char *p, bool latFlag);
}


uint8_t hour;          ///< GMT hours
uint8_t minute;        ///< GMT minutes
uint8_t seconds;       ///< GMT seconds
uint16_t milliseconds; ///< GMT milliseconds
uint8_t year;          ///< GMT year
uint8_t month;         ///< GMT month
uint8_t day;           ///< GMT day

nmea_float_t latitudeDegrees;  ///< Latitude in decimal degrees
nmea_float_t longitudeDegrees; ///< Longitude in decimal degrees
bool lat = 0;    ///< N(0)/S(1)
bool lon = 0;    ///< E(0)/W(1)
bool fix;          ///< Have a fix?

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
