/*
 * bsp.h
 *
 *  Created on: 12 de ago de 2019
 *      Author: Lesc
 */

#ifndef BSP_H_
#define BSP_H_
#include "led_panel.h"
#include "sensor_panel.h"
#include "hal.h"

sensor_panel_t sensor_panel;
led_panel_t led_panel;

void SystemClock_Config(void);
void bsp_init(void);
void bsp_low_level_init(void);

#define bsp_led_panel_init()                       					 led_panel_init(&led_panel)
#define bsp_led_panel_write(__led_panel_value)    					 led_panel_write(&led_panel, __led_panel_value)
#define bsp_turn_on_ADC_it(__AD)                      				 HAL_ADC_Start_IT(__AD)
#define bsp_turn_off_ADC_it(__AD)                     				 HAL_ADC_Stop_IT(__AD)
#define bsp_start_time(__timer)                   					 HAL_TIM_Base_Start_IT(__timer)
#define bsp_stop_time(__timer)                    					 HAL_TIM_Base_Stop_IT(__timer)
#define bsp_send_data(__uart,__pointer,__ndata,__timeout)            HAL_UART_Transmit(__uart,__pointer,__ndata,__timeout)
#define bsp_receive_data(__uart,__pointer,__ndata,__timeout)         HAL_UART_Receive(__uart,__pointer,__ndata,__timeout)
#define bsp_send_data_it(__uart,__pointer,__ndata)                   HAL_UART_Transmit_IT(__uart,__pointer,__ndata)
#define bsp_receive_data_it(__uart,__pointer,__ndata)                HAL_UART_Receive_IT(__uart,__pointer,__ndata)

#endif /* BSP_H_ */
-
