#ifndef DEF_H
#define DEF_H

#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FOREVER for(;;)

typedef uint16_t SIZE_t;

typedef enum
{
	FALSE = 0x00,
	TRUE  = 0xFF
}
BOOL_t;

/* haha! */
typedef BOOL_t (*IO_FUNC_ptr)(uint8_t *src, SIZE_t cnt);
typedef BOOL_t (*IO_FUNC_BYTE_READ_ptr)(uint8_t *byte);
typedef BOOL_t (*IO_FUNC_BYTE_WRITE_ptr)(uint8_t byte);
typedef SIZE_t (*LOG_FUNC_ptr)(char *msg);

#endif
