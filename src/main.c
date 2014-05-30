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
init(void)
{
	/* Stop WDT */
	WDTCTL = WDTPW + WDTHOLD;

	/* Use Calibration values for 1MHz Clock DCO*/
	DCOCTL = 0;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	PROTOCOL_init();
	UART_init();
}

void
startup(void)
{
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