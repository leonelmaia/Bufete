/*
 * hal.h
 *
 *  Created on: 29 de ago de 2019
 *      Author: Lesc
 */

#ifndef HAL_H_
#define HAL_H_


#include "stm32f3xx_hal.h"
#include "ringbuf.h"
#include "tim.h"
#include "adc.h"
#include "gpio.h"
#include "message.h"
#include "usart.h"
#include "led_panel.h"
#include "sensor_panel.h"


void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define Size_Of_Ring_Buffer 255
uint8_t buffer_Command[Size_Of_Ring_Buffer];
struct ringbuf command_Handle;
#define START_BYTE           0x7E
#define END_BYTE             0x7E
#define ERROR_BYTE           0x99
#define ESCAPE_BYTE          0x7D
#define  ADDITIONAL_BTYE_5E  0x5E
#define  ADDITIONAL_BTYE_5D  0x5D
#define AD_POSITIVE_TOUCH    0xFF
#define MAX_TIME			 0xFFFFFFFF
#define TIMEOUT_5S           500000
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED8_Pin GPIO_PIN_13
#define LED8_GPIO_Port GPIOC
#define LED9_Pin GPIO_PIN_14
#define LED9_GPIO_Port GPIOC
#define LED10_Pin GPIO_PIN_15
#define LED10_GPIO_Port GPIOC
#define AD9_Pin GPIO_PIN_0
#define AD9_GPIO_Port GPIOA
#define AD10_Pin GPIO_PIN_1
#define AD10_GPIO_Port GPIOA
#define AD7_Pin GPIO_PIN_2
#define AD7_GPIO_Port GPIOA
#define AD8_Pin GPIO_PIN_3
#define AD8_GPIO_Port GPIOA
#define AD5_Pin GPIO_PIN_4
#define AD5_GPIO_Port GPIOA
#define AD6_Pin GPIO_PIN_5
#define AD6_GPIO_Port GPIOA
#define AD3_Pin GPIO_PIN_6
#define AD3_GPIO_Port GPIOA
#define AD4_Pin GPIO_PIN_0
#define AD4_GPIO_Port GPIOB
#define AD1_Pin GPIO_PIN_1
#define AD1_GPIO_Port GPIOB
#define AD2_Pin GPIO_PIN_2
#define AD2_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_4
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_5
#define LED3_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_6
#define LED4_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_7
#define LED5_GPIO_Port GPIOB
#define LED6_Pin GPIO_PIN_8
#define LED6_GPIO_Port GPIOB
#define LED7_Pin GPIO_PIN_9
#define LED7_GPIO_Port GPIOB




#endif /* HAL_H_ */
