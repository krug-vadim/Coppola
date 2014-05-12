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

	}

	return 0;
}