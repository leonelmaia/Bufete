/*
 * sensor_panel.h
 *
 *  Created on: 28 de ago de 2019
 *      Author: Lesc
 */

#ifndef SENSOR_PANEL_H_
#define SENSOR_PANEL_H_
#include "adc.h"
#include "main.h"
#include "led_panel.h"

typedef struct SENSOR_T{
	ADC_HandleTypeDef *adc;
	uint32_t 		   channel;
	led_t             *led;
}sensor_t;

typedef struct SENSOR_PANEL_T {
	sensor_t *sensors;          /*!< Vetor dos leds pertencentes ao painel */
	uint8_t num_of_sensors;  /*!< Número de leds presentes no painel */
}sensor_panel_t;



void sensor_panel_turn_on(sensor_t *sensor);

#endif /* SENSOR_PANEL_H_ */
