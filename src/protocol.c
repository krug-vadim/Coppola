#include "def.h"

#include "protocol.h"

#include "crc.h"

static PROTOCOL_STATE_t state;
static PROTOCOL_PACKET_t packet;

static uint8_t *data_ptr;
static uint8_t bytes_needed;
static CRC_t crc;

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
			{
				crc = CRC_init();
				crc = CRC_update(crc, data);
				data_ptr = (uint8_t)&packet;
				*data_ptr++ = data;

				state = PROTOCOL_STATE_MAGIC2;
			}
			break;

		case PROTOCOL_STATE_MAGIC2:
			if ( data == PROTOCOL_MAGIC2 )
			{
				bytes_needed = sizeof(PROTOCOL_HEADER_t) - sizeof(PROTOCOL_MAGIC_t);
				crc = CRC_update(crc, data);
				*data_ptr++ = data;
				state = PROTOCOL_STATE_HEADER;
			}
			else if ( data == PROTOCOL_MAGIC1 )
				; /* DO NOTHING */
			else
				state = PROTOCOL_STATE_MAGIC1;
			break;

		case PROTOCOL_STATE_HEADER:
			*data_ptr++ = data;
			crc = CRC_update(crc, data);

			bytes_needed--;
			if ( bytes_needed )
				break;

			bytes_needed = packet.header.size;
			state = PROTOCOL_STATE_DATA;
			break;

		case PROTOCOL_STATE_DATA:
			*data_ptr++ = data;

			bytes_needed--;
			if ( bytes_needed )
				break;

			bytes_needed = sizeof(CRC_t);
			data_ptr = (uint8_t)&packet.crc;
			state = PROTOCOL_STATE_CRC;
			break;

		case PROTOCOL_STATE_CRC:
			*data_ptr++ = data;

			bytes_needed--;
			if ( bytes_needed )
				break;

			break;

		default:
			/* XXX: Invalid state */
			break;
	}
}

void
PROTOCOL_parse_data(char *data)
{
	// to the rescue

}

void
PROTOCOL_process(void)
{
	if ( !QUEUE_has_data() )
		return;
	for ( QUEUE_has_data() )
		PROTOCOL_parse( UART_QUEUE_)
}


/*

	#double narkomany mode on
	PROTOCOL_set_rx_func(UART_rx);
	PROTOCOL_set_tx_func(UART_tx);

	wait_for_header_magic;
	read_header;
	[check_header_crc]
	read_data;
	check_crc;
	execute;
*/