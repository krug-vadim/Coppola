#include "def.h"
#include "protocol.h"
#include "crc.h"

typedef void* (*PROTOCOL_PARSER_VOID_ptr)(uint8_t data);
typedef PROTOCOL_PARSER_VOID_ptr (*PROTOCOL_PARSER_ptr)(uint8_t data);

/*typedef PROTOCOL_PARSER_t* (*PROTOCOL_PARSER_t)(uint8_t data);*/

static PROTOCOL_PARSER_ptr parser;
static PROTOCOL_PACKET_t packet;
/*static PROTOCOL_PACKET_t ack_packet;*/

static uint8_t *data_ptr;
static uint8_t bytes_needed;
static CRC_t crc;

PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_magic1(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_magic2(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_header(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_data(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_crc(uint8_t data);

void
PROTOCOL_init(void)
{
	parser = (PROTOCOL_PARSER_ptr)PROTOCOL_parse_magic1;
}

PROTOCOL_PARSER_VOID_ptr
PROTOCOL_parse_magic1(uint8_t data)
{
	if ( data != PROTOCOL_MAGIC1 )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic1;

	crc = CRC_init();
	crc = CRC_update(crc, data);
	data_ptr = (uint8_t *)&packet;
	*data_ptr++ = data;

	return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic2;
}

PROTOCOL_PARSER_VOID_ptr
PROTOCOL_parse_magic2(uint8_t data)
{
	if ( data == PROTOCOL_MAGIC2 )
	{
		bytes_needed = sizeof(PROTOCOL_HEADER_t) - sizeof(PROTOCOL_MAGIC_t);
		crc = CRC_update(crc, data);
		*data_ptr++ = data;
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_header;
	}
	else if ( data == PROTOCOL_MAGIC1 )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic2;
	else
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic1;
}

PROTOCOL_PARSER_VOID_ptr
PROTOCOL_parse_header(uint8_t data)
{
	*data_ptr++ = data;
	crc = CRC_update(crc, data);

	bytes_needed--;
	if ( bytes_needed )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_header;

	bytes_needed = packet.header.size;

	return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_data;
}

PROTOCOL_PARSER_VOID_ptr
PROTOCOL_parse_data(uint8_t data)
{
	*data_ptr++ = data;

	bytes_needed--;
	if ( bytes_needed )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_data;

	bytes_needed = sizeof(CRC_t);
	data_ptr = (uint8_t *)&packet.crc;

	return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_crc;
}

PROTOCOL_PARSER_VOID_ptr
PROTOCOL_parse_crc(uint8_t data)
{
	*data_ptr++ = data;

	bytes_needed--;
	if ( bytes_needed )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_crc;

	/* XXX: NEED TO DO SOMETHING */
	/*if ( parse_packet() )
		flags |=*/

	return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic1;
}

void
PROTOCOL_ack_send(uint8_t flags)
{
}

void
PROTOCOL_process(void)
{
	parser = (PROTOCOL_PARSER_ptr)(*parser)(0x00);
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