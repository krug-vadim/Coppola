#include "queue.h"

typedef struct
{
	QUEUE_SIZE_t used;
	char *data;
}
QUEUE_t;

static QUEUE_t uart_queue;
static uint8_t uart_data[QUEUE_SIZE];

void
QUEUE_init(void)
{
	uart_queue.used = 0;
	uart_queue.data = uart_data;
}

void
QUEUE_write_byte(uint8_t byte)
{
	uart_queue.used++;
	*uart_queue.write_pos = byte;
	uart_queue.write_pos++;
}

uint8_t
QUEUE_read_byte(void)
{
	uint8_t t;

	uart_queue.used--;
	t = uart_queue.read_pos;
	uart_queue.read_pos++;

	return t;
}

uint8_t
QUEUE_head(void)
{
	return uart_queue.data;
}

void
QUEUE_skip(QUEUE_SIZE_t n)
{
	uart_queue.used -= n;
	uart_queue.data += (n & QUEUE_SIZE_MASK);
}