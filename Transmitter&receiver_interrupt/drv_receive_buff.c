#include "gd32f3x0.h"
#include <stdio.h>
#include "string.h"
#include "drv_ring_buff.h"
#include "send_command.h"
#include "gd32f3x0_usart.h"
#include "drv_receive_buff.h"
#include "stdbool.h"
extern uint8_t transfersize;
extern uint8_t receivesize;
extern __IO uint8_t txcount; 
extern __IO uint16_t rxcount; 
char receiver_buffer[32];
char* str=receiver_buffer;

float p_value;
float i_value;
float d_value;

volatile bool recevie_done = false;
volatile bool recevie_start = true;
extern  uint8_t recevie_data_temp ;
 
void receivebuff_data_receive(void)
{
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
			  receiver_buffer[rxcount++] = '\0';
   		  recevie_done = true;
				recevie_start = false;
		 }
		}
	
	
	
}

void set_pid(void)
{

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


void receive_done(void)
{
	
	 if(recevie_done == true)
			{
				  set_pid();
 					uint8_t len = strlen(receiver_buffer);
			    printf("recevie_data %d:", rxcount);
          printf("len: %d\t\n", len);
					for(uint8_t i=0; i< len; i++)
					   {
							  printf("%c", receiver_buffer[i]);
						 }
					
					printf("\r\n");			
					memset(receiver_buffer, 0, 32);
					recevie_done = false;
					rxcount = 0;
			}
	
	
	
}




		














