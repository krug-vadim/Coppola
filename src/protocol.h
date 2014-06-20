#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef uint16_t PROTOCOL_MAGIC_t;
typedef uint16_t PROTOCOL_ID_t;
typedef uint8_t  PROTOCOL_VERSION_t;
typedef uint8_t  PROTOCOL_LENGTH_t;
typedef uint16_t PROTOCOL_CRC_t;

typedef struct __attribute__((packed))
{
	PROTOCOL_MAGIC_t   magic;
	PROTOCOL_ID_t      id;
	PROTOCOL_VERSION_t version;
	uint8_t            reserved;
	PROTOCOL_LENGTH_t  size;
}
PROTOCOL_HEADER_t;

typedef struct __attribute__((packed))
{
	PROTOCOL_HEADER_t header;
	uint8_t           data[32];
	PROTOCOL_CRC_t    crc;
}
PROTOCOL_PACKET_t;

typedef enum
{
	PROTOCOL_ACK_FLAG_CRC_OK         = 0x80,
	PROTOCOL_ACK_FLAG_OPERATION_OK   = 0x40,
	PROTOCOL_ACK_FLAG_OPERATION_DONE = 0x20,
	PROTOCOL_ACK_FLAG_OTHER          = 0x1F
}
PROTOCOL_ACK_FLAG_t;

typedef uint8_t PROTOCOL_ACK_FLAGS_t;

typedef struct __attribute__((packed))
{
	PROTOCOL_HEADER_t    header;
	PROTOCOL_ACK_FLAGS_t flags;
	PROTOCOL_CRC_t       crc;
}
PROTOCOL_ACK_t;

#define PROTOCOL_MAGIC1 0x38
#define PROTOCOL_MAGIC2 0xAF

#define PROTOCOL_MAGIC 0xAF38U

#define PROTOCOL_VERSION 0x00

void PROTOCOL_init(void);
void PROTOCOL_process(void);

void PROTOCOL_set_write_func(IO_FUNC_ptr write_fn);
void PROTOCOL_set_read_func(IO_FUNC_ptr read_fn);

void PROTOCOL_set_write_byte_func(IO_FUNC_BYTE_WRITE_ptr write_byte_fn);
void PROTOCOL_set_read_byte_func(IO_FUNC_BYTE_READ_ptr read_byte_fn);

void PROTOCOL_set_log_func(LOG_FUNC_ptr log_fn);

void PROTOCOL_set_parser_func(IO_FUNC_ptr parser_fn);

#endif
