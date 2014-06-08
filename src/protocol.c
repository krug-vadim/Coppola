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

static IO_FUNC_ptr            write;
static IO_FUNC_ptr            read;
static IO_FUNC_BYTE_WRITE_ptr write_byte;
static IO_FUNC_BYTE_READ_ptr  read_byte;
static LOG_FUNC_ptr           log;

PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_magic1(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_magic2(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_header(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_data(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_crc(uint8_t data);

void
PROTOCOL_init(void)
{
	parser = (PROTOCOL_PARSER_ptr)PROTOCOL_parse_magic1;

	/* TODO: fn pointers to dumb fn */
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

	log("[i] found magic byte 1\r\n");

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

		log("[i] found magic byte 2\r\n");

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

	log("[i] reading header...\r\n");

	bytes_needed--;
	if ( bytes_needed )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_header;

	log("[i] header readed\r\n");

	bytes_needed = packet.header.size;

	return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_data;
}

PROTOCOL_PARSER_VOID_ptr
PROTOCOL_parse_data(uint8_t data)
{
	*data_ptr++ = data;
	crc = CRC_update(crc, data);

	log("[i] reading data...\r\n");

	bytes_needed--;
	if ( bytes_needed )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_data;

	log("[i] data readed\r\n");

	bytes_needed = sizeof(CRC_t);
	data_ptr = (uint8_t *)&packet.crc;

	return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_crc;
}

PROTOCOL_PARSER_VOID_ptr
PROTOCOL_parse_crc(uint8_t data)
{
	BOOL_t execute_result;

	*data_ptr++ = data;

	log("[i] reading crc...\r\n");

	bytes_needed--;
	if ( bytes_needed )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_crc;

	if ( (packet.flags & PROTOCOL_FLAG_DCL) && (crc != packet.crc) )
	{
		log("[e] wrong CRC");
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic1;
	}

	/*execute_result = parse||;*/

	if ( (packet.flags & PROTOCOL_FLAG_ACK) )
		PROTOCOL_ack_send();

	log("[i] packet parsed");

	return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic1;
}

void
PROTOCOL_process(void)
{
	uint8_t byte;

	if ( read_byte(&byte) )
		parser = (PROTOCOL_PARSER_ptr)(*parser)(byte);
}

void
PROTOCOL_set_write_func(IO_FUNC_ptr write_fn)
{
	write = write_fn;
}

void
PROTOCOL_set_read_func(IO_FUNC_ptr read_fn)
{
	read = read_fn;
}

void
PROTOCOL_set_write_byte_func(IO_FUNC_BYTE_WRITE_ptr write_byte_fn)
{
	write_byte = write_byte_fn;
}

void
PROTOCOL_set_read_byte_func(IO_FUNC_BYTE_READ_ptr read_byte_fn)
{
	read_byte = read_byte_fn;
}

void
PROTOCOL_set_log_func(LOG_FUNC_ptr log_fn)
{
	log = log_fn;
}
