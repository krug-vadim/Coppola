#include "def.h"
#include "uart.h"

#include "queue.h"

static QUEUE_t queue;

static uint8_t counter;

void
UART_init(void)
{
	/* Place UCA0 in Reset to be configured */
	UCA0CTL1 = UCSWRST;

	/* Configure Pin Muxing P1.1 RXD and P1.2 TXD */
	P1SEL = BIT1 | BIT2 ;
	P1SEL2 = BIT1 | BIT2;

	/* Configure */

	UCA0CTL0 = 0x00;                          /* UART, 8N1, LSB */
	UCA0CTL1 |= UCSSEL_2;                     /* SMCLK */
	UCA0BR0 = 104;                            /* 1MHz 9600 */
	UCA0BR1 = 0;                              /* 1MHz 9600 */
	UCA0MCTL = UCBRS0;                        /* Modulation UCBRSx = 1 */

	/* Take UCA0 out of reset */
	UCA0CTL1 &= ~UCSWRST;

	counter = 0;
}

BOOL_t 
UART_write(uint8_t *src, SIZE_t cnt)
{
	return QUEUE_write(&queue, src, cnt);
}

BOOL_t
UART_read(uint8_t *dst, SIZE_t cnt)
{
	return QUEUE_read(&queue, dst, cnt);
}

SIZE_t
UART_put(char *str)
{
	return QUEUE_put(&queue, str);
}

BOOL_t
UART_tx(void)
{
	uint8_t byte;

	if ( queue.used == 0 ) 
		return TRUE;
	else if ( QUEUE_read_byte(&queue, &byte) )
	{
		UCA0TXBUF = byte;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL_t
UART_rx(void)
{
	if ( (queue.size - queue.used) == 0 )
		return FALSE;
	else
		return ( QUEUE_write_byte(&queue, UCA0RXBUF) );
}

void
UART_process(void)
{
	if ( UART_CAN_TX() )
		UART_tx();

	if ( UART_CAN_RX() )
		UART_rx();
}
