#include "lcd.h"
#include <pic.h>


unsigned char _lcd_selectedE = 0x01;

void _lcd_setEHigh() {
    if (_lcd_selectedE & 0x01) { LCD_E1 = 1;  }
    if (_lcd_selectedE & 0x02) { LCD_E2 = 1;  }
    __delay_ms(1);
}

void _lcd_setELow() {
    if (_lcd_selectedE & 0x01) { LCD_E1 = 0;  }
    if (_lcd_selectedE & 0x02) { LCD_E2 = 0;  }
    __delay_ms(1);
}

void _lcd_setDB(unsigned char data) {
    if (data & 0x01) { LCD_D0 = 1; } else { LCD_D0 = 0; }
    if (data & 0x02) { LCD_D1 = 1; } else { LCD_D1 = 0; }
    if (data & 0x04) { LCD_D2 = 1; } else { LCD_D2 = 0; }
    if (data & 0x08) { LCD_D3 = 1; } else { LCD_D3 = 0; }
    if (data & 0x10) { LCD_D4 = 1; } else { LCD_D4 = 0; }
    if (data & 0x20) { LCD_D5 = 1; } else { LCD_D5 = 0; }
    if (data & 0x40) { LCD_D6 = 1; } else { LCD_D6 = 0; }
    if (data & 0x80) { LCD_D7 = 1; } else { LCD_D7 = 0; }
}


char _lcd_currLine = 0;

void lcd_nextLine() {
    _lcd_currLine += 1;
    if (_lcd_currLine <= 3) {
        switch (_lcd_currLine) {
            case 1: lcd_setAddress(0x40); break;
            case 2: lcd_setAddress(0x14); break;
            case 3: lcd_setAddress(0x54); break;
        }
    } else {
        _lcd_currLine = 4; //just reset it to one line above highest and do nothing
    }
}


void lcd_writeInstruction(unsigned char data) {
    LCD_RS = 0;
    LCD_RW = 0;
    _lcd_setDB(data);
    _lcd_setEHigh();
    _lcd_setELow();
    __delay_ms(3);
    if ((data == 0x01) || (data == 0x02))  { _lcd_currLine = 0; }
}

void lcd_clearDisplay() {
    lcd_writeInstruction(0x01);
}

void lcd_returnHome() {
    lcd_writeInstruction(0x02);
    __delay_ms(3);
}

void lcd_setAddress(unsigned char address) {
    lcd_writeInstruction(0x80 | address);
}


void lcd_writeData(unsigned char data) {
    LCD_RS = 1;
    LCD_RW = 0;
    _lcd_setDB(data);
    _lcd_setEHigh();
    _lcd_setELow();
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
    _lcd_selectedE = mask;
}


void lcd_init() {
    lcd_writeInstruction(0x38); //Function set (11****)
    __delay_ms(10);             //additional delay
    lcd_writeInstruction(0x38); //Function set (11****) - it really needs to be set second time (of four)
    lcd_writeInstruction(0x38); //Function set (11****) - it really needs to be set third time (of four)
    lcd_writeInstruction(0x38); //Function set (111000) set 8-bit operation, 2-line display and 5x8 font
    lcd_writeInstruction(0x08); //Display off (1000) - turn off display
    lcd_writeInstruction(0x01); //clear screen
    lcd_writeInstruction(0x06); //Entry mode set (110)
    lcd_writeInstruction(0x14); //sets both address and shift incrementing by one
    lcd_writeInstruction(0x02); //Return home (10)
    lcd_writeInstruction(0x0C); //Display control (1100) - turn on display
}
