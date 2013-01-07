#include "eeprom.h"
#include <pic.h>


unsigned char eeprom_readByte(int address) {
    EEADR = address;
    CFGS = 0; //deselect Config space
    EEPGD = 0; //point to DATA memory
    RD = 1;
    asm("CLRWDT");
    return EEDATL;
}

bit eeprom_writeByte(int address, unsigned char data) {
    if ((address<0) || (address >= _EEPROMSIZE)) { return 0; }

    EEADR = address;
    EEDATL = data;
    WREN = 1;
    CFGS = 0; //deselect Config space
    EEPGD = 0; //point to DATA memory

    GIE = 0; //disable interrupts
    while (GIE != 0) { GIE = 0; } //see AN576.

    EECON2 = 0x55;
    EECON2 = 0xAA;
    WR = 1;
    WREN = 0;

    while (WR != 0) {
        asm("CLRWDT");
    }
    return 1;
}
