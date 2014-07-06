#include "def.h"
#include "protocol.h"
#include "crc.h"

typedef void* (*PROTOCOL_PARSER_VOID_ptr)(uint8_t data);
typedef PROTOCOL_PARSER_VOID_ptr (*PROTOCOL_PARSER_ptr)(uint8_t data);

/*typedef PROTOCOL_PARSER_t* (*PROTOCOL_PARSER_t)(uint8_t data);*/

static PROTOCOL_PARSER_ptr parser;
static PROTOCOL_PACKET_t packet;
static PROTOCOL_PACKET_t answer;
static PROTOCOL_ACK_t ack;

static uint8_t *data_ptr;
static uint8_t bytes_needed;

static CRC_t crc;
static CRC_t ack_init_crc;
static CRC_t answer_init_crc;

static IO_INTERFACE_t protocol_io;

static IO_FUNC_ptr            data_parser;

PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_magic1(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_magic2(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_header(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_data(uint8_t data);
PROTOCOL_PARSER_VOID_ptr PROTOCOL_parse_crc(uint8_t data);

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
	char str[] = "0000 ";

	str[0] = digits_sym[((word & 0xF000) >> 12)];
	str[1] = digits_sym[((word & 0x0F00) >>  8)];
	str[2] = digits_sym[((word & 0x00F0) >>  4)];
	str[3] = digits_sym[((word & 0x000F) >>  0)];
	protocol_io.log(str);
}

void
PROTOCOL_init(void)
{
	SIZE_t i;

	answer.header.magic = PROTOCOL_MAGIC;
	answer.header.id = 0;
	answer.header.version = PROTOCOL_VERSION;
	answer.header.reserved = 0x00;

	answer_init_crc = CRC_init();
	for(i = 0; i < sizeof(PROTOCOL_HEADER_t)-1; i++)
		answer_init_crc = CRC_update(answer_init_crc, ((uint8_t *)&answer)[i]);

	ack.header.magic = PROTOCOL_MAGIC;
	ack.header.id = 0;
	ack.header.version = PROTOCOL_VERSION;
	ack.header.reserved = 0x00;
	ack.header.size = 0x01;
	ack.flags = 0x00;
	ack.crc = 0;

	ack_init_crc = CRC_init();
	for(i = 0; i < sizeof(PROTOCOL_HEADER_t); i++)
		ack_init_crc = CRC_update(ack_init_crc, ((uint8_t *)&ack)[i]);

	parser = (PROTOCOL_PARSER_ptr)PROTOCOL_parse_magic1;
}

void
PROTOCOL_ack_send(PROTOCOL_ACK_FLAGS_t flags)
{
	ack.flags = flags
	          | PROTOCOL_ACK_FLAG_OTHER;

	ack.crc = CRC_update(ack_init_crc, ack.flags);

	/* TODO: check write fail */
	protocol_io.write((uint8_t *)&ack, sizeof(PROTOCOL_ACK_t));
}

PROTOCOL_PARSER_VOID_ptr
PROTOCOL_parse_magic1(uint8_t data)
{
	if ( data != PROTOCOL_MAGIC1 )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic1;

	crc = CRC_init();
	PROTOCOL_debug_uint(crc);
	protocol_io.log("[i] init crc\r\n");

	crc = CRC_update(crc, data);

	data_ptr = (uint8_t *)&packet;
	*data_ptr++ = data;

	PROTOCOL_debug_uint(data);
	protocol_io.log("[i] found magic byte 1\r\n");
	PROTOCOL_debug_uint(crc);
	protocol_io.log("[i] crc\r\n");

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
		protocol_io.log("[i] found magic byte 2\r\n");
		PROTOCOL_debug_uint(crc);
		protocol_io.log("[i] crc\r\n");

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
	protocol_io.log("[i] reading header...\r\n");
	PROTOCOL_debug_uint(crc);
	protocol_io.log("[i] crc\r\n");

	bytes_needed--;
	if ( bytes_needed )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_header;

	protocol_io.log("[i] header readed\r\n");

	bytes_needed = packet.header.size;

	return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_data;
}

PROTOCOL_PARSER_VOID_ptr
PROTOCOL_parse_data(uint8_t data)
{
	*data_ptr++ = data;
	crc = CRC_update(crc, data);

	PROTOCOL_debug_uint(data);
	protocol_io.log("[i] reading data...\r\n");
	PROTOCOL_debug_uint(crc);
	protocol_io.log("[i] crc\r\n");

	bytes_needed--;
	if ( bytes_needed )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_data;

	protocol_io.log("[i] data readed\r\n");

	bytes_needed = sizeof(CRC_t);
	data_ptr = (uint8_t *)&packet.crc;

	return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_crc;
}

PROTOCOL_PARSER_VOID_ptr
PROTOCOL_parse_crc(uint8_t data)
{
	PROTOCOL_ACK_FLAGS_t flags;

	*data_ptr++ = data;

	bytes_needed--;
	if ( bytes_needed )
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_crc;
	flags = 0x00;

	if ( (crc == packet.crc) )
		flags |= PROTOCOL_ACK_FLAG_CRC_OK;
	else
		protocol_io.log("[e] wrong CRC\r\n");

	if ( packet.header.version != PROTOCOL_VERSION )
	{
		protocol_io.log("[e] wrong protocol version\r\n");
		return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic1;
	}

	if ( data_parser(packet.data, packet.header.size) )
		flags |= PROTOCOL_ACK_FLAG_OPERATION_OK;

	flags |= PROTOCOL_ACK_FLAG_OPERATION_DONE;

	PROTOCOL_ack_send(flags);

	protocol_io.log("[i] packet parsed\r\n");

	return (PROTOCOL_PARSER_VOID_ptr)PROTOCOL_parse_magic1;
}

void
PROTOCOL_process(void)
{
	uint8_t byte;

	if ( protocol_io.byte_read(&byte) )
		parser = (PROTOCOL_PARSER_ptr)(*parser)(byte);
}

void
PROTOCOL_set_write_func(IO_FUNC_ptr write_fn)
{
	protocol_io.write = write_fn;
}

void
PROTOCOL_set_read_func(IO_FUNC_ptr read_fn)
{
	protocol_io.read = read_fn;
}

void
PROTOCOL_set_write_byte_func(IO_FUNC_BYTE_WRITE_ptr write_byte_fn)
{
	protocol_io.byte_write = write_byte_fn;
}

void
PROTOCOL_set_read_byte_func(IO_FUNC_BYTE_READ_ptr read_byte_fn)
{
	protocol_io.byte_read = read_byte_fn;
}

void
PROTOCOL_set_log_func(LOG_FUNC_ptr log_fn)
{
	protocol_io.log = log_fn;
}

void
PROTOCOL_set_parser_func(IO_FUNC_ptr parser_fn)
{
	data_parser = parser_fn;
}

void
PROTOCOL_io_set(IO_INTERFACE_t *io)
{
	protocol_io = *io;
}

BOOL_t
PROTOCOL_write(uint8_t *src, SIZE_t cnt)
{
	BOOL_t res;

	answer.header.size = cnt;
	answer.crc = CRC_update(answer_init_crc, answer.header.size);

	res = TRUE;
	res = res && protocol_io.write((uint8_t *)&answer.header, sizeof(PROTOCOL_HEADER_t));

	for(;cnt != 0; cnt--, src++)
	{
		answer.crc = CRC_update(answer.crc, *src);
		res = res && protocol_io.byte_write(*src);
	}

	res = res && protocol_io.write((uint8_t *)&answer.crc, sizeof(CRC_t));

	return res;
}