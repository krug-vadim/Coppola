#ifndef DUMB_H
#define DUMB_H

BOOL_t DUMB_io_func(uint8_t *src, SIZE_t cnt);
BOOL_t DUMB_io_func_byte_read(uint8_t *byte);
BOOL_t DUMB_io_func_byte_write(uint8_t byte);
SIZE_t DUMB_log_func(char *msg);

#endif
