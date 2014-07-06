#ifndef UART_H
#define UART_H

/* should be in HAL */
#define UART_CAN_RX() ( IFG2 & UCA0RXIFG )
#define UART_CAN_TX() ( IFG2 & UCA0TXIFG )

void UART_init(void);
void UART_process(void);

BOOL_t UART_write(uint8_t *src, SIZE_t cnt);
BOOL_t UART_read(uint8_t *dst, SIZE_t cnt);
SIZE_t UART_put(char *str);

BOOL_t UART_write_byte(uint8_t byte);
BOOL_t UART_read_byte(uint8_t *byte);

void UART_io_init(IO_INTERFACE_t *io);

#endif