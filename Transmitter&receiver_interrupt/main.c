/*!
    \file    main.c
    \brief   USART transmit and receive interrupt
    
    \version 2017-06-06, V1.0.0, firmware for GD32F3x0
    \version 2019-06-01, V2.0.0, firmware for GD32F3x0
    \version 2020-09-30, V2.1.0, firmware for GD32F3x0
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f3x0.h"
#include "send_command.h"
#include "drv_ring_buff.h"
#include "drv_receive_buff.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "stdlib.h"
#include "drv_uart.h"
#include "gd32f3x0_usart.h"
#include "stdint.h"
//#include "gd32f350r_eval.h"



void usart0_gpio_config(void);
void usart0_config(void);


uint8_t transmitter_buffer[] = "\n\rUSART interrupt test\n\r";
volatile bool recevie_data = false;

uint8_t receivesize = 32;
__IO uint8_t txcount = 0; 
__IO uint16_t rxcount = 0; 
 RING_BUF_DEF_STRUCT s_rx_ring_buf;
extern volatile bool recevie_done;
extern uint8_t recevie_data_temp;
//char receive_buffer[32];

//static uint8_t s_link_tx_buf[128];
static uint8_t s_link_rx_buf[128];

static uint8_t buffer_count;
static bool received_start;
static bool received_done;
char receive_buffer[32];
uint8_t temp_buffer[32];
int main(void)
{
   nvic_irq_enable(USART0_IRQn, 0, 0); 
   usart0_gpio_config();
   usart0_config();
   usart_interrupt_enable(USART0, USART_INT_RBNE);
   drv_ringbuf_init((RING_BUF_DEF_STRUCT*)&s_rx_ring_buf, s_link_rx_buf, 128);
   while (1)
	{
		static uint8_t count;
		count = drv_ringbuf_count((RING_BUF_DEF_STRUCT*)&s_rx_ring_buf);
		if(count != 0)
		{
		  drv_ringbuf_read((RING_BUF_DEF_STRUCT*)&s_rx_ring_buf, 1, temp_buffer);	  
			if((temp_buffer[0] == 'g') || (temp_buffer[0]=='G'))
			{	
			//	printf("received_start\r\n");
				received_start = true;
			}
			if(	received_start)
			{
				 receive_buffer[buffer_count++] = temp_buffer[0];
				   if(buffer_count>32)
			    {
             memset(receive_buffer, 0, 32);
			       received_done = false;
			       received_start = false;
			       buffer_count = 0;
			     }
			    if((temp_buffer[0] == '\n') || (temp_buffer[0]=='\r'))
		     {
//					    printf("received done\r\n");
			        receive_buffer[buffer_count++] = '\0';
				     	received_start = false;
				      received_done = true;
	        }
			}			  
		}
	
		if(received_done)
		{
		   set_pid();
			 if(strlen(receive_buffer) != 0)
			 {
         printf("str:%s\r\n", receive_buffer);
			 }
//			   if(buffer_count > 0)
//				 {
//			       printf("buffer_count:%d\r\n", buffer_count);
//				 }                                               //定位错误的方法，用printf函数去一步一步定位
				 memset(receive_buffer, 0, 32);
				 received_done = false;
				 received_start = false;
				 buffer_count = 0;
		}

  }
}		



