#include "m20071_gps.h"
#include <string.h>
#include <stdio.h>

void M20071_GPS_Init(M20071_GPS_HandleTypeDef *gps, UART_HandleTypeDef *huart) {
    gps->huart = huart;
    memset(gps->txBuffer, 0, GPS_UART_BUFFER_SIZE);
    memset(gps->rxBuffer, 0, GPS_UART_BUFFER_SIZE);
}

/**
 * @brief Sends a GPS command over UART
 * @param gps: Pointer to the GPS handle
 * @param cmd: Command string (e.g., "$PAIR690,1,21,39,48,72*28\r\n")
 * @return HAL_StatusTypeDef: HAL_OK if successful, otherwise error status
 */
HAL_StatusTypeDef M20071_GPS_SendCommand(M20071_GPS_HandleTypeDef *gps, const char *cmd) {
    uint16_t len = strlen(cmd);
    
    if (len >= GPS_UART_BUFFER_SIZE) {
        return HAL_ERROR;
    }

    // Clear the TX buffer and copy the command
    memset(gps->txBuffer, 0, GPS_UART_BUFFER_SIZE);
    strncpy((char *)gps->txBuffer, cmd, len);

    // Send the command using UART polling
    return HAL_UART_Transmit(gps->huart, gps->txBuffer, len, GPS_CMD_TIMEOUT);
}

/**
 * @brief Calculates checksum of string.
 * @param msg: Pointer to the message
 */
void addChecksum(char *msg) {
  const char *star = strchr(msg, '*');

  uint8_t calculatedChecksum = 0;
  // Compute XOR of all characters between $ and *
  for (const char *p = msg + 1; p < star; ++p) {
      calculatedChecksum ^= *p;
  }

  sprintf(star + 1, "%02X\r\n", calculatedChecksum);
}

/**
 * @brief Power on the GNSS system, must send this command before using lacation services
 * @param gps: Pointer to the GPS handle
 * @return True if command was sent successfully, false otherwise
 */
bool powerOn(M20071_GPS_HandleTypeDef *gps) {
  char cmd[40] = "$PAIR002*";
  addChecksum(cmd);

  if (M20071_GPS_SendCommand(gps, cmd) == HAL_OK) {
      return true;
  }
  return false;
}

/**
 * @brief Power off the GNSS system
 * @param gps: Pointer to the GPS handle
 * @return True if command was sent successfully, false otherwise
 */
bool powerOff(M20071_GPS_HandleTypeDef *gps) {
  char cmd[40] = "$PAIR003*";
  addChecksum(cmd);

  if (M20071_GPS_SendCommand(gps, cmd) == HAL_OK) {
      return true;
  }
  return false;
}

/**
 * @brief Reboot whole chip
 * @param gps: Pointer to the GPS handle
 * @return True if command was sent successfully, false otherwise
 */
bool systemReboot(M20071_GPS_HandleTypeDef *gps) {
  char cmd[40] = "$PAIR023*";
  addChecksum(cmd);

  if (M20071_GPS_SendCommand(gps, cmd) == HAL_OK) {
      return true;
  }
  return false;
}

/**
 * @brief Set the NMEA sentence output interval
 * @param gps: Pointer to the GPS handle
 * @param rate: NMEA sentence type (4 is RMC)
 * @param rate: Output rate, range: 0-20 (how many times every position fix)
 * @return True if command was sent successfully, false otherwise
 */
bool setNMEARate(M20071_GPS_HandleTypeDef *gps, uint8_t type, uint8_t rate) {
  char cmd[40]; 
  sprintf(cmd, "$PAIR062,%d,%d*", type, rate);
  addChecksum(cmd);

  if (M20071_GPS_SendCommand(gps, cmd) == HAL_OK) {
    return true;
  }
  return false;
}

/**
 * @brief Set periodic mode settings
 * @param gps: Pointer to the GPS handle
 * @param mode: 0 disable periodic mode, 1 smart periodic mode, 2 strict periodic mode
 * @param firstRun: Interval in seconds to be awake and get new position fix in normal periodic mode operation (3~518400 s)
 * @param firstSleep: Duration in seconds to sleep in normal periodic mode operation
 * @param secondRun: Interval in seconds to be awake when there is no signal
 * @param secondSleep: Duration in seconds to sleep when there is no signal
 * @return True if command was sent successfully, false otherwise
 */
bool setPeriodicMode(M20071_GPS_HandleTypeDef *gps, uint8_t mode, uint8_t firstRun, uint8_t firstSleep, uint8_t secondRun, uint8_t secondSleep) {
  char cmd[40];
  if (mode == 0) {
    strcpy(cmd, "$PAIR690,0*");
  }
  else {
    sprintf(cmd, "$PAIR690,%d,%d,%d,%d,%d*", mode, firstRun, firstSleep, secondRun, secondSleep);
  }
  addChecksum(cmd);

  if (M20071_GPS_SendCommand(gps, cmd) == HAL_OK) {
    return true;
  }
  return false;
}

/**
 * @brief Open a GNSS data port
 * @param gps: Pointer to the GPS handle
 * @param portIndex: HW port index
 * @param baudRate: Baud rate
 * @return True if command was sent successfully, false otherwise
 */
bool openIOPort(M20071_GPS_HandleTypeDef *gps, uint8_t portIndex, uint8_t baudRate) {
  char cmd[40];
  sprintf(cmd, "$PAIR860,0,%d,%d,%d,0*", portIndex, GNSS_IO_FLAG_OUT_NMEA | GNSS_IO_FLAG_OUT_CMD_RSP | GNSS_IO_FLAG_OUT_DATA_RSP | GNSS_IO_FLAG_IN_CMD, baudRate);
  addChecksum(cmd);

  if (M20071_GPS_SendCommand(gps, cmd) == HAL_OK) {
    return true;
  }
  return false;
}

/**
 * @brief Close a GNSS data port
 * @param gps: Pointer to the GPS handle
 * @param portIndex: HW port index
 * @return True if command was sent successfully, false otherwise
 */
bool closeIOPort(M20071_GPS_HandleTypeDef *gps, uint8_t portIndex) {
  char cmd[40];
  sprintf(cmd, "$PAIR861,0,%d*", portIndex);
  addChecksum(cmd);

  if (M20071_GPS_SendCommand(gps, cmd) == HAL_OK) {
    return true;
  }
  return false;
}

/**
 * @brief Check if PAIR channel is ready to work
 * @param gps: Pointer to the GPS handle
 * @return True if command was sent successfully, false otherwise
 */
bool testIOPort(M20071_GPS_HandleTypeDef *gps) {
  char cmd[40] = "$PAIR870*";
  addChecksum(cmd);

  if (M20071_GPS_SendCommand(gps, cmd) == HAL_OK) {
      return true;
  }
  return false;
}

/**
 * @brief Receives a variable-length response from the GPS module
 * @param gps: Pointer to the GPS handle
 * @return HAL_StatusTypeDef: HAL_OK if successful, otherwise error status
 */
HAL_StatusTypeDef M20071_GPS_Receive(M20071_GPS_HandleTypeDef *gps) {
  uint8_t byte;
  uint16_t index = 0;

  memset(gps->rxBuffer, 0, GPS_UART_BUFFER_SIZE);

  while (index < GPS_UART_BUFFER_SIZE - 1) {
      // Receive one byte at a time
      if (HAL_UART_Receive(gps->huart, &byte, 1, GPS_CMD_TIMEOUT) != HAL_OK) {
          return HAL_TIMEOUT;  // Exit if timeout occurs
      }

      gps->rxBuffer[index++] = byte;

      // End reception when detecting \r\n
      if (index >= 2 && gps->rxBuffer[index - 2] == '\r' && gps->rxBuffer[index - 1] == '\n') {
        gps->rxBuffer[index] = '\0';  // Null-terminate the string

        if (validateChecksum(gps->rxBuffer)) {
          return HAL_OK;
        } else {
          return HAL_ERROR; // checksum not checking or summing
        }
      }
  }

  return HAL_ERROR;  // ahhh too long
}

/**
 * @brief Validates the checksum of a message.
 * @param msg: Pointer to the message
 * @return 1 if checksum is valid, 0 if not
 */
bool validateChecksum(const uint8_t *msg) {
  if (msg == NULL || *msg != '$') {
      return 0;  // Invalid format
  }

  const uint8_t *star = (uint8_t *)strchr((char *)msg, '*');
  if (star == NULL || star - msg < 1) {
      return 0;  // No checksum found, invalid message
  }

  // Compute checksum (XOR of all characters between preamble $ and *)
  uint8_t calculatedChecksum = 0;
  for (const uint8_t *p = msg + 1; p < star; ++p) {
      calculatedChecksum ^= *p;
  }

  // Extract the checksum from the sentence
  uint8_t receivedChecksum = (uint8_t)strtol((char *)(star + 1), NULL, 16);

  return (calculatedChecksum == receivedChecksum);
}

/**
 * @brief Determines which parsing function to use for message (NMEA or PAIR)
 * @param gps: Pointer to the GPS handle
 */
void M20071_GPS_Parse(M20071_GPS_HandleTypeDef *gps) {
  char *p = gps->rxBuffer + 1;  // Move to first character after $

  // Check for PAIR
  if (p[0] == 'P' && p[1] == 'A' && p[2] == 'I' && p[3] == 'R') {
      // return 1;
  }

  // Check for RMC (any talker ID: $xxRMC)
  p = gps->rxBuffer + 3;
  if (p[0] == 'R' && p[1] == 'M' && p[2] == 'C') {
    p = gps->rxBuffer;
    M20071_GPS_ParseNMEA(char *p);
  }

  return 0; // Unknown message
}


/* The following code is modified from https://github.com/adafruit/Adafruit_GPS/blob/master/src/NMEA_parse.cpp */

/**
 * @brief Parse RMC NMEA sentences
 * @param msg: Pointer to the message starting after SentenceID
 * @return True if successfully parsed, false if it fails check or parsing
 */
bool M20071_GPS_ParseNMEA(char *nmea) {

  char *p = nmea; // Pointer to move through sentence
  p = strchr(p, ',') + 1; // Skip to char after the next comma

  parseTime(p);
  p = strchr(p, ',') + 1;
  parseFix(p);
  p = strchr(p, ',') + 1;
  // parse out both latitude and direction, then go to next field, or fail
  parseLatLon(p, true);
  p = strchr(p, ',') + 1;
  // parse out both longitude and direction, then go to next field, or fail
  parseLatLon(p, false);
  p = strchr(p, ',') + 1;
  // speed over ground
  p = strchr(p, ',') + 1;
  // course over ground
  p = strchr(p, ',') + 1;
  if (!isEmpty(p)) {
    uint32_t fulldate = atof(p);
    day = fulldate / 10000;
    month = (fulldate % 10000) / 100;
    year = (fulldate % 100);
  }
}

/**************************************************************************/
/*!
    @brief Is the field empty, or should we try conversion? Won't work
    for a text field that starts with an asterisk or a comma, but that
    probably violates the NMEA-183 standard.
    @param pStart Pointer to the location of the token in the NMEA string
    @return true if empty field, false if something there
*/
/**************************************************************************/
bool isEmpty(char *pStart) {
  return (*pStart == ',' || *pStart == '*' || pStart == NULL);
}

/**************************************************************************/
/*!
    @brief Parse a part of an NMEA string for time. Independent of number
    of decimal places after the '.'
    @param p Pointer to the location of the token in the NMEA string
    @return true if successful, false otherwise
*/
/**************************************************************************/
bool parseTime(char *p) {
  if (!isEmpty(p)) {
    uint32_t time = atol(p);
    hour = time / 10000;
    minute = (time % 10000) / 100;
    seconds = (time % 100);
    char *dec = strchr(p, '.');
    char *comstar = min(strchr(p, ','), strchr(p, '*'));
    if (dec != NULL && comstar != NULL && dec < comstar)
      milliseconds = atof(dec) * 1000;
    else
      milliseconds = 0;
    return true;
  }
  return false;
}

/**************************************************************************/
/*!
    @brief Parse a part of an NMEA string for whether there is a fix
    @param p Pointer to the location of the token in the NMEA string
    @return True if we parsed it, false if it has invalid data
*/
/**************************************************************************/
bool parseFix(char *p) {
  if (!isEmpty(p)) {
    if (p[0] == 'A') {
      fix = true;
      return true;
    } else if (p[0] == 'V')
      fix = false;
      return true;
  }
  return false;
}

/**************************************************************************/
/*!
    @brief Parse a part of an NMEA string for latitude (ll.llll,a) and longitude (yyy.yyyy,a) data
    @param p Pointer to the location of the token in the NMEA string
    @param latFlag 1 if parsing latitude, 0 if parsing longitude
    @return True if we parsed it, false if it has invalid data
*/
/**************************************************************************/
bool parseLatLon(char *p, bool latFlag) {
  if (!isEmpty(p)) {
    char *e = strchr(p, ',');
    if (e == NULL || e - p > 8) {
      return false;
    }

    char coordValue[16] = {0};
    strncpy(coordValue, p, e - p); // get DD
    
    p = e + 1;

    if (latFlag) {  // if parsing latitude
      latitudeDegrees = atof(coordValue);
      lat = (p[0] == 'S') ? 1 : 0;  // 1 if S, 0 if N
    }
    else {  // if parsing longitude
      longitudeDegrees = atof(coordValue);
      lon = (p[0] == 'W') ? 1 : 0;  // 1 if W, 0 if E
    }

    return true;
  }
  return false;
}