#include "def.h"
#include "uart.h"

#include "queue.h"

#define UART_QUEUE_SIZE 64U

static QUEUE_t queue_tx;
static QUEUE_t queue_rx;

static uint8_t data_tx[UART_QUEUE_SIZE];
static uint8_t data_rx[UART_QUEUE_SIZE];

void
UART_hw_init(void)
{
	/* Place UCA0 in Reset to be configured */
	UCA0CTL1 = UCSWRST;

	/* Configure Pin Muxing P1.1 RXD and P1.2 TXD */
	P1SEL  = BIT1 + BIT2 ;
	P1SEL2 = BIT1 + BIT2;

	/* Configure */

	UCA0CTL0 = 0x00;                          /* UART, 8N1, LSB */
	UCA0CTL1 |= UCSSEL_2;                     /* SMCLK */
	UCA0BR0 = 104;                            /* 1MHz 9600 */
	UCA0BR1 = 0;                              /* 1MHz 9600 */
	UCA0MCTL = UCBRS0;                        /* Modulation UCBRSx = 1 */

	/* Take UCA0 out of reset */
	UCA0CTL1 &= ~UCSWRST;

	/*IE2 |= UCA0RXIE;
	IE2 |= UCA0TXIE;*/
}

void
UART_init(void)
{
	UART_hw_init();

	QUEUE_init(&queue_tx, data_tx, UART_QUEUE_SIZE);
	QUEUE_init(&queue_rx, data_rx, UART_QUEUE_SIZE);
}

BOOL_t
UART_write(uint8_t *src, SIZE_t cnt)
{
	return QUEUE_write(&queue_tx, src, cnt);
}

BOOL_t
UART_write_byte(uint8_t byte)
{
	return QUEUE_write_byte(&queue_tx, byte);
}

BOOL_t
UART_read(uint8_t *dst, SIZE_t cnt)
{
	return QUEUE_read(&queue_rx, dst, cnt);
}

BOOL_t
UART_read_byte(uint8_t *byte)
{
	return QUEUE_read_byte(&queue_rx, byte);
}

SIZE_t
UART_put(char *str)
{
	return QUEUE_put(&queue_tx, str);
}

BOOL_t
UART_tx(void)
{
	uint8_t byte;

	if ( queue_tx.used == 0 )
		return TRUE;
	else if ( QUEUE_read_byte(&queue_tx, &byte) )
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
	if ( (queue_rx.size - queue_rx.used) == 0 )
		return FALSE;
	else
		return ( QUEUE_write_byte(&queue_rx, UCA0RXBUF) );
}

void
UART_process(void)
{
	if ( UART_CAN_TX() )
		UART_tx();

	if ( UART_CAN_RX() )
		UART_rx();
}

void
UART_io_init(IO_INTERFACE_t *io)
{
	io->read = UART_read;
	io->write = UART_write;
	io->byte_read = UART_read_byte;
	io->byte_write = UART_write_byte;
	io->log = UART_put;
}