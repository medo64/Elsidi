#ifndef SETTINGS_H
#define SETTINGS_H

void settings_init();

unsigned char settings_getContrast();
void settings_setContrast(unsigned char value);
void settings_writeContrast();

unsigned char settings_getBacklight();
void settings_setBacklight(unsigned char value);
void settings_writeBacklight();

#endif
