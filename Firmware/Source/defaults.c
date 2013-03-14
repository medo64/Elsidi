#include "defaults.h"
#include <pic.h>
#include "config.h"
#include "lcd.h"
#include "settings.h"


bit isResetToDefaultsRequired() {
    unsigned char isShorted = 1;

    LCD_D0_TRIS = 1; //make DB0 an input

    LCD_D1 = 0;
    __delay_ms(1);
    if (LCD_D0_PORT != 0) { isShorted = 0; }

    LCD_D1 = 1;
    __delay_ms(1);
    if (LCD_D0_PORT != 1) { isShorted = 0; }

    LCD_D0_TRIS = 0; //DB0 is output again

    return isShorted;
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

    lcd_setBacklightPwm(settings_getBacklight());
    lcd_setContrastPwm(settings_getContrast());
}
