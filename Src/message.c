/*
 * message.c
 *
 *  Created on: 20 de ago de 2019
 *      Author: Lesc
 */

#include "message.h"


uint16_t message_size(struct ringbuf *rb){
	int index = 0;
	uint8_t teste;
	if (rb->rptr > rb->wptr){
		index = Size_Of_Ring_Buffer - rb->rptr;
		index = index + rb->wptr;
		return index + 1;
	}
	if (rb->rptr < rb->wptr){
	if(rb->buffer[index + rb->rptr] == START_BYTE){
		index ++;
		while( rb->buffer[index + rb->rptr]  != END_BYTE ){
			teste = rb->buffer[index + rb->rptr];
			index++;
		}
		return index + 1;
	}
	else return ERROR_BYTE;
	}
}


void receive_message (msg_t* message)
{
	uint8_t raw_message[Size_Of_Ring_Buffer];
	uint8_t size_msg = message_size(&command_Handle);
	ringbuf_read_buffer(&command_Handle, size_msg, raw_message);
	if(size_msg == 0x99){
		memset(&(command_Handle.buffer), 0xFF, 255);   // clear the buffer
		return size_msg;
	}
	else{
	message->SOM          = raw_message[0];
	message->command      = raw_message[1];
	message->payload      = &raw_message[2];
	message->EOM          = raw_message[size_msg - 1];
	message->size_payload = size_msg - 3;
	message->message_event = OLD_MESSAGE;
	}
}


void encoder_message (msg_t* message)
{
	int index = 0;
	int index_new_message = 0;
	uint8_t new_message_payload[message->size_payload];
	for (index = 0 ; index < message->size_payload ; index ++)
	{
		if(message->payload[index] == START_BYTE){
			new_message_payload[index_new_message] = ESCAPE_BYTE;
			index_new_message++;
			new_message_payload[index_new_message] = ADDITIONAL_BTYE_5E;
		}
		else{
		if(message->payload[index] == ESCAPE_BYTE){
			new_message_payload[index_new_message] = *(message->payload + index);
			index_new_message++;
			new_message_payload[index_new_message] = ADDITIONAL_BTYE_5D;
		}
		else{
			new_message_payload[index_new_message] = *(message->payload + index);
		}
		}
		index_new_message++;
	}

	message->size_payload = index_new_message;
	for (index = 0 ; index < message->size_payload ; index ++)
	{
		*(message->payload + index) =  new_message_payload[index];
	}
}

void decoder_message (msg_t* message)
{
	int index = 0;
	int index_new_message = 0;
	uint8_t new_message_payload[message->size_payload];
	for (index = 0 ; index < message->size_payload ; index ++)
	{
		if(message->payload[index+1] == ADDITIONAL_BTYE_5E && message->payload[index] == ESCAPE_BYTE){
			new_message_payload[index_new_message] = START_BYTE;
			index++;
		}
		else{
			if(message->payload[index+1] == ADDITIONAL_BTYE_5D && message->payload[index] == ESCAPE_BYTE){
				new_message_payload[index_new_message] = ESCAPE_BYTE;
				index++;
			}
			else{
				new_message_payload[index_new_message] = *(message->payload + index);
			}
		}
		index_new_message++;
	}

	message->size_payload = index_new_message;
	for (index = 0 ; index < message->size_payload ; index ++)
	{
		*(message->payload + index) =  new_message_payload[index];
	}

}

