#include "gd32f3x0.h"
#include <stdio.h>
#include "string.h"
#include "drv_ring_buff.h"
#include "send_command.h"
#include "gd32f3x0_usart.h"
#include "drv_uart.h"
#include "stdbool.h"
#include "drv_receive_buff.h"
RING_BUF_DEF_STRUCT s_rx_ring_buf;
extern uint8_t recevie_data_temp ;
#define DATA_TX_BUFSIZE      128
static RING_BUF_DEF_STRUCT s_tx_ring_buf;
//extern uint8_t s_link_rx_buf[128];
 uint8_t s_link_rx_buf[128];
static uint8_t s_link_tx_buf[DATA_TX_BUFSIZE];
uint8_t temp_buffer[32];
static uint8_t buffer_count;
static bool received_start;
static bool received_done;

char receive_buffer[32];
/*
  使用ringbuff,现在drv_uart.c中的void USART0_IRQHandler(void)打开 ringbuff_wirte();
	然后main函数里初始化 
	 uart_init();
	 ring_buff_init();
	 while里面写	ringbuff_start();即可

*/
void ring_buff_init(void)
{
  drv_ringbuf_init((RING_BUF_DEF_STRUCT*)&s_rx_ring_buf, s_link_rx_buf, DATA_TX_BUFSIZE);

}	
void ringbuff_wirte(void)
{
	
	 drv_ringbuf_write((RING_BUF_DEF_STRUCT*)&s_rx_ring_buf, &recevie_data_temp, 1 );
	
}

void ring_buff_receive_start(void)
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
	
	
}
void ringbuff_set_pid(void)
{
float p_value;
float i_value;
float d_value;
	char * tmp;
	char * str;
	char receiver_data[32];
	//memcpy(receiver_data, receiver_buffer, strlen(receiver_buffer));
	memcpy(receiver_data, receive_buffer, strlen(receive_buffer));
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


void ring_buff_done(void)
{
		if(received_done)
		{
		   ringbuff_set_pid();
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

void ringbuff_start(void)
{
	ring_buff_receive_start();
	ring_buff_done();
	
}





