/*
 * bufete_fsm.c
 *
 *  Created on: 22 de ago de 2019
 *      Author: Lesc
 */

#include "bufete_fsm.h"
#include "hal.h"

bufete_st_function_t st_function[BUFETE_NUM_STATES];
uint16_t ad_read_value = 0;
uint8_t id_led_on      = 0;
uint32_t count_ticks   = 0;
uint8_t ad_check;
uint8_t time_Value[5];

#define bufete_sleep() HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI)
#define bufete_change_state(__bufete, __state) (__bufete->state = __state)
#define bufete_check_event(__bufete, __event) (__event = __bufete->message->message_event)


void bufete_init(bufete_t *bufete, msg_t *message)
{
	bufete->state = BUFETE_STATE_AUTHENTICATION;
	bufete->enable_timeout = BUFETE_ENABLE_TIMEOUT;
	bufete->timeout = TIMEOUT_5S;
	bufete->message = message;

	st_function[BUFETE_STATE_IDLE           ]    = bufete_idle;
	st_function[BUFETE_STATE_TURN_ON_LEDS   ]    = bufete_turn_on_leds;
	st_function[BUFETE_STATE_TIME_COUNTING  ]    = bufete_time_counting;
	st_function[BUFETE_STATE_AUTHENTICATION ]    = bufete_authentication;
	st_function[BUFETE_STATE_ANSWER_SOFTEC  ]    = bufete_answer_softec;

	return;

}
void bufete_iterate(bufete_t *bufete)
{
	st_function[bufete->state](bufete);
	bufete_sleep();
}

void bufete_idle(bufete_t *bufete)
{
	message_event_t event_message;
	bufete_check_event(bufete, event_message);

	if(event_message == NEW_MESSAGE)
	{
		  receive_message(bufete->message);
		  decoder_message(bufete->message);
		  bufete_change_state(bufete, BUFETE_STATE_TURN_ON_LEDS);
	}

}
void bufete_turn_on_leds(bufete_t *bufete)
{
	uint16_t led_panel_value;

	if(bufete->message->command == REGULAR_OPERATION || bufete->message->command == TESTE_OPERATION)
	{
		for (uint8_t index = 1; index <= bufete->message->size_payload; index ++)
		{
			if (*(bufete->message->payload + index-1)){
				led_panel_value = led_panel_value | 1<<(led_panel.num_of_leds - index);
			}
		}
	}

	bufete_check_id_led(led_panel_value);

	if(bufete->message->command == REGULAR_OPERATION)
	{
		  bsp_led_panel_write(led_panel_value);
		  count_ticks = 0;
		  bsp_start_time(&htim7);
		  sensor_panel_turn_on(&(sensor_panel.sensors[sensor_panel.num_of_sensors - id_led_on]));
		  bufete_change_state(bufete, BUFETE_STATE_TIME_COUNTING);
	}
	else{
		if (bufete->message->command == TESTE_OPERATION)
		{
			bsp_led_panel_write(led_panel_value);
			bsp_start_time(&htim3);
			bufete_build_message(bufete, time_Value);
			bufete_change_state(bufete, BUFETE_STATE_ANSWER_SOFTEC);
		}
		else{
			bufete_change_state(bufete, BUFETE_STATE_IDLE);
		}
	}

}

void bufete_build_message(bufete_t *bufete, uint8_t* time)
{
	if (bufete->message->command == TESTE_OPERATION)
	{
		bufete->message->payload = &id_led_on;
		bufete->message->size_payload = 1;

	}
	if (bufete->message->command == REGULAR_OPERATION)
	{
		time[0] = id_led_on;
		uint32_to_uint8(count_ticks, &time[1]);
		bufete->message->payload = time;
		bufete->message->size_payload = 5;

	}
}

void bufete_check_id_led(uint16_t led_panel_value)
{
	for (int index = 0 ; index < led_panel.num_of_leds; index++)
	{
		if(led_panel_value >> index){
			id_led_on = led_panel.num_of_leds - index;
		}
	}
}


void bufete_authentication(bufete_t *bufete)
{
	uint8_t data_in_buffer = ringbuf_read_available(&command_Handle);
	if(data_in_buffer > 2 )
	{
		if (strstr((char*)buffer_Command,(const char*)  AUTHENTIFICATION_MSG) != 0)
		{
			bufete->message->SOM = 0x7E;
			bufete->message->command = AUTHENTICATION;
			bufete->message->payload = &(bufete->message->command);
			bufete->message->EOM = 0x7E;
			bufete->message->size_payload = 1;
			bufete_change_state(bufete, BUFETE_STATE_ANSWER_SOFTEC);
		}
			memset(buffer_Command, 't', Size_Of_Ring_Buffer);   // clear the buffer
			command_Handle.rptr = 0;
			command_Handle.wptr = 0;
	}
}
void bufete_time_counting(bufete_t *bufete)
{

	if(ad_check == AD_POSITIVE_TOUCH)
	{
		uint32_to_uint8(count_ticks, time_Value);
		bufete_build_message(bufete, time_Value);
		ad_check = 0;
		count_ticks = 0;
		bufete_change_state(bufete, BUFETE_STATE_ANSWER_SOFTEC);

	}

	if (bufete->enable_timeout == BUFETE_ENABLE_TIMEOUT){
		if (count_ticks > bufete->timeout )
		{
			bsp_turn_off_ADC_it(sensor_panel.sensors[sensor_panel.num_of_sensors - id_led_on].adc);
		    bsp_stop_time(&htim7);
			count_ticks = 0;
			bufete->message->message_event = OLD_MESSAGE;
			bufete->message->SOM = 0x7E;
			bufete->message->command = TIMEOUT_OPERATION;
			bufete->message->payload = &(bufete->message->command);
			bufete->message->EOM = 0x7E;
			bufete->message->size_payload = 1;
			bufete_change_state(bufete, BUFETE_STATE_ANSWER_SOFTEC);
		}
	}
}
void bufete_answer_softec(bufete_t *bufete)
{
	  bsp_send_data(&huart1, &(bufete->message->SOM), 1 , 1000);
	  bsp_send_data(&huart1, &(bufete->message->command), 1, 1000);
	  bsp_send_data(&huart1, bufete->message->payload, bufete->message->size_payload, 1000);
	  bsp_send_data(&huart1, &(bufete->message->EOM), 1, 1000);
	  bufete->message->message_event = OLD_MESSAGE;
	  bufete_change_state(bufete, BUFETE_STATE_IDLE);
}

void uint32_to_uint8(uint32_t Value, uint8_t* answer)
{

	*(answer)   = (Value & 0xff000000)  >> 24;
	*(answer+1) = (Value & 0x00ff0000) >> 16;
	*(answer+2) = (Value & 0x0000ff00) >> 8;
	*(answer+3) = (Value & 0x000000ff);

}

