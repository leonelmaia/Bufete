/*
 * app.c
 *
 *  Created on: 23 de ago de 2019
 *      Author: Lesc
 */
#include "app.h"


uint8_t ack_Authentication[] ={0x00};
uint16_t adc_value;
msg_t message =
{
		.SOM          = 0x7E,
		.command      = AUTHENTICATION,
		.payload      = ack_Authentication,
		.EOM          = 0x7E,
		.size_payload = 1,
		.message_event = OLD_MESSAGE,
};


void app_message_event_handler(message_event_t event)
{
	message.message_event = NEW_MESSAGE;
}
