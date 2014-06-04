#ifndef QUEUE_H
#define QUEUE_H

typedef SIZE_t QUEUE_SIZE_t;

typedef struct
{
	QUEUE_SIZE_t used;
	QUEUE_SIZE_t size;

	QUEUE_SIZE_t write_pos;
	QUEUE_SIZE_t read_pos;

	uint8_t *data;
}
QUEUE_t;

void QUEUE_init(QUEUE_t *queue, uint8_t *data, QUEUE_SIZE_t size);

BOOL_t QUEUE_write(QUEUE_t *queue, uint8_t *src, QUEUE_SIZE_t cnt);
BOOL_t QUEUE_read(QUEUE_t *queue, uint8_t *dst, QUEUE_SIZE_t cnt);

QUEUE_SIZE_t QUEUE_put(QUEUE_t *queue, char *str);

BOOL_t QUEUE_write_byte(QUEUE_t *queue, uint8_t src);
BOOL_t QUEUE_read_byte(QUEUE_t *queue, uint8_t *dst);

#endif