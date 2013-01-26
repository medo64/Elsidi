#include "settings.h"
#include "pic.h"

#include "config.h"


#define DEFAULT_CONTRAST   50
#define DEFAULT_BACKLIGHT   0
#define DEFAULT_INTERFACE   8
#define DEFAULT_DEVICECOUNT 1

#define OFFSET_CONTRAST    0
#define OFFSET_BACKLIGHT   1
#define OFFSET_INTERFACE   2
#define OFFSET_DEVICECOUNT 3

#define FLASH_RAW { DEFAULT_CONTRAST, DEFAULT_BACKLIGHT, DEFAULT_INTERFACE, DEFAULT_DEVICECOUNT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } //reserving space because erase block is block 32-word
#define FLASH_RAW_LOCATION 0x1800
#define FLASH_RAW_COUNT 32

#define STORE_EMPTY { 0, 0, 0, 0 }
#define STORE_COUNT 4

const unsigned char FLASH[] @ FLASH_RAW_LOCATION = FLASH_RAW;


unsigned char current_contrast = 0;
unsigned char current_backlight = 0;
unsigned char current_interface = 0;
unsigned char current_deviceCount = 0;


void settings_write(int index, unsigned char data) {
    unsigned char newProgram[] = STORE_EMPTY;
    for (int i=0; i<STORE_COUNT; i++) {
        newProgram[i] = FLASH[i];
    }
    newProgram[index] = data;

    GIE = 0; //disable interrupts
    WREN = 1; //enable write to memory
    CFGS = 0; //access Flash program memory

    //erase row
    PMADR = FLASH_RAW_LOCATION;
    FREE = 1; //enable block Erase operation
    #asm //unlock flash
        BANKSEL PMCON2
        MOVLW 055h
        MOVWF (PMCON2 & 0x7F)   ;write 55h
        MOVLW 0AAh
        MOVWF (PMCON2 & 0x7F)   ;write AAh
        BSF (PMCON1 & 0x7F), 1  ;WR=1
        NOP
        NOP
    #endasm

    //write data
    LWLO = 0; //immediate write
    for (int i=0; i<STORE_COUNT; i++) {
        PMADR = FLASH_RAW_LOCATION + i;
        PMDATH = 0;
        PMDATL = newProgram[i];
        #asm //unlock flash
            BANKSEL PMCON2
            MOVLW 055h
            MOVWF (PMCON2 & 0x7F)   ;write 55h
            MOVLW 0AAh
            MOVWF (PMCON2 & 0x7F)   ;write AAh
            BSF (PMCON1 & 0x7F), 1  ;WR=1
            NOP
            NOP
        #endasm
    }

    WREN = 0; //disable write to memory
    GIE = 1; //enable interrupts
}


void settings_init() {
    current_contrast = FLASH[OFFSET_CONTRAST];
    current_backlight = FLASH[OFFSET_BACKLIGHT];
    current_interface = FLASH[OFFSET_INTERFACE];
    current_deviceCount = FLASH[OFFSET_DEVICECOUNT];
}


unsigned char  settings_getContrast() {
    unsigned char value = current_contrast;
    if (value > 100) { value = 100; }
    return value;
}

void settings_setContrast(unsigned char value) {
    current_contrast = value;
}

void settings_writeContrast() {
    settings_write(OFFSET_CONTRAST, current_contrast);
}


unsigned char  settings_getBacklight() {
    unsigned char value = current_backlight;
    if (value > 100) { value = 100; }
    return value;
}

void settings_setBacklight(unsigned char value) {
    current_backlight = value;
}

void settings_writeBacklight() {
    settings_write(OFFSET_BACKLIGHT, current_backlight);
}


unsigned char settings_getInterface() {
    return (current_interface == 4) ? 4 : DEFAULT_INTERFACE;
}

void settings_setInterface(unsigned char value) {
    current_interface = value;
}

void settings_writeInterface() {
    settings_write(OFFSET_INTERFACE, current_interface);
}


unsigned char settings_getDeviceCount() {
    return (current_deviceCount == 2) ? 2 : DEFAULT_DEVICECOUNT;
}

void settings_setDeviceCount(unsigned char value) {
    current_deviceCount = value;
}

void settings_writeDeviceCount() {
    settings_write(OFFSET_DEVICECOUNT, current_deviceCount);
}
