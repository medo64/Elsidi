#include "uart.h"
#include "pic.h"
#include "config.h"

void uart_init(int desiredBaudRate) {
    SPBRG   = (unsigned char)(_XTAL_FREQ / desiredBaudRate / 16 - 1);
    BRG16   = 0;    //8-bit
    BRGH    = 1;    //high speed
    SYNC    = 0;    //asynchronous mode
    SPEN    = 1;    //serial port enabled
    TXEN    = 1;
    CREN    = 1;
    asm("CLRWDT");
}

void uart_initTx(int desiredBaudRate) {
    SPBRG   = (unsigned char)(_XTAL_FREQ / desiredBaudRate / 16 - 1);
    BRG16   = 0;    //8-bit
    BRGH    = 1;    //high speed
    SYNC    = 0;    //asynchronous mode
    SPEN    = 1;    //serial port enabled
    TXEN    = 1;
    asm("CLRWDT");
}

void uart_initRx(int desiredBaudRate) {
    SPBRG   = (unsigned char)(_XTAL_FREQ / desiredBaudRate / 16 - 1);
    BRG16   = 0;    //8-bit
    BRGH    = 1;    //high speed
    SYNC    = 0;    //asynchronous mode
    SPEN    = 1;    //serial port enabled
    CREN    = 1;
    asm("CLRWDT");
}

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

bit uart_isDataAvailable() {
    asm("CLRWDT");
    return RCIF;
}

unsigned char uart_readByte() {
    if (FERR) { uart_resetRx(); } //framing error
    if (OERR) { uart_resetRx(); } //overrun error
    while (RCIF == 0) { //wait until something is received
        asm("CLRWDT");
    }
    unsigned char data = RCREG;
    return data;
}

void uart_writeByte(unsigned char value) {
    while (TXIF == 0) { //wait until buffer is empty
        asm("CLRWDT");
    }
    TXREG = value;
}

void uart_writeBytes(unsigned char value[], short count) {
    for (short i=0; i<count; i++) {
        while (TXIF == 0) { //wait until buffer is empty
            asm("CLRWDT");
        }
        TXREG = value[i];
    }
}
