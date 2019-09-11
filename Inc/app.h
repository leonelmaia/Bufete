/*
 * app.h
 *
 *  Created on: 23 de ago de 2019
 *      Author: Lesc
 */

#include "bsp.h"
#include "bufete_fsm.h"

extern msg_t message;

void app_message_event_handler(message_event_t event);
