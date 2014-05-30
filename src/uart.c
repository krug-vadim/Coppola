#include "def.h"
#include "uart.h"

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
UART_has_data(void)
{
	return ( IFG2 & UCA0RXIFG );
}

void
UART_process(void)
{
	if ( UART_has_data() )
		counter = UCA0RXBUF;

	if ( (IFG2 & UCA0TXIFG) )
		UCA0TXBUF = counter;
}
