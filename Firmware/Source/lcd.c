#include "lcd.h"
#include <pic.h>
#include "config.h"


unsigned char SelectedE = 0x03;
unsigned char DeviceCount = 1;
unsigned char InterfaceWidth = 8;
unsigned char DisplayState = 0;


void pulseE() {
    if (SelectedE == 0x03) {
        LCD_E1 = 1;
        LCD_E2 = 1;
        __delay_us(5);
        LCD_E1 = 0;
        LCD_E2 = 0;
    } else if (SelectedE == 0x02) {
        LCD_E2 = 1;
        __delay_us(5);
        LCD_E2 = 0;
    } else {
        LCD_E1 = 1;
        __delay_us(5);
        LCD_E1 = 0;
    }
    __delay_us(20);
}

void execute(unsigned char data) {
    if (InterfaceWidth == 4) {
        if (data & 0x80) { LCD_D7 = 1; } else { LCD_D7 = 0; }
        if (data & 0x40) { LCD_D6 = 1; } else { LCD_D6 = 0; }
        if (data & 0x20) { LCD_D5 = 1; } else { LCD_D5 = 0; }
        if (data & 0x10) { LCD_D4 = 1; } else { LCD_D4 = 0; }
        pulseE();
        if (data & 0x08) { LCD_D7 = 1; } else { LCD_D7 = 0; }
        if (data & 0x04) { LCD_D6 = 1; } else { LCD_D6 = 0; }
        if (data & 0x02) { LCD_D5 = 1; } else { LCD_D5 = 0; }
        if (data & 0x01) { LCD_D4 = 1; } else { LCD_D4 = 0; }
        pulseE();
    } else { //assume 8-bit
        if (data & 0x80) { LCD_D7 = 1; } else { LCD_D7 = 0; }
        if (data & 0x40) { LCD_D6 = 1; } else { LCD_D6 = 0; }
        if (data & 0x20) { LCD_D5 = 1; } else { LCD_D5 = 0; }
        if (data & 0x10) { LCD_D4 = 1; } else { LCD_D4 = 0; }
        if (data & 0x08) { LCD_D3 = 1; } else { LCD_D3 = 0; }
        if (data & 0x04) { LCD_D2 = 1; } else { LCD_D2 = 0; }
        if (data & 0x02) { LCD_D1 = 1; } else { LCD_D1 = 0; }
        if (data & 0x01) { LCD_D0 = 1; } else { LCD_D0 = 0; }
        pulseE();
    }
}


char CurrLine = 0;

bit lcd_nextLine() {
    CurrLine += 1;
    if (CurrLine <= 3) {
        if (DeviceCount == 2) {
            switch (CurrLine) {
                case 1:
                    SelectedE = 0x01;
                    lcd_setAddress(0x40);
                    return 1;
                case 2:
                    SelectedE = 0x02;
                    lcd_setAddress(0x00);
                    return 1;
                case 3:
                    SelectedE = 0x02;
                    lcd_setAddress(0x40);
                    return 1;
                default: return 0;
            }
        } else {
            switch (CurrLine) {
                case 1: lcd_setAddress(0x40); return 1;
                case 2: lcd_setAddress(0x14); return 1;
                case 3: lcd_setAddress(0x54); return 1;
                default: return 0;
            }
        }
    } else {
        CurrLine = 4; //just reset it to one line above highest and do nothing
        return 0;
    }
}


void lcd_writeInstruction(unsigned char data) {
    LCD_RS = 0;
    LCD_RW = 0;
    execute(data);
    if ((data == 0x01) || (data == 0x02))  {
        CurrLine = 0;
        __delay_ms(2);
    } else if ((data & 0xF8) == 0x08) { //Display ON/OFF
        DisplayState = data & 0x07;
    } else {
        __delay_us(25);
    }
}

void lcd_clearDisplay() {
    SelectedE = 0x03;
    lcd_writeInstruction(0x01);
    if (DeviceCount == 2) {
        SelectedE = 0x01;
    }
}

void lcd_returnHome() {
    if (DeviceCount == 2) {
        SelectedE = 0x01;
    } else {
        SelectedE = 0x03; //just keep both E lines in same state.
    }
    lcd_writeInstruction(0x02);
}

void lcd_setAddress(unsigned char address) {
    lcd_writeInstruction(0x80 | address);
}


void lcd_writeData(unsigned char data) {
    LCD_RS = 1;
    LCD_RW = 0;
    execute(data);
}


void lcd_setContrastPwm(unsigned char percent) {
    unsigned char value = (unsigned char)((100-percent) * 255 / 100);
    CCPR1L = value;
}

void lcd_setBacklightPwm(unsigned char percent) {
    unsigned char value = (unsigned char)((100-percent) * 255 / 100);
    CCPR2L = value;
}

void lcd_useE(unsigned char mask) {
    SelectedE = mask;
}


void lcd_initInstruction() {
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_D7 = 0;
    LCD_D6 = 0;
    LCD_D5 = 1;
    LCD_D4 = 1;
    LCD_D3 = 0;
    LCD_D2 = 0;
    LCD_D1 = 0;
    LCD_D0 = 0;
    pulseE();
}

void lcd_initInterfaceWidth(unsigned char width) {
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_D7 = 0;
    LCD_D6 = 0;
    LCD_D5 = 1;
    LCD_D4 = (width == 4) ? 0 : 1;
    LCD_D3 = 0;
    LCD_D2 = 0;
    LCD_D1 = 0;
    LCD_D0 = 0;
    pulseE();
    InterfaceWidth = width;
}

void lcd_reinit(unsigned char width, unsigned char deviceCount) {
    SelectedE = 0x03;
    lcd_initInstruction();      //Function set (0011 ****)
    __delay_ms(10);             //additional delay
    lcd_initInstruction();      //Function set (0011 ****) - it really needs to be set second time (of four)
    __delay_ms(1);              //additional delay
    lcd_initInstruction();      //Function set (0011 ****) - it really needs to be set third time (of four)
    __delay_ms(1);              //additional delay
    lcd_initInterfaceWidth(width);
    if (width == 4) {
        lcd_writeInstruction(0x28); //Function set (0010 1000) set 4-bit operation, 2-line display and 5x8 font
    } else {
        lcd_writeInstruction(0x38); //Function set (0011 1000) set 8-bit operation, 2-line display and 5x8 font
    }
    lcd_writeInstruction(0x08); //Display off (0000 1000) - turn off display
    lcd_writeInstruction(0x01); //Clear screen
    lcd_writeInstruction(0x06); //Entry mode set (0000 0110)
    lcd_writeInstruction(0x14); //Sets both address and shift incrementing by one
    lcd_writeInstruction(0x02); //Return home (0000 0010)
    lcd_writeInstruction(0x0C); //Display control (0000 1100) - turn on display

    CurrLine = 0;
    DeviceCount = deviceCount;
    if (DeviceCount == 2) {
        SelectedE = 0x01;
    } else {
        SelectedE = 0x03; //just keep both E lines in same state.
    }
}

void lcd_init(unsigned char width, unsigned char deviceCount) {
    __delay_ms(50);
    lcd_reinit(width, deviceCount);
}


bit lcd_isDisplayOn() {
    return ((DisplayState & 0x04) != 0);
}

bit lcd_isCursorOn() {
    return ((DisplayState & 0x02) != 0);
}

bit lcd_isCursorBlink() {
    return ((DisplayState & 0x01) != 0);
}


void lcd_setDisplayOn(unsigned char state) {
    unsigned char newState = (DisplayState & 0x03) | (state ? 0x04 : 0x00);
    lcd_writeInstruction(0x08 | newState);
}

void lcd_setCursorOn(unsigned char state) {
    unsigned char newState = (DisplayState & 0x05) | (state ? 0x02 : 0x00);
    lcd_writeInstruction(0x08 | newState);
}

void lcd_setCursorBlinkOn(unsigned char state) {
    unsigned char newState = (DisplayState & 0x06) | (state ? 0x01 : 0x00);
    lcd_writeInstruction(0x08 | newState);
}
