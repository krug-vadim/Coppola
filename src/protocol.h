#ifndef PROTOCOL_H
#define PROTOCOL_H

/*typedef enum
{
	PROTOCOL_STATE_MAGIC1,
	PROTOCOL_STATE_MAGIC2,
	PROTOCOL_STATE_HEADER,
	PROTOCOL_STATE_DATA,
	PROTOCOL_STATE_CRC,
}
PROTOCOL_STATE_t;*/

typedef uint16_t PROTOCOL_MAGIC_t;
typedef uint16_t PROTOCOL_ID_t;
typedef uint8_t  PROTOCOL_VERSION_t;
typedef uint8_t  PROTOCOL_FLAGS_t;
typedef uint8_t  PROTOCOL_LENGTH_t;
typedef uint16_t PROTOCOL_CRC_t;

typedef struct __attribute__((packed))
{
	PROTOCOL_MAGIC_t   magic;
	PROTOCOL_ID_t      id;
	PROTOCOL_VERSION_t version;
	PROTOCOL_FLAGS_t   flags;
	uint8_t            reserved;
	PROTOCOL_LENGTH_t  size;
}
PROTOCOL_HEADER_t;

typedef struct __attribute__((packed))
{
	PROTOCOL_HEADER_t header;
	uint8_t           *data;
	PROTOCOL_CRC_t     crc;
}
PROTOCOL_PACKET_t;

#define PROTOCOL_MAGIC1 0x38
#define PROTOCOL_MAGIC2 0xAF

void PROTOCOL_init(void);
void PROTOCOL_parse(uint8_t data);

#endif