#include "crc.h"

uint16_t
crc_ccitt_update(uint16_t crc, uint8_t data)
{
	uint16_t t;

	data ^= crc & 255;
	data ^= data << 4;

	t  = ((uint8_t)data << 8) | ((crc >> 8) & 255);
	t ^= (uint8_t)(data >> 4);
	t ^= ((uint16_t)data << 3);

	return t;
}