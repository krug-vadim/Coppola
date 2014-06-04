/*
THIS SOFTWARE IS A PRE-RELEASE VERSION FOR BETA USE
THAT IS NOT INTENDED FOR PRODUCTION APPLICATIONS.
THIS SOFTWARE HAS NOT BEEN TESTED
AND MAY CONTAIN IRREGULARITIES AND DEFECTS NOT FOUND IN PRODUCTION SOFTWARE.
*/

#include "def.h"

#include "uart.h"
#include "protocol.h"

void
hw_init(void)
{
	/* Stop WDT */
	WDTCTL = WDTPW + WDTHOLD;

	/* Use Calibration values for 1MHz Clock DCO*/
	DCOCTL = 0;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	/* SMCLK = DCO / DIVS = nMHz */
	BCSCTL2 = 0;
	/* ACLK = VLO = ~ 12 KHz */
	BCSCTL3 |= LFXT1S_2;

	/* */
	P1OUT = BIT0 + BIT6;
	P1DIR = BIT0 + BIT6;

	TA0CCR0 = 256UL;
	TA0CCTL0 |= CCIE;
	TA0CTL = TASSEL_1 /* Timer A clock source select: 1 - ACLK */
	       | ID_0     /* Timer A input divider: 0 - /1 */
	       | MC_1     /* Timer A mode control: 1 - Up to CCR0 */
	       ;
}

void
init(void)
{
	hw_init();

	/* modules init */
	UART_init();
	PROTOCOL_init();

	/* post setup */
	PROTOCOL_set_write_func(UART_write);
	PROTOCOL_set_read_func(UART_read);
	PROTOCOL_set_write_byte_func(UART_write_byte);
	PROTOCOL_set_read_byte_func(UART_read_byte);
}

void
startup(void)
{
	UART_put("HELLO\r\nwasher/coppola\r\n");
}

int
main(void)
{
	init();

	startup();

	__enable_interrupt();

	UCA0TXBUF = 'A';

	for (;;)
	{
		UART_process();
	}

	return 0;
}

/*int
main(void)
{
	init();

	startup();

	hw_init();

	__enable_interrupt();

	FOREVER
	{
		UART_process();

		PROTOCOL_process();
	}

	return 0;
}*/

/* TIMERA0 interrupt service routine */
void __attribute__((interrupt(TIMER0_A0_VECTOR))) Timer_A(void)
{
	P1OUT ^= (BIT6);
}