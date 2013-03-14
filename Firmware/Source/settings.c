#include "settings.h"
#include "pic.h"

#include "config.h"


#define DEFAULT_CONTRAST  50
#define DEFAULT_BACKLIGHT  0
#define DEFAULT_WIDTH     20
#define DEFAULT_HEIGHT     4
#define DEFAULT_BUSWIDTH   8

#define OFFSET_CONTRAST   0
#define OFFSET_BACKLIGHT  1
#define OFFSET_WIDTH      2
#define OFFSET_HEIGHT     3
#define OFFSET_BUSWIDTH   4

#define FLASH_RAW { DEFAULT_CONTRAST, DEFAULT_BACKLIGHT, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BUSWIDTH, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } //reserving space because erase block is block 32-word
#define FLASH_RAW_LOCATION 0x1800
#define FLASH_RAW_COUNT 32

#define STORE_EMPTY { 0, 0, 0, 0, 0 }
#define STORE_COUNT 5

const unsigned char FLASH[] @ FLASH_RAW_LOCATION = FLASH_RAW;


unsigned char current_contrast = 0;
unsigned char current_backlight = 0;
unsigned char current_width = 0;
unsigned char current_height = 0;
unsigned char current_busWidth = 0;


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
    current_width = FLASH[OFFSET_WIDTH];
    current_height = FLASH[OFFSET_HEIGHT];
    current_busWidth = FLASH[OFFSET_BUSWIDTH];
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


unsigned char settings_getWidth() {
    return ((current_width == 0) || (current_width > 80)) ? DEFAULT_WIDTH : current_width;
}

void settings_setWidth(unsigned char value) {
    current_width = value;
}

void settings_writeWidth() {
    settings_write(OFFSET_WIDTH, current_width);
}


unsigned char settings_getHeight() {
    return ((current_height == 0) || (current_height > 4)) ? DEFAULT_HEIGHT : current_height;
}

void settings_setHeight(unsigned char value) {
    current_height = value;
}

void settings_writeHeight() {
    settings_write(OFFSET_HEIGHT, current_height);
}


unsigned char settings_getBusWidth() {
    return (current_busWidth == 4) ? 4 : DEFAULT_BUSWIDTH;
}

void settings_setBusWidth(unsigned char value) {
    current_busWidth = value;
}

void settings_writeBusWidth() {
    settings_write(OFFSET_BUSWIDTH, current_busWidth);
}


void settings_writeDefaults() {
    settings_write(OFFSET_BACKLIGHT, DEFAULT_BACKLIGHT);
    settings_write(OFFSET_CONTRAST, DEFAULT_CONTRAST);
    settings_write(OFFSET_WIDTH, DEFAULT_WIDTH);
    settings_write(OFFSET_HEIGHT, DEFAULT_HEIGHT);
    settings_write(OFFSET_BUSWIDTH, DEFAULT_BUSWIDTH);
}
