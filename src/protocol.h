#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef enum
{
	PROTOCOL_STATE_MAGIC1,
	PROTOCOL_STATE_MAGIC2,
}
PROTOCOL_STATE_t;

#define PROTOCOL_MAGIC1 0xDA
#define PROTOCOL_MAGIC2 0xAD

void PROTOCOL_init(void);
void PROTOCOL_parse(uint8_t data);

#endif // PROTOCOL_H