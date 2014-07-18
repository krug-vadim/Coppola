/*
THIS SOFTWARE IS A PRE-RELEASE VERSION FOR BETA USE
THAT IS NOT INTENDED FOR PRODUCTION APPLICATIONS.
THIS SOFTWARE HAS NOT BEEN TESTED
AND MAY CONTAIN IRREGULARITIES AND DEFECTS NOT FOUND IN PRODUCTION SOFTWARE.
*/

#include "def.h"

#include "uart.h"
#include "protocol.h"

#include "dumb.h"
#include "washer.h"

void
hw_init(void)
{
	/* Stop WDT */
	WDTCTL = WDTPW + WDTHOLD;

	/* XXX: CHANGE TO ERROR */
	if (CALBC1_1MHZ == 0xFF || CALDCO_1MHZ == 0xFF)
	{
		while(TRUE);
	}

	/* Use Calibration values for 1MHz Clock DCO*/
	DCOCTL = 0;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	/* SMCLK = DCO / DIVS = nMHz */
	BCSCTL2 = 0;
	/* ACLK = VLO = ~ 12 KHz */
	BCSCTL3 |= LFXT1S_2;

	/* setting XIN and XOUT as digital */
	P2SEL  &= ~(BIT6 | BIT7);
	P2SEL2 &= ~(BIT6 | BIT7);

	P2DIR |= (1 << 3);

	/* Timer A0 ACLK/1, UP 1 second */
	TA0CCR0 = 12000;
	TA0CCTL0 = 0x10;
	TA0CTL = TASSEL_1
	       | ID_0
	       | MC_1
	       ;
}

void
init(void)
{
	IO_INTERFACE_t uart_io;
	IO_INTERFACE_t protocol_io;

	hw_init();

	/* modules init */
	UART_init();
	PROTOCOL_init();
	WASHER_init();

	/* post setup */

	UART_io_init(&uart_io);
	PROTOCOL_io_set(&uart_io);

	protocol_io = uart_io;
	protocol_io.write = PROTOCOL_write;

	WASHER_io_set(&protocol_io);

	PROTOCOL_set_log_func(DUMB_log_func);
	/*PROTOCOL_set_log_func(UART_put);*/

	PROTOCOL_set_parser_func(WASHER_parse);
}

void
startup(void)
{
	WASHER_startup();

	UART_put("\r\n\r\n\r\nYOBA WASHER LTD.\r\nCoppola\r\n");
	UART_put("version: "GITSHA"\r\n");
	UART_put("  build: "BUILD_DATETIME"\r\n");
}

int
main(void)
{
	init();

	startup();

	__enable_interrupt();

	for (;;)
	{
		UART_process();
		PROTOCOL_process();
		WASHER_process();
	}

	return 0;
}

/* TIMERA0 interrupt service routine */
void __attribute__((interrupt(TIMER0_A0_VECTOR))) one_second(void)
{
	WASHER_one_second_tick();
}
