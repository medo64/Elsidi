#include "defaults.h"
#include <pic.h>
#include "settings.h"
#include "lcd.h"


#define bitSet(var, bitno)              ((var) |= 1UL << (bitno))
#define bitClear(var, bitno)            ((var) &= ~(1UL << (bitno)))


bit isResetToDefaultsRequired() {
    unsigned char isShorted = 1;

    TRISB0 = 1; //make DB0 an input

    LATA1 = 0;
    __delay_ms(1);
    if (PORTBbits.RB0 != 0) { isShorted = 0; }

    LATA1 = 1;
    __delay_ms(1);
    if (PORTBbits.RB0 != 1) { isShorted = 0; }

    TRISB0 = 0; //DB0 is output again

    return isShorted;
}

void resetToDefaults() {
    settings_setBacklight(1);
    settings_writeBacklight();
    settings_setContrast(50);
    settings_writeContrast();
    settings_setInterface(8);
    settings_writeInterface();

    lcd_init(4); //we cannot use 8-bit interface since DB0 and DB1 are shorted!
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
