/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void parseMessage(int messageType, int messageID, M20071_GPS_HandleTypeDef *hGps);
static bool sendACK(int messageID);
static bool waitForACK();
static uint32_t getRandomBackoff(uint32_t min_ms, uint32_t max_ms);

static void lsm6dsox_activity_init(stmdev_ctx_t dev_ctx);
static bool lsm6dsox_get_event(void *handle, stmdev_ctx_t dev_ctx);
static int32_t lsm6dsox_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len);
static int32_t lsm6dsox_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);
static void lsm6dsox_delay(uint32_t ms);

static void setGPS(M20071_GPS_HandleTypeDef *hGps);
static void getGPS(uint8_t dataGPS[], M20071_GPS_HandleTypeDef *hGps);
static bool sendSingleGPS(int messageID, M20071_GPS_HandleTypeDef *hGps);

static bool sendSleep(int messageID);

UART_HandleTypeDef huart1;
M20071_GPS_HandleTypeDef hGps;
I2C_HandleTypeDef hi2c3;

#define ACK_MESSAGE_TYPE				0 // sent and received
#define GPS_SEND_MESSAGE_TYPE			1
#define GPS_REQUEST_MESSAGE_TYPE		2 // received
#define SEARCH_MODE_MESSAGE_TYPE		3 // received
#define SPEAKER_ON_MESSAGE_TYPE			4 // received
#define SPEAKER_OFF_MESSAGE_TYPE		5
#define BATTERY_SEND_MESSAGE_TYPE		6
#define BATTERY_REQUEST_MESSAGE_TYPE	7 // received
#define SLEEP_MESSAGE_TYPE				8

#define LORA_MAX_MESSAGE_LEN			25 // ??? check number
#define GPS_SIZE 16

#define BOOT_TIME            			10 //ms

static int send_interval = 2; // 5 minutes

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  // LoRa setup
   lora_init();
   lora_setFrequency(902.5);
   lora_setTxPower(17); // 14-22
   lora_setSF(9);

  // GPS setup
  HAL_UART_MspInit(&huart1);
  M20071_GPS_Init(&hGps, &huart1);
  GPS_powerOn(&hGps);
  setGPS(&hGps);

  // IMU setup
   HAL_I2C_MspInit(&hi2c3);
   stmdev_ctx_t dev_ctx;
   lsm6dsox_activity_init(dev_ctx);

  // General setup
  time_t timer;
  timer = HAL_GetTick();

  bool sleep = false;

  int messageID = 0;


  static unsigned short pin_state = 0;
  while (1) {

  pin_state = !pin_state;
  // write pin state
  // NOTE: You can in turn use HAL_GPIO_TogglePin
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, pin_state);
  // synchronous delay for 500 ms
//  lsm6dsox_device_id_get(&dev_ctx, &dummy);

  HAL_Delay(500);
  }

}

static void parseMessage(int messageType, int messageID, M20071_GPS_HandleTypeDef *hGps) {
	switch (messageType) {
		case GPS_REQUEST_MESSAGE_TYPE:
			while(!sendSingleGPS(messageID, hGps)) {
				messageID++;
				HAL_Delay(getRandomBackoff(10, 100));
			}
			messageID++;
			break;
		case SEARCH_MODE_MESSAGE_TYPE:
			if (send_interval == 5) {
				send_interval = 1;
			}
			else {
				send_interval = 5;
			}
			lora_setTxPower(22);
			// change SF
			lora_setSF(12);
			// wait for ACK??
			break;
		case SPEAKER_ON_MESSAGE_TYPE:
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0); // Toggle PA0
			HAL_Delay(500);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
			break;
		case BATTERY_REQUEST_MESSAGE_TYPE:
	//		while(!sendBattery(messageID)) {
	//			messageID++;
	//			HAL_Delay(getRandomBackoff(10, 100));
	//		}
	//		messageID++;
			break;

	}
}

static void setGPS(M20071_GPS_HandleTypeDef *hGps) {
	GPS_openIOPort(hGps, 1); // check port index and baud rate

	for (int i = 0; i <= 8; i++) {
		GPS_setNMEARate(hGps, i, 0);
	}
	GPS_setNMEARate(hGps, 4, 1);
}

static void getGPS(uint8_t dataGPS[], M20071_GPS_HandleTypeDef *hGps) {
	// wake up GPS (pull RTC_EINT high for 1ms)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // RTS high
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET); // RTS low
	setGPS(hGps); // need to re-initialize I think

	while (M20071_GPS_Receive(hGps) == HAL_ERROR); // this is dumb but probably works

	while (GPS_Parse(hGps) != 1) { // check to make sure we received proper NMEA
		while (M20071_GPS_Receive(hGps) == HAL_ERROR);
	}

	// put GPS to sleep
	while(!GPS_enterRTC(hGps));

	GPS_FormatData(dataGPS, hGps);
	dataGPS[15] = 100;  // battery status
}

static bool sendSingleGPS(int messageID, M20071_GPS_HandleTypeDef *hGps) {
	uint8_t message[18];
	message[0] = messageID;
	message[1] = GPS_SEND_MESSAGE_TYPE;
	uint8_t dataGPS[GPS_SIZE];
	getGPS(dataGPS, hGps);
	memcpy(&message[2], dataGPS, GPS_SIZE);

	lora_send(message, 18);
	lora_waitPacketSent(2000);

	return waitForACK();
}

static bool sendSleep(int messageID) {
	uint8_t toSend[] = {messageID, SLEEP_MESSAGE_TYPE};
	lora_send(toSend, sizeof(toSend));
	lora_waitPacketSent(2000);

	return waitForACK();
}

bool sendACK(int messageID) {
 uint8_t toSend[] = {messageID, ACK_MESSAGE_TYPE};
 lora_send(toSend, sizeof(toSend));
 lora_waitPacketSent(2000);

 return true;
}

bool waitForACK() {
 uint8_t buf[LORA_MAX_MESSAGE_LEN];
 uint8_t len = LORA_MAX_MESSAGE_LEN;

 if (lora_waitAvailableTimeout(2000)) {
   if (lora_receive(buf, len)) {
   	if (buf[1] == ACK_MESSAGE_TYPE) {
   		return true;
   	}
   	return false;
   } else {
   	return false;
   }
 } else {
   return false;
 }
}

static uint32_t getRandomBackoff(uint32_t min_ms, uint32_t max_ms) {
   return (rand() % (max_ms - min_ms + 1)) + min_ms;
}

static void lsm6dsox_activity_init(stmdev_ctx_t dev_ctx)
{
 lsm6dsox_pin_int1_route_t int1_route;

 uint8_t dummy;
 /* Initialize mems driver interface */
 dev_ctx.write_reg = lsm6dsox_write;
 dev_ctx.read_reg = lsm6dsox_read;
 dev_ctx.mdelay = lsm6dsox_delay;
 dev_ctx.handle = &hi2c3;
 /* Wait Boot Time */
 lsm6dsox_delay(BOOT_TIME);
 /* Check device ID */
 lsm6dsox_device_id_get(&dev_ctx, &dummy);

// if (dummy != LSM6DSOX_ID)
//   while (1);

// /* Restore default configuration */
// lsm6dsox_reset_set(&dev_ctx, PROPERTY_ENABLE);

// do {
//   lsm6dsox_reset_get(&dev_ctx, &dummy);
// } while (dummy);

// /* Disable I3C interface */
// lsm6dsox_i3c_disable_set(&dev_ctx, LSM6DSOX_I3C_DISABLE);
// /* Set XL and Gyro Output Data Rate */
// lsm6dsox_xl_data_rate_set(&dev_ctx, LSM6DSOX_XL_ODR_208Hz);
// lsm6dsox_gy_data_rate_set(&dev_ctx, LSM6DSOX_GY_ODR_104Hz);
// /* Set 2g full XL scale and 250 dps full Gyro */
// lsm6dsox_xl_full_scale_set(&dev_ctx, LSM6DSOX_2g);
// lsm6dsox_gy_full_scale_set(&dev_ctx, LSM6DSOX_250dps);
// /* Set duration for Activity detection to 9.62 ms (= 2 * 1 / ODR_XL) */
// lsm6dsox_wkup_dur_set(&dev_ctx, 0x02);
// /* Set duration for Inactivity detection to 4.92 s (= 2 * 512 / ODR_XL) */
// lsm6dsox_act_sleep_dur_set(&dev_ctx, 0x02);
// /* Set Activity/Inactivity threshold to 62.5 mg */
// lsm6dsox_wkup_threshold_set(&dev_ctx, 0x02);
// /* Inactivity configuration: XL to 12.5 in LP, gyro to Power-Down */
// lsm6dsox_act_mode_set(&dev_ctx, LSM6DSOX_XL_12Hz5_GY_PD);
// /* Enable interrupt generation on Inactivity INT1 pin */
// lsm6dsox_pin_int1_route_get(&dev_ctx, &int1_route);
// int1_route.sleep_change = PROPERTY_ENABLE;
// lsm6dsox_pin_int1_route_set(&dev_ctx, int1_route);
}

/*
* Get activity event
* @return : True if awake, False if asleep
*/
static bool lsm6dsox_get_event(void *handle, stmdev_ctx_t dev_ctx) {
	lsm6dsox_all_sources_t all_source;

	/* Check if Activity/Inactivity events */
	lsm6dsox_all_sources_get(&dev_ctx, &all_source);

	if (all_source.wake_up) {
   // TODO: do something if in wakeup
   // snprintf((char *)tx_buffer, sizeof(tx_buffer), "Activity Detected\r\n");
		return 1;
	}
	else{
		return 0;
	}
}

/*
* @brief  Write generic device register (platform dependent)
*
* @param  handle    customizable argument. In this examples is used in
*                   order to select the correct sensor bus handler.
* @param  reg       register to write
* @param  bufp      pointer to data to write in register reg
* @param  len       number of consecutive register to write
*
*/
static int32_t lsm6dsox_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
 HAL_I2C_Master_Transmit(handle, LSM6DSOX_I2C_ADD_L, &reg, 1, 1000);
 HAL_I2C_Master_Transmit(handle, LSM6DSOX_I2C_ADD_L, (uint8_t*) bufp, len, 1000);
 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
 return 0;
}

/*
* @brief  Read generic device register (platform dependent)
*
* @param  handle    customizable argument. In this examples is used in
*                   order to select the correct sensor bus handler.
* @param  reg       register to read
* @param  bufp      pointer to buffer that store the data read
* @param  len       number of consecutive register to read
*
*/
static int32_t lsm6dsox_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
 reg |= 0x80;
 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
 HAL_I2C_Master_Transmit(handle, LSM6DSOX_I2C_ADD_L, &reg, 1, 1000);
 HAL_I2C_Master_Receive(handle, LSM6DSOX_I2C_ADD_L, bufp, len, 1000);
 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
 return 0;
}

/*
 * @brief  platform specific delay (platform dependent)
 *
 * @param  ms        delay in ms
 *
 */
static void lsm6dsox_delay(uint32_t ms)
{
  HAL_Delay(ms);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
