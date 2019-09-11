/*
 * led_panel.c
 *
 *  Created on: 12 de ago de 2019
 *      Author: Leonel Maia
 */

#ifndef LED_PANEL_C_
#define LED_PANEL_C_
#include "main.h"
/**
 * @brief Modelagem de cada led usando sua porta e pino
 */
typedef struct LED_T{
	GPIO_TypeDef *gpio_port;
	uint16_t     gpio_pin;
}led_t;

typedef struct LED_PANEL_T {
	led_t *leds;          /*!< Vetor dos leds pertencentes ao painel */
	uint8_t num_of_leds;  /*!< Número de leds presentes no painel */
}led_panel_t;


void led_panel_init(led_panel_t *led_panel);
uint32_t led_panel_write(led_panel_t *led_panel, uint16_t led_panel_value);


#endif /* LED_PANEL_C_ */
