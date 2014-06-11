#include "def.h"
#include "crc.h"

CRC_t
CRC_init(void)
{
	return 0xFFFF;
}

CRC_t
CRC_update(CRC_t crc, uint8_t data)
{
	CRC_t t;

	data ^= crc & 0xFF;
	data ^= data << 4;

	t  = ((uint16_t)data << 8) | ((crc >> 8) & 0xFF);
	t ^= (uint8_t)(data >> 4);
	t ^= ((uint16_t)data << 3);

	return t;
}
