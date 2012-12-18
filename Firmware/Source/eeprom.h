#ifndef EEPROM_H
#define EEPROM_H

/* Returns byte read from EEPROM. */
unsigned char eeprom_readByte(int address);

/* Writes single byte to EEPROM and returns 1 on success. If needed interrupts must be manually reenabled after this function. */
bit eeprom_writeByte(int address, unsigned char data);

#endif
