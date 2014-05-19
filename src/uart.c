#include "uart.h"

void
UART_init(void)
{
	P1SEL  = BIT1 + BIT2;	// P1.1 = RXD, P1.2 = TXD
	P1SEL2 = BIT1 + BIT2;	// P1.1 = RXD, P1.2 = TXD
	UCA0CTL1 |= UCSSEL_2;	// SMCLK
	UCA0BR0 = 104;			// see baud rate divider above
	UCA0BR1 = 0;
	UCA0MCTL = UCBRS0;		// modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;	// ** initialize USCI state machine **
	IE2 |= UCA0TXIE;		// Enable USCI_A0 TX interrupt
}

void
UART_proccess(void)
{

}

void
UART_has_data(void)
{
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
		while (!(IFG2 & UCA0TXIFG));	// USCI_A0 TX buffer ready?
		UCA0TXBUF = buffer[i++];
	}

	P1OUT &= ~0x01;
	IE2 &= ~UCA0RXIFG;
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
	P1OUT |= 0x01;
	unsigned int i = 0;					// iterator pointers
	sprintf(buffer, "temp: %d F\n\r", (int)(latest_adc_result/10));

	while (buffer[i] != '\0') {
		while (!(IFG2 & UCA0TXIFG));	// USCI_A0 TX buffer ready?
		UCA0TXBUF = buffer[i++];
	}

	P1OUT &= ~0x01;
	IE2 &= ~UCA0TXIFG;
}