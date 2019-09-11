/*
 * sensor_panel.c
 *
 *  Created on: 28 de ago de 2019
 *      Author: Lesc
 */
#include "sensor_panel.h"
#include "bsp.h"

void sensor_panel_turn_on(sensor_t *sensor)
{
	  ADC_ChannelConfTypeDef sConfig = {0};
	  sensor->adc->Init.NbrOfConversion = 1;

	   sConfig.Channel = sensor->channel;
	   sConfig.Rank = ADC_REGULAR_RANK_1;
	   sConfig.SingleDiff = ADC_SINGLE_ENDED;
	   sConfig.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
	   sConfig.OffsetNumber = ADC_OFFSET_NONE;
	   sConfig.Offset = 0;

	  if (HAL_ADC_ConfigChannel(sensor->adc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  if (bsp_turn_on_ADC_it(sensor->adc) != HAL_OK)
	  {
		  Error_Handler();
	  }
}
