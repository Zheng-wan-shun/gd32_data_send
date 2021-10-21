#include "gd32f3x0.h"
#include <stdio.h>
#include "string.h"
#include "drv_ring_buff.h"
#include "send_command.h"
#include "gd32f3x0_usart.h"

#define DATA_TX_BUFSIZE      128
static RING_BUF_DEF_STRUCT s_tx_ring_buf;
 
static uint8_t s_link_tx_buf[DATA_TX_BUFSIZE];
void ring_buff_init(void)
{
  drv_ringbuf_init((RING_BUF_DEF_STRUCT*)&s_tx_ring_buf, s_link_tx_buf, DATA_TX_BUFSIZE);

}	

void ring_buff_receive(void)
{
	
	
	
	
	
	
}

void rinf_buff_transmit(uint8_t *str, uint8_t len)
{
	
	 drv_ringbuf_write(&s_tx_ring_buf, str, len);
   usart_interrupt_enable(USART0, USART_INT_TBE);

	
}

void set_data(void)
{
	
float p_value;
float i_value;
float d_value;
char receiver_buffer[32];
char* str=receiver_buffer;

	char * tmp;
	char * stri;
	char receiver_data[32];
	memcpy(receiver_data, receiver_buffer, strlen(receiver_buffer));
	stri = strtok_r(receiver_data," ",&tmp);
	while(stri != NULL)
	{
		stri = strtok_r(0," ",&tmp);
        if((stri[0]=='p')||(stri[0]=='P'))
	         {
		          p_value = atof(stri+1);
							printf("p_value:%f\t\n",p_value);
	         }
	       else if((stri[0]=='i')||(stri[0]=='I'))
	         {
		          i_value = atof(stri+1);
							printf("i_value:%f\t\n",i_value);
	          }
	       else if((stri[0]=='d')||(stri[0]=='D'))
	          {
	            d_value= atof(stri+1);
							printf("d_value:%f\t\n",d_value);
	           }
		}	
	
	
	
	
}












