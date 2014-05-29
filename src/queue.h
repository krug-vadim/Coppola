#ifndef QUEUE_H
#define QUEUE_H

/* QUEUE_SIZE should be power of two */
#define QUEUE_SIZE 1024

typedef uint8_t QUEUE_SIZE_t;

typedef struct
{
	QUEUE_SIZE_t used;

	uint8_t *write_pos;
	uint8_t *read_pos;

	uint8_t *data;
}
QUEUE_t;

void QUEUE_init(void);

void QUEUE_write_byte(uint8_t byte);
uint8_t QUEUE_read_byte(void);

uint8_t* QUEUE_head(void);
void QUEUE_skip(QUEUE_SIZE_t n);

#endif