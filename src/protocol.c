#include "def.h"
#include "protocol.h"
#include "crc.h"

typedef void* (*PROTOCOL_PARSER_VOID_ptr)(uint8_t data);
typedef PROTOCOL_PARSER_VOID_ptr (*PROTOCOL_PARSER_ptr)(uint8_t data);

/*typedef PROTOCOL_PARSER_t* (*PROTOCOL_PARSER_t)(uint8_t data);*/

static PROTOCOL_PARSER_ptr parser;
static PROTOCOL_PACKET_t packet;
static PROTOCOL_PACKET_t ack;

static uint8_t *data_ptr;
static uint8_t bytes_needed;
static CRC_t crc;

static IO_FUNC_ptr            write;
static IO_FUNC_ptr            read;
static IO_FUNC_BYTE_WRITE_ptr write_byte;
static IO_FUNC_BYTE_READ_ptr  read_byte;
static LOG_FUNC_ptr           log;

static IO_FUNC_ptr            parse_packet_data;

PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_magic1(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_magic2(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_header(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_data(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_crc(uint8_t data);

void
PROTOCOL_init(void)
{
	ack.header.magic = PROTOCOL_MAGIC;
	ack.header.id = 0;
	ack.header.version = PROTOCOL_VERSION;
	ack.header.reserved = 0x00;
	ack.header.size = 0x01;

	parser = (PROTOCOL_PARSER_ptr)PROTOCOL_parse_magic1;

	/* TODO: fn pointers to dumb fn */
}

void
PROTOCOL_ack_send()
{

}

static const char digits_sym[] =
{
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'A', 'B',
	'C', 'D', 'E', 'F'
};

void
PROTOCOL_debug_uint(uint16_t word)
{
	char str[] = ">> 0x0000 | ";

	str[5] = digits_sym[((word & 0xF000) >> 12)];
	str[6] = digits_sym[((word & 0x0F00) >>  8)];
	str[7] = digits_sym[((word & 0x00F0) >>  4)];
	str[8] = digits_sym[((word & 0x000F) >>  0)];

	log(str);
}

PROTOCOL_PARSER_VOID_ptr
PROTOCOL_parse_magic1(uint8_t data)
{
	if ( data != PROTOCOL_MAGIC1 )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic1;

	crc = CRC_init();
	PROTOCOL_debug_uint(crc);
	log("[i] init crc\r\n");

	crc = CRC_update(crc, data);

	data_ptr = (uint8_t *)&packet;
	*data_ptr++ = data;

	PROTOCOL_debug_uint(data);
	log("[i] found magic byte 1\r\n");
	PROTOCOL_debug_uint(crc);
	log("[i] crc\r\n");

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

		PROTOCOL_debug_uint(data);
		log("[i] found magic byte 2\r\n");
		PROTOCOL_debug_uint(crc);
		log("[i] crc\r\n");

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

	PROTOCOL_debug_uint(data);
	log("[i] reading header...\r\n");
	PROTOCOL_debug_uint(crc);
	log("[i] crc\r\n");

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

	PROTOCOL_debug_uint(data);
	log("[i] reading data...\r\n");
	PROTOCOL_debug_uint(crc);
	log("[i] crc\r\n");

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
	*data_ptr++ = data;

	PROTOCOL_debug_uint(data);
	log("[i] reading crc...\r\n");
	PROTOCOL_debug_uint(crc);
	log("[i] crc\r\n");

	bytes_needed--;
	if ( bytes_needed )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_crc;

	log("[i] packed readed\r\n");

	PROTOCOL_debug_uint(packet.crc);
	log("[i] packet.crc\r\n");
	PROTOCOL_debug_uint(crc);
	log("[i] calculated crc\r\n");

	if ( (crc != packet.crc) )
	{
		log("[e] wrong CRC\r\n");
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic1;
	}

	if ( packet.header.version != PROTOCOL_VERSION )
	{
		log("[e] wrong protocol version\r\n");
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic1;
	}

	if ( parse_packet_data(packet.data, packet.header.size) )
		ack.data[0] = 0xFF;
	else
		ack.data[0] = 0x00;

	PROTOCOL_ack_send();

	log("[i] packet parsed\r\n");

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
