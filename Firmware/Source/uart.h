#ifndef UART_H
#define UART_H

/* Initializes UART with given baud rate. */
void uart_init(int desiredBaudRate);

/* Initializes TX for UART with given baud rate. */
void uart_initTx(int desiredBaudRate);

/* Initializes RX for UART with given baud rate. */
void uart_initRx(int desiredBaudRate);

/* Resets RX buffer. */
void uart_resetRx();

/* Returns 1 if there is data waiting. */
bit uart_isDataAvailable();

/* Returns single character read from UART. If there is no character it will wait for one. */
unsigned char uart_readByte();

/* Writes single byte to UART. */
void uart_writeByte(unsigned char value);

/* Writes number of bytes to buffer. */
void uart_writeBytes(unsigned char value[], short count);

#endif
