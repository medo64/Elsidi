#include "defaults.h"
#include <pic.h>
#include "config.h"
#include "lcd.h"
#include "settings.h"


bit checkJumperState(unsigned char state) {
    LCD_D0 = 0; //set DB0=0
    __delay_ms(1);
    LCD_D0_TRIS = 1; //make DB0 an input
    __delay_ms(1);
    LCD_D1 = state; //set DB1=(state)
    __delay_ms(1);
    unsigned char result = LCD_D0_PORT;
    LCD_D0_TRIS = 0; //restore DB0 to output
    return (result==state) ? 0 : 1;
}

bit isResetToDefaultsRequired() {
    for (char i=0; i<8; i++) {
        if (!checkJumperState(1) || !checkJumperState(0)) { return  0; }
    }
    return 1;
}


void resetToDefaults() {
    settings_writeDefaults();

    lcd_init(4, settings_getWidth(), settings_getHeight()); //we cannot use 8-bit interface since DB0 and DB1 are shorted!
    lcd_writeData('D');
    lcd_writeData('E');
    lcd_writeData('F');
    lcd_writeData('A');
    lcd_writeData('U');
    lcd_writeData('L');
    lcd_writeData('T');
    lcd_writeData('S');
    lcd_nextLine();
    lcd_writeNumber(settings_getWidth());
    lcd_writeData('x');
    lcd_writeNumber(settings_getHeight());
    lcd_writeData(' ');
    lcd_writeData('B');
    lcd_writeNumber(settings_getBacklight());
    lcd_writeData(' ');
    lcd_writeData('C');
    lcd_writeNumber(settings_getContrast());
    lcd_writeData(' ');
    lcd_writeData('D');
    lcd_writeNumber(settings_getBusWidth());

    lcd_setBacklightPwm(settings_getBacklight());
    lcd_setContrastPwm(settings_getContrast());
}
