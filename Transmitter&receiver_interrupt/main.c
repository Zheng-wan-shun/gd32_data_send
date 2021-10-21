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


int main(void)
{
	
	 drv_uart_init();
	 ring_buff_init();
   while (1)
	{
		ringbuff_start();

		//receivebuff_start();

  }
}		



