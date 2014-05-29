#ifndef DEF_H
#define DEF_H

#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FOREVER for(;;)

typedef enum
{
	FALSE = 0x00,
	TRUE  = 0xFF
}
BOOL_t;

#endif