#include "settings.h"
#include "eeprom.h"


#define SETTINGS_EEPROM_ADDRESS_CONTRAST  240
#define SETTINGS_EEPROM_ADDRESS_BACKLIGHT 241
#define SETTINGS_EEPROM_ADDRESS_INTERFACE 242
#define SETTINGS_EEPROM_ADDRESS_DEVICECOUNT 243

unsigned char current_contrast = 0;
unsigned char current_backlight = 0;
unsigned char current_interface = 0;
unsigned char current_deviceCount = 0;


void settings_init() {
    current_contrast = eeprom_readByte(SETTINGS_EEPROM_ADDRESS_CONTRAST);
    current_backlight = eeprom_readByte(SETTINGS_EEPROM_ADDRESS_BACKLIGHT);
    current_interface = eeprom_readByte(SETTINGS_EEPROM_ADDRESS_INTERFACE);
    current_deviceCount = eeprom_readByte(SETTINGS_EEPROM_ADDRESS_DEVICECOUNT);
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


unsigned char settings_getDeviceCount() {
    return (current_deviceCount == 2) ? 2 : 1;
}

void settings_setDeviceCount(unsigned char value) {
    current_deviceCount = value;
}

void settings_writeDeviceCount() {
    eeprom_writeByte(SETTINGS_EEPROM_ADDRESS_DEVICECOUNT, current_deviceCount);
}
