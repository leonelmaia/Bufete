/*
 * bufete_fsm.h
 *
 *  Created on: 16 de ago de 2019
 *      Author: Lesc
 */
#include "main.h"
#include "bsp.h"
#include "message.h"


#ifndef BUFETE_FSM_H_
#define BUFETE_FSM_H_

typedef enum {
	BUFETE_STATE_AUTHENTICATION,
	BUFETE_STATE_IDLE,
	BUFETE_STATE_TURN_ON_LEDS,
	BUFETE_STATE_TIME_COUNTING,
	BUFETE_STATE_ANSWER_SOFTEC,
	BUFETE_NUM_STATES
}bufete_states_t;

typedef enum BUFETE_TIMEOUT {
	BUFETE_ENABLE_TIMEOUT,
	BUFETE_DISABLE_TIMEOUT
}bufete_timeout_t;

typedef struct BUFETE_T{
	bufete_states_t state;
	bufete_timeout_t enable_timeout;
	uint32_t timeout;
	msg_t* message;
}bufete_t;


typedef void (*bufete_st_function_t) (bufete_t *bufete);

void bufete_init(bufete_t *poly, msg_t *message);
void bufete_iterate(bufete_t *poly);
void bufete_idle(bufete_t *poly);
void bufete_turn_on_leds(bufete_t *poly);
void bufete_authentication(bufete_t *poly);
void bufete_time_counting(bufete_t *poly);
void bufete_answer_softec(bufete_t *poly);
void bufete_build_message(bufete_t *bufete, uint8_t* time);
void bufete_check_id_led(uint16_t led_panel_value);


#endif /* BUFETE_FSM_H_ */
