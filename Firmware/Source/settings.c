#include "settings.h"
#include "eeprom.h"


#define SETTINGS_EEPROM_ADDRESS_CONTRAST  240
#define SETTINGS_EEPROM_ADDRESS_BACKLIGHT 241
#define SETTINGS_EEPROM_ADDRESS_INTERFACE 242

unsigned char current_contrast = 0;
unsigned char current_backlight = 0;
unsigned char current_interface = 0;


void settings_init() {
    current_contrast = eeprom_readByte(SETTINGS_EEPROM_ADDRESS_CONTRAST);
    current_backlight = eeprom_readByte(SETTINGS_EEPROM_ADDRESS_BACKLIGHT);
    current_interface = eeprom_readByte(SETTINGS_EEPROM_ADDRESS_INTERFACE);
}


unsigned char settings_getContrast() {
    return current_contrast;
}

void settings_setContrast(unsigned char value) {
    current_contrast = value;
}

void settings_writeContrast() {
    eeprom_writeByte(SETTINGS_EEPROM_ADDRESS_CONTRAST, current_contrast);
}


unsigned char settings_getBacklight() {
    return current_backlight;
}

void settings_setBacklight(unsigned char value) {
    current_backlight = value;
}

void settings_writeBacklight() {
    eeprom_writeByte(SETTINGS_EEPROM_ADDRESS_BACKLIGHT, current_backlight);
}


unsigned char settings_getInterface() {
    return (current_interface == 4) ? 4 : 8;
}

void settings_setInterface(unsigned char value) {
    current_interface = value;
}

void settings_writeInterface() {
    eeprom_writeByte(SETTINGS_EEPROM_ADDRESS_INTERFACE, current_interface);
}
