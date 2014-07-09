#include "def.h"
#include "queue.h"

void
QUEUE_init(QUEUE_t *queue, uint8_t *data, QUEUE_SIZE_t size)
{
	queue->size = size;
	queue->used = 0;
	queue->read_pos = 0;
	queue->write_pos = 0;
	queue->data = data;
}

BOOL_t
QUEUE_write(QUEUE_t *queue, uint8_t *src, QUEUE_SIZE_t cnt)
{
	if ( (queue->size - queue->used) < cnt )
		return FALSE;

	queue->used += cnt;

	for( ; cnt; cnt--)
	{
		queue->data[queue->write_pos] = *src;

		src++;
		queue->write_pos++;

		if ( queue->write_pos >= queue->size )
			queue->write_pos = 0;
	}

	return TRUE;
}

BOOL_t
QUEUE_read(QUEUE_t *queue, uint8_t *dst, QUEUE_SIZE_t cnt)
{
	if ( queue->used < cnt )
		return FALSE;

	queue->used -= cnt;

	for( ; cnt; cnt--)
	{
		*dst = queue->data[queue->read_pos];

		dst++;
		queue->read_pos++;

		if ( queue->read_pos >= queue->size )
			queue->read_pos = 0;
	}

	return TRUE;
}

QUEUE_SIZE_t
QUEUE_put(QUEUE_t *queue, char *str)
{
	QUEUE_SIZE_t cnt;

	cnt = 0;

	while ( queue->used < queue->size )
	{
		queue->data[queue->write_pos] = *str;

		str++;
		queue->used++;
		queue->write_pos++;

		if ( queue->write_pos >= queue->size )
			queue->write_pos = 0;

		if ( *str == '\0' )
			return cnt;
	}

	return cnt;
}

BOOL_t
QUEUE_write_byte(QUEUE_t *queue, uint8_t src)
{
	if ( queue->used >= queue->size )
		return FALSE;

	queue->used++;

	queue->data[queue->write_pos] = src;
	queue->write_pos++;

	if ( queue->write_pos >= queue->size )
		queue->write_pos = 0;

	return TRUE;
}

BOOL_t
QUEUE_read_byte(QUEUE_t *queue, uint8_t *dst)
{
	if ( queue->used == 0 )
		return FALSE;

	queue->used--;

	*dst = queue->data[queue->read_pos];
	queue->read_pos++;

	if ( queue->read_pos >= queue->size )
		queue->read_pos = 0;

	return TRUE;
}
