#include <pic16f1936.h>
#include <pic.h>
#include "communication.h"
#include "config.h"
#include "defaults.h"
#include "lcd.h"
#include "settings.h"


void main() {
    init();
    LED = 1;

    if (isResetToDefaultsRequired()) {
        resetToDefaults();
        while (1); //wait for reset
    }

    settings_init();
    communication_init();

    
    lcd_init(settings_getInterface());

    int i = 0;
    while (ELSIDI_NAME[i] != '\0') {
        lcd_writeData(ELSIDI_NAME[i]);
        i++;
    }
    i = 0;
    lcd_nextLine(0x40); //line2
    while (ELSIDI_URL[i] != '\0') {
        lcd_writeData(ELSIDI_URL[i]);
        i++;
    }

    lcd_setContrastPwm(settings_getContrast());
    lcd_setBacklightPwm(settings_getBacklight());

    __delay_ms(1000);
    lcd_clearDisplay();
    LED = 0;

    while (1) {
        unsigned char data = readByte();
        LED = 1;
        processByte(data);
        LED = 0;
    }
}
