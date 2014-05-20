/*
THIS SOFTWARE IS A PRE-RELEASE VERSION FOR BETA USE
THAT IS NOT INTENDED FOR PRODUCTION APPLICATIONS.
THIS SOFTWARE HAS NOT BEEN TESTED
AND MAY CONTAIN IRREGULARITIES AND DEFECTS NOT FOUND IN PRODUCTION SOFTWARE.
*/

#include <msp430.h>

#include "def.h"

#include "uart.h"

void
init(void)
{
	PROTOCOL_init();
	UART_init();
}

void
startup(void)
{
}

void
main(void)
{
	init();

	startup();

	FOREVER // alone
	{
		UART_process();
		PROTOCOL_process();
	}

	return 0;
}