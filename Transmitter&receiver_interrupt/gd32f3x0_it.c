/*!
    \file    gd32f3x0_it.c
    \brief   interrupt service routines
    
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


#include "gd32f3x0_it.h"
#include <stdbool.h>
#include <string.h>
#include "stdlib.h"
//#include "gd32f350r_eval.h"

extern uint8_t transfersize;
extern uint8_t receivesize;
extern __IO uint8_t txcount; 
extern __IO uint16_t rxcount; 
char receiver_buffer[32];
char* str=receiver_buffer;

extern uint8_t transmitter_buffer[];
/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1);
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/

//int32_t recevie_data;
volatile bool recevie_done = false;
volatile bool recevie_start = true;
 uint8_t recevie_data_temp = 0xff;
void USART0_IRQHandler(void)
{

    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)){
        /* receive data */
        recevie_data_temp = usart_data_receive(USART0);
        if((recevie_data_temp == 'G') || (recevie_data_temp=='g'))
				{
					 rxcount = 0;
					 receiver_buffer[0]='G';
   				 recevie_done = false;
					 recevie_start = true;
				}
			 if(recevie_start == true)
				{
           if(rxcount > 32)
					 {
						  memset(receiver_buffer, 0, 32);
              rxcount = 0;
						  recevie_done=false;
					    recevie_start = false;
					 }
					 receiver_buffer[rxcount++] = recevie_data_temp;
					     
					 if((recevie_data_temp == '\n') || (recevie_data_temp=='\r'))
					 {
						  receiver_buffer[rxcount++] == '\0';
   				    recevie_done = true;
					    recevie_start = false;
					 }
				}

}

}

//void send_command(gcode command)
//{
//	switch (command):
//		case G0:
//			thing1;
//		break;
//		case G1:
//			thing2;
//		break;
//		case G2:
//			thing3;
//		break;
//		case G3:
//			thing4;
//		break;
//}
float p_value;
float i_value;
float d_value;
void set_pid(void)
{
	char * cmd;
	char * tmp;
	char * str;
	char receiver_data[32];
	memcpy(receiver_data, receiver_buffer, strlen(receiver_buffer));
	str = strtok_r(receiver_data," ",&tmp);
	while(str != NULL)
	{
		str = strtok_r(0," ",&tmp);
if((str[0]=='p')||(str[0]=='P'))
	           {
		            p_value = atof(str+1);
							 printf("p_value:%f\t\n",p_value);
	           }
	         else if((str[0]=='i')||(str[0]=='I'))
	            {
		            i_value = atof(str+1);
								printf("i_value:%f\t\n",i_value);
	            }
	         else if((str[0]=='d')||(str[0]=='D'))
	             {
	             	d_value= atof(str+1);
								 printf("d_value:%f\t\n",d_value);
	             }
	 

						 }
	
					 }

					 
					 