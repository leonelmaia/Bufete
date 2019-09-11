/*
 * bsp.c
 *
 *  Created on: 12 de ago de 2019
 *      Author: Lesc
 */

#include "bsp.h"
/* Lista global de acesso aos LEDs. O mapeamento das portas e pinos associados a cada LED
 * feito nesta estrutura.
*/

led_t led_vector[] =
{
  /*LED_10*/
  {
    .gpio_port = LED10_GPIO_Port,
    .gpio_pin  = LED10_Pin,
  },
  {
    .gpio_port = LED9_GPIO_Port,
    .gpio_pin  = LED9_Pin,
  },
  {
    .gpio_port = LED8_GPIO_Port,
    .gpio_pin  = LED8_Pin,
  },
  {
    .gpio_port = LED7_GPIO_Port,
    .gpio_pin  = LED7_Pin,
  },
  {
    .gpio_port = LED6_GPIO_Port,
    .gpio_pin  = LED6_Pin,
  },
  {
    .gpio_port = LED5_GPIO_Port,
    .gpio_pin  = LED5_Pin,
  },
  {
    .gpio_port = LED4_GPIO_Port,
    .gpio_pin  = LED4_Pin,
  },
  {
    .gpio_port = LED3_GPIO_Port,
    .gpio_pin  = LED3_Pin,
  },
  {
    .gpio_port = LED2_GPIO_Port,
    .gpio_pin  = LED2_Pin,
  },
  {
    .gpio_port = LED1_GPIO_Port,
    .gpio_pin  = LED1_Pin,
  },
  /* CONTINUAR OS 10 LEDS*/
};

led_panel_t led_panel =
{
		.leds = led_vector,
		.num_of_leds = 10,
};


sensor_t sensor_vector[] =
{
		  /*SENSOR_10*/
		{
				.adc     = &hadc1,
				.channel = ADC_CHANNEL_2,
				.led	 = &(led_vector[0]),
		},
		{
				.adc     = &hadc1,
				.channel = ADC_CHANNEL_1,
				.led	 = &(led_vector[1]),
		},
		{
				.adc     = &hadc1,
				.channel = ADC_CHANNEL_4,
				.led	 = &(led_vector[2]),
		},
		{
				.adc     = &hadc1,
				.channel = ADC_CHANNEL_3,
				.led	 = &(led_vector[3]),
		},
		{
				.adc     = &hadc2,
				.channel = ADC_CHANNEL_2,
				.led	 = &(led_vector[4]),
		},
		{
				.adc     = &hadc2,
				.channel = ADC_CHANNEL_1,
				.led	 = &(led_vector[5]),
		},
		{
				.adc     = &hadc3,
				.channel = ADC_CHANNEL_12,
				.led	 = &(led_vector[6]),
		},
		{
				.adc     = &hadc2,
				.channel = ADC_CHANNEL_3,
				.led	 = &(led_vector[7]),
		},
		{
				.adc     = &hadc2,
				.channel = ADC_CHANNEL_12,
				.led	 = &(led_vector[8]),
		},
		{
				.adc     = &hadc3,
				.channel = ADC_CHANNEL_1,
				.led	 = &(led_vector[9]),
		},


};

sensor_panel_t sensor_panel =
{
		.sensors = sensor_vector,
		.num_of_sensors = 10,
};

void bsp_low_level_init(void)
{
	  HAL_Init();

	  SystemClock_Config();

	  MX_GPIO_Init();
	  MX_USART1_UART_Init();
	  MX_ADC1_Init();
	  MX_ADC2_Init();
	  MX_ADC3_Init();
	  MX_TIM7_Init();
	  MX_TIM2_Init();
	  MX_TIM3_Init();

	  HAL_ADCEx_Calibration_Start(&hadc1,  ADC_SINGLE_ENDED);
	  HAL_ADCEx_Calibration_Start(&hadc2,  ADC_SINGLE_ENDED);
	  HAL_ADCEx_Calibration_Start(&hadc3 , ADC_SINGLE_ENDED);

	  return;
}


void bsp_init(void)
{
	bsp_low_level_init();

	bsp_led_panel_init();

	bsp_start_time(&htim2);
	char* start = "START\r\n";
	bsp_send_data_it(&huart1, &start, 5);

	ringbuf_init(&command_Handle, buffer_Command , 256);

	return;
}


/**
  * @brief System Clock Configuration
  * @retval None
  */

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
while(1);
  /* USER CODE END Error_Handler_Debug */
}
