#include "gd32f3x0.h"
#include <stdio.h>
#include "string.h"
#include "drv_ring_buff.h"
#include "send_command.h"
#include "gd32f3x0_usart.h"
#include "drv_receive_buff.h"
#include "stdbool.h"
#include "drv_uart.h"
uint8_t recevie_data_temp = 0xff;

extern  RING_BUF_DEF_STRUCT s_tx_ring_buf;
#define DATA_RX_BUFSIZE        256
#define CANCEL_CMD_LEN         (5)
static uint8_t s_link_rx_buf[DATA_RX_BUFSIZE];
extern  RING_BUF_DEF_STRUCT s_rx_ring_buf;
static uint8_t s_link_tx_buf[128];
void usart0_gpio_config(void)
{
    /* enable COM GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_6);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_7);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_6);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_7);
}

/*!
    \brief      cofigure the USART0
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usart0_config(void)
{
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

    usart_enable(USART0);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
	  
    return ch;
}

void USART0_IRQHandler(void)
{

    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
			{
          /* receive data */
          recevie_data_temp = usart_data_receive(USART0);
          drv_ringbuf_write((RING_BUF_DEF_STRUCT*)&s_rx_ring_buf, &recevie_data_temp, 1 );
       }

}











