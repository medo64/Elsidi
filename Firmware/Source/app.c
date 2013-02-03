#include <pic16f1516.h>
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
        while (1) { //wait for reset
            LED = 0;
            __delay_ms(100);
            LED = 1;
            __delay_ms(100);
            asm("CLRWDT");
        }
    }

    settings_init();
    communication_init();

    lcd_init(settings_getInterface(), settings_getDeviceCount());

    int i = 0;

    asm("CLRWDT");
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

    for (int i=0; i<10; i++) {
        asm("CLRWDT");
        __delay_ms(100);
    }
    lcd_reinit(settings_getInterface(), settings_getDeviceCount());
    LED = 0;

    while (1) {
        unsigned char data = readByte();
        LED = 1;
        processByte(data);
        LED = 0;
    }
}
