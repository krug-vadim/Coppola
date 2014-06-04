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

	FOREVER
	{
		UART_process();

		PROTOCOL_process();
	}

	return 0;
}