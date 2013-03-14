#ifndef SETTINGS_H
#define SETTINGS_H

void settings_init();
void settings_writeDefaults();

unsigned char settings_getContrast();
void settings_setContrast(unsigned char value);
void settings_writeContrast();

unsigned char settings_getBacklight();
void settings_setBacklight(unsigned char value);
void settings_writeBacklight();

unsigned char settings_getWidth();
void settings_setWidth(unsigned char value);
void settings_writeWidth();

unsigned char settings_getHeight();
void settings_setHeight(unsigned char value);
void settings_writeHeight();

unsigned char settings_getBusWidth();
void settings_setBusWidth(unsigned char value);
void settings_writeBusWidth();

#endif
