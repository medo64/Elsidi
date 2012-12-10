unsigned char _lcd_selectedE = 0x03; //select both displays at first

void _lcd_setEHigh() {
    if (_lcd_selectedE & 0x01) { LCD_E1 = 1;  }
    if (_lcd_selectedE & 0x02) { LCD_E2 = 1;  }
}

void _lcd_setELow() {
    if (_lcd_selectedE & 0x01) { LCD_E1 = 0;  }
    if (_lcd_selectedE & 0x02) { LCD_E2 = 0;  }
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


void lcd_writeInstruction(unsigned char data) {
    LCD_RS = 0;
    LCD_RW = 0;
    _lcd_setEHigh();
    _lcd_setDB(data);
    _lcd_setELow();
    wait_ms(1);
}

void lcd_clearDisplay() {
    lcd_writeInstruction(0x01);
}

void lcd_returnHome() {
    lcd_writeInstruction(0x02);
    wait_ms(2);
}

void lcd_setAddress(unsigned char address) {
    lcd_writeInstruction(0x80 | address);
}


void lcd_writeData(unsigned char data) {
    LCD_RS = 1;
    LCD_RW = 0;
    _lcd_setEHigh();
    _lcd_setDB(data);
    _lcd_setELow();
}


void lcd_setContrastPwm(unsigned char data) {
    CCPR1L = data;
}

void lcd_setBacklightPwm(unsigned char data) {
    CCPR2L = data;
}

void lcd_useE(unsigned char mask) {
    _lcd_selectedE = mask;
}


void lcd_init() {
    wait_ms(60);                //wait for display to stabilize

    lcd_writeInstruction(0x38); //Function set (11****)
    wait_ms(6);                 //longer wait (at least 4.1*2 is needed - 6 + 4)

    lcd_writeInstruction(0x38); //Function set (11****) - it really needs to be set second time (of four)
    wait_ms(1);

    lcd_writeInstruction(0x38); //Function set (11****) - it really needs to be set third time (of four)
    wait_ms(1);

    lcd_writeInstruction(0x38); //Function set (111000) set 8-bit operation, 2-line display and 5x8 font
    wait_ms(1);

    lcd_writeInstruction(0x08); //Display off (1000) - turn off display
    wait_ms(1);

    lcd_writeInstruction(0x01); //clear screen
    wait_ms(1);

    lcd_writeInstruction(0x06); //Entry mode set (110)
    wait_ms(1);

    lcd_writeInstruction(0x14); //sets both address and shift incrementing by one
    wait_ms(2);

    lcd_writeInstruction(0x02); //Return home (10)
    wait_ms(3);

    lcd_writeInstruction(0x0C); //Display control (1100) - turn on display
    wait_ms(1);

}
