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

typedef enum
{
	/* port one */
	P1_0 =  0,
	P1_1 =  1,
	P1_2 =  2,
	P1_3 =  3,
	P1_4 =  4,
	P1_5 =  5,
	P1_6 =  6,
	P1_7 =  7,

	/* port two */
	P2_0 =  8,
	P2_1 =  9,
	P2_2 = 10,
	P2_3 = 11,
	P2_4 = 12,
	P2_5 = 13,
	P2_6 = 14,
	P2_7 = 15
}
PIN_t;

#define PIN_COUNT 16

typedef enum
{
	PIN_STATE_LOW  = 0x00,
	PIN_STATE_HIGH = 0xFF
}
PIN_STATE_t;

typedef struct
{
	volatile       uint8_t *out;
	volatile       uint8_t *dir;
	volatile const uint8_t *in;
	volatile       uint8_t pin;
}
PIN_REGS_t;

#define PIN_MODE_INPUT(p)  (*(pins_hw[p].dir) |=  (1 << pins_hw[p].pin))
#define PIN_MODE_OUTPUT(p) (*(pins_hw[p].dir) &= ~(1 << pins_hw[p].pin))

#define PIN_SET_LOW(p)     (*(pins_hw[p].out) &= ~(1 << pins_hw[p].pin))
#define PIN_SET_HIGH(p)    (*(pins_hw[p].out) |=  (1 << pins_hw[p].pin))
#define PIN_TOGGLE(p)      (*(pins_hw[p].out) ^=  (1 << pins_hw[p].pin))
#define PIN_VALUE(p)       ((*(pins_hw[p].in)) & (1 << pins_hw[p].pin))

extern PIN_REGS_t pins_hw[PIN_COUNT];

/* haha! */
typedef BOOL_t (*IO_FUNC_ptr)(uint8_t *src, SIZE_t cnt);
typedef BOOL_t (*IO_FUNC_BYTE_READ_ptr)(uint8_t *byte);
typedef BOOL_t (*IO_FUNC_BYTE_WRITE_ptr)(uint8_t byte);
typedef SIZE_t (*LOG_FUNC_ptr)(char *msg);

/* crazy world */
typedef struct
{
	IO_FUNC_ptr read;
	IO_FUNC_ptr write;

	IO_FUNC_BYTE_READ_ptr  byte_read;
	IO_FUNC_BYTE_WRITE_ptr byte_write;

	LOG_FUNC_ptr log;
}
IO_INTERFACE_t;

#endif
