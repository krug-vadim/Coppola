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

	/* */
	P1OUT = BIT0 + BIT6;
	P1DIR = BIT0 + BIT6;
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
	PROTOCOL_set_log_func(DUMB_log_func);
	/*PROTOCOL_set_log_func(UART_put);*/
	PROTOCOL_set_parser_func(WASHER_parse);

}

void
startup(void)
{
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
	}

	return 0;
}

/* TIMERA0 interrupt service routine */
void __attribute__((interrupt(TIMER0_A0_VECTOR))) Timer_A(void)
{
	P1OUT ^= (BIT6);
}
