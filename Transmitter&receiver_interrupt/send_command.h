#ifndef __SEND_COMMAND_H__
#define __SEND_COMMAND_H__
#include <stdlib.h>
#include "string.h"
#include "stdint.h"
#include "stdio.h"



void ring_buff_init(void);
void ring_buff_receive(void);
void rinf_buff_transmit(uint8_t *str, uint8_t len);
void ringbuff_data_receive(void);
void set_data(void);








#endif



