#ifndef CRC_H
#define CRC_H

typedef uint16_t CRC_t;

CRC_t CRC_init(void);
CRC_t CRC_update(CRC_t crc, uint8_t data);

#endif // CRC_H