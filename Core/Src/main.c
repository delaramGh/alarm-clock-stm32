/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "ssd1306_font.h"
#include <stdbool.h> 
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
enum ButtonState
{
	BUTTON_DEFAULT,
	BUTTON_HOUR_CONTROL,
	BUTTON_MINUTE_CONTROL,
	BUTTON_SET_ALARM,
	
	BUTTON_SET_CLOCK_HOUR,
	BUTTON_SET_CLOCK_MINUTE,
	BUTTON_SET_CLOCK
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t hour, minute = 0;
char display_buffer[20];

RTC_TimeTypeDef myTime;

uint8_t alarm_hour, alarm_minute = 0;

enum ButtonState button_state = BUTTON_DEFAULT;
char button_state_str[20];

uint8_t test = 0;
uint8_t if_test[4];

bool alarm_flag = false;
bool alarm_on = true;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void write_clock_state();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)

{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	ssd1306_init();
	ssd1306_black_screen();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//CHECK ALARM
		HAL_RTC_GetTime(&hrtc, &myTime, RTC_FORMAT_BIN);
	
		if(button_state == BUTTON_DEFAULT)
			sprintf(display_buffer, "%d:%d:%d     ", myTime.Hours, myTime.Minutes, myTime.Seconds);
		
		else
			sprintf(display_buffer, "%d : %d      ", hour, minute);
		
		alarm_flag = ((myTime.Hours == alarm_hour) && (myTime.Minutes == alarm_minute));
		
		if(alarm_flag && alarm_on)
		{
			sprintf(display_buffer, "! ALARM !     ");
			//Alarm beeping
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
			HAL_Delay(500);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
			HAL_Delay(1000);
		}
		else 
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); //turns off the alarm
		}
			
	
		ssd1306_set_cursor(5, 10);
		ssd1306_write_string(font11x18, display_buffer);
		ssd1306_set_cursor(5, 40);
	  write_clock_state();
		ssd1306_write_string(font11x18, button_state_str);
		ssd1306_update_screen();
		HAL_Delay(10);
  }
}
//_______________________________________________________________________________
//_______________________________________________________________________________


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
	if(GPIO_Pin == GPIO_PIN_12) //pb12 -> minute and hour
	{
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_SET)  //double button
		{	alarm_on = false; HAL_Delay(100);}
			
		else
		{
			if(button_state == BUTTON_HOUR_CONTROL || button_state == BUTTON_SET_CLOCK_HOUR)
			{
				if(hour < 23)
					hour += 1;
				else
					hour = 0;
			}
			else if(button_state == BUTTON_MINUTE_CONTROL || button_state == BUTTON_SET_CLOCK_MINUTE)
			{
			if(minute < 55)
				minute += 5;
			else 
				minute = 0;
		  }
			else if(button_state == BUTTON_SET_ALARM)
			{
				button_state = BUTTON_SET_CLOCK_HOUR;
			}
	  }
		HAL_Delay(50);
	}//pb12
	
	
	if(GPIO_Pin == GPIO_PIN_13)  //pb13 -> mode selector
	{
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET)  //double button
		{	alarm_on = false; HAL_Delay(100);}
		
		else
		{
			if(button_state == BUTTON_DEFAULT)
				button_state = BUTTON_HOUR_CONTROL;
			
			else if(button_state == BUTTON_HOUR_CONTROL)
				button_state = BUTTON_MINUTE_CONTROL;
			
			else if(button_state == BUTTON_MINUTE_CONTROL)
				button_state = BUTTON_SET_ALARM;
			
			else if(button_state == BUTTON_SET_ALARM)
			{
				alarm_hour = hour;
				alarm_minute = minute;
				button_state = BUTTON_DEFAULT;
				alarm_on = true;
			}
			
			else if(button_state == BUTTON_SET_CLOCK_HOUR)
				button_state = BUTTON_SET_CLOCK_MINUTE;
			
			else if(button_state == BUTTON_SET_CLOCK_MINUTE)
				button_state = BUTTON_SET_CLOCK;
		 
			else if(button_state == BUTTON_SET_CLOCK)
			{
				// config the clock
				RTC_TimeTypeDef sTime = {0};
				sTime.Hours = hour;
				sTime.Minutes = minute;
				sTime.Seconds = 0;
				HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				button_state = BUTTON_DEFAULT;
			}
			
		}
		
		HAL_Delay(50);
	}
}



void write_clock_state()
{
		if(button_state == BUTTON_DEFAULT)
			sprintf(button_state_str, "Time, %d   ", (uint8_t)alarm_on);
		else if(button_state == BUTTON_HOUR_CONTROL)
			sprintf(button_state_str, "Set Alarm H");
		else if(button_state == BUTTON_MINUTE_CONTROL)
			sprintf(button_state_str, "Set Alarm M");
		else if(button_state == BUTTON_SET_ALARM)
			sprintf(button_state_str, "Alarm Set! ");
		
		else if(button_state == BUTTON_SET_CLOCK)
			sprintf(button_state_str, "Clock Set! ");
		else if(button_state == BUTTON_SET_CLOCK_HOUR)
			sprintf(button_state_str, "Set Clock H");
		else if(button_state == BUTTON_SET_CLOCK_MINUTE)
			sprintf(button_state_str, "Set Clock M");
}

  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
