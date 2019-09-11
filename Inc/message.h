/*
 * message.h
 *
 *  Created on: 20 de ago de 2019
 *      Author: Lesc
 */


/* Todos os pacotes começam e terminam com 7E(~)
 *
 * Se tiver um 7E no meio do payload -> Troca-se por um 7D5E
 *
 * Se existir um 7D no meio do payload -> Troca-se 7D5D
 *
 * COMANDOS:
 *
 * Autenticação:
 *
 *  BUFETE RECEBE 0x7E 0x00 0x7E
 *  BUFETE ENVIA  0x7E 0x00 0x00 0x7E
 *
 *  Ligar_Leds:
 *
 *  BUFETE RECEBE 0x7E 0x01 <SEQUENCIA DE 10 BYTES> 0x7E
 *
 *  A sequencia de bytes representa o painel de leds, onde cada byte representa o LED na sua respectiva posição.
 *  Ligado    = 0x69
 *  Desligado = 0x00
 *  BUFETE RETORNA 0x7E 0x01 <ID DO LED> < 4 Bytes de TEMPO> ... 0x7E
 *  TEMPO(MSB) = uint32_t
 *  Repete-se o formato em caso de mais de um LED
 *
 *
 *  Testar_Led:
 *
 *  BUFETE RECEBE 0x7E 0x02 <SEQUENCIA DE 10 BYTES> 0x7E
 *
 *  A sequencia de bytes representa o painel de leds, onde cada byte representa o LED na sua respectiva posição.]
 *  Ligado    = 0x69
 *  Desligado = 0x00
 *
 *  BUFETE RETORNA 0x7E 0x02 <ID DO LED> 0x7E
 *  Ligar_Leds_TDT:
 *
 *  BUFETE RECEBE 0x7E 0x03 <SEQUENCIA DE 10 BYTES> 0x7E
 *  A sequencia de bytes representa o painel de leds, onde cada byte representa o LED na sua respectiva posição.
 *  Ligado          = 0x69
 *  Desligado       = 0x00
 *  Led de Confusão = 0x99
 *
 *  BUFETE RETORNA 0x7E 0x03 <ID DO LED> < 4 Bytes de TEMPO> ... 0x7E
 *  TEMPO(MSB) = uint32_t
 *  Repete-se o formato em caso de mais de um LED */



#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "main.h"
#include "bsp.h"

#define AUTHENTIFICATION_MSG "~x~"
#define ERROR_MSG "~x~"

typedef enum {
	AUTHENTICATION  = 0x78U,
	TURN_ON_LEDS    = 0x01U,
	TEST_LEDS       = 0x02U,
	TURN_ON_TDT     = 0x03U,
}commands_t;

typedef enum EVENT_MESSAGE{
	NEW_MESSAGE,
	OLD_MESSAGE,
}message_event_t;


typedef enum {
	REGULAR_OPERATION   = 0x01,
	TESTE_OPERATION     = 0x02,
	TIMEOUT_OPERATION   = 0x50
}operations_t;

typedef struct MESSAGE{
	uint8_t SOM; // Start of message
	uint8_t command;
	uint8_t* payload;
	uint8_t EOM; // End of message (FIN)
	uint16_t size_payload;
	message_event_t message_event;
}msg_t;


uint16_t message_size(struct ringbuf *rb);
void encoder_message (msg_t* message);
void decoder_message (msg_t* message);
void receive_message (msg_t* message);

#endif /* MESSAGE_H_ */
