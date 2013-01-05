#include "settings.h"
#include "eeprom.h"


#define _SETTINGS_EEPROM_ADDRESS_CONTRAST   240
#define _SETTINGS_EEPROM_ADDRESS_BACKLIGHT  241

unsigned char _settings_current_contrast = 0;
unsigned char _settings_current_backlight = 0;


void settings_init() {
    _settings_current_contrast = eeprom_readByte(_SETTINGS_EEPROM_ADDRESS_CONTRAST);
    _settings_current_backlight = eeprom_readByte(_SETTINGS_EEPROM_ADDRESS_BACKLIGHT);
}


unsigned char settings_getContrast() {
    return _settings_current_contrast;
}

void settings_setContrast(unsigned char value) {
    _settings_current_contrast = value;
}

void settings_writeContrast() {
    eeprom_writeByte(_SETTINGS_EEPROM_ADDRESS_CONTRAST, _settings_current_contrast);
}


unsigned char settings_getBacklight() {
    return _settings_current_backlight;
}

void settings_setBacklight(unsigned char value) {
    _settings_current_backlight = value;
}

void settings_writeBacklight() {
    eeprom_writeByte(_SETTINGS_EEPROM_ADDRESS_BACKLIGHT, _settings_current_backlight);
}
