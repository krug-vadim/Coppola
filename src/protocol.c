#include "def.h"

#include "protocol.h"

#include "crc.h"

static PROTOCOL_STATE_t state;

void
PROTOCOL_init(void)
{
	state = PROTOCOL_STATE_MAGIC1;
}

void
PROTOCOL_parse(uint8_t data)
{
	switch (state)
	{
		case PROTOCOL_STATE_MAGIC1:
			if ( data == PROTOCOL_MAGIC1 )
				state = PROTOCOL_STATE_MAGIC2;
			break;

		case PROTOCOL_STATE_MAGIC2:
			if ( data == PROTOCOL_MAGIC2 )
				state = PROTOCOL_STATE_RECEIVE;
			else if ( data == PROTOCOL_MAGIC1 )
				; /* DO NOTHING */
			else
				state = PROTOCOL_STATE_MAGIC1;
			break;

		default:
			/* XXX: Invalid state */
			break;
	}
}