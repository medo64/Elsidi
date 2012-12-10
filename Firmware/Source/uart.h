//2012-12-09: Initial version.

#ifndef _UART_H
#define _UART_H

/* Initializes UART with given baud rate. */
void uart_init(int desiredBaudRate) {
    SPBRG   = (unsigned char)(_XTAL_FREQ / desiredBaudRate / 64 - 1);
    BRG16   = 0;    //8-bit
    BRGH    = 1;    //high speed
    SYNC    = 0;    //asynchronous mode
    SPEN    = 1;    //serial port enabled
    TXEN    = 1;
    CREN    = 1;
    asm("CLRWDT");
}

/* Initializes TX for UART with given baud rate. */
void uart_initTx(int desiredBaudRate) {
    SPBRG   = (unsigned char)(_XTAL_FREQ / desiredBaudRate / 64 - 1);
    BRG16   = 0;    //8-bit
    BRGH    = 1;    //high speed
    SYNC    = 0;    //asynchronous mode
    SPEN    = 1;    //serial port enabled
    TXEN    = 1;
    asm("CLRWDT");
}

/* Initializes RX for UART with given baud rate. */
void uart_initRx(int desiredBaudRate) {
    SPBRG   = (unsigned char)(_XTAL_FREQ / desiredBaudRate / 64 - 1);
    BRG16   = 0;    //8-bit
    BRGH    = 1;    //high speed
    SYNC    = 0;    //asynchronous mode
    SPEN    = 1;    //serial port enabled
    CREN    = 1;
    asm("CLRWDT");
}

/* Resets RX buffer. */
void uart_resetRx() {
    CREN = 0; //disable continous receive, also clears errors
    CREN = 1; //enable continous receive
    unsigned char dummyRead;
    dummyRead = RCREG;
    dummyRead = RCREG;
    dummyRead = RCREG; //read data to clear FIFO
    SPEN = 0; //disable USART.
    SPEN = 1; //enable USART.
    asm("CLRWDT");
}

/* Returns 1 if there is data waiting. */
bit uart_isDataAvailable() {
    asm("CLRWDT");
    return RCIF;
}

/* Returns single character read from UART. If there is no character it will wait for one. */
unsigned char uart_readByte() {
    if (FERR) { uart_resetRx(); } //framing error
    if (OERR) { uart_resetRx(); } //overrun error
    while (RCIF == 0) { //wait until something is received
        asm("CLRWDT");
    }
    return RCREG;
}

/* Writes single byte to UART. */
void uart_writeByte(unsigned char value) {
    while (TXIF == 0) { //wait until buffer is empty
        asm("CLRWDT");
    }
    TXREG = value;
}

/* Writes number of bytes to buffer. */
void uart_writeBytes(unsigned char value[], short count) {
    for (short i=0; i<count; i++) {
        while (TXIF == 0) { //wait until buffer is empty
            asm("CLRWDT");
        }
        TXREG = value[i];
    }
}

#endif
