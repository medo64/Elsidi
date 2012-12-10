#include <pic16f1936.h>
#include "config.h"

#include "eeprom.h"
#include "uart.h"
#include "wait.h"

#include "settings.h"
#include "lcd.h"


void splash() {
    LATC0 = 1;
    wait_ms(100);
    LATC0 = 0;
    wait_ms(100);
    LATC0 = 1;
    wait_ms(100);
    LATC0 = 0;
    wait_ms(100);
    LATC0 = 1;
}


int readInt() {
    unsigned char isValid = 1;
    unsigned char hadNumbers = 0;
    int value = 0;
    while(1) {
        unsigned char data = uart_readByte();
        if ((data==0x0A) || (data==0x0D)) { break; } //both LF and CR are supported
        if (isValid) {
            if ((data >= '0') && (data <='9')) {
                hadNumbers = 1;
                unsigned char digitValue = (data - '0');
                value = value * 10 + digitValue;
            } else {
                isValid = 0;
            }
        }
    }
    return (isValid && hadNumbers) ? value : -32768;
}

bit readNothing() {
    unsigned char isValid = 1;
    while(1) {
        unsigned char data = uart_readByte();
        if ((data==0x0A) || (data==0x0D)) { break; } //both LF and CR are supported
        isValid = 0;
    }
    return (isValid) ? 1 : 0;
}


const unsigned char DEFAULT_LINE_1[] = "Elsidi\0";
const unsigned char DEFAULT_LINE_2[] = "www.jmedved.com\0";

void main() {
    init();
    settings_init();
    uart_init(9600);

    splash();

    lcd_init();


    int i = 0;
    while (DEFAULT_LINE_1[i] != '\0') {
        lcd_writeData(DEFAULT_LINE_1[i]);
        i++;
    }
    i = 0;
    lcd_setAddress(0x40); //line2
    while (DEFAULT_LINE_2[i] != '\0') {
        lcd_writeData(DEFAULT_LINE_2[i]);
        i++;
    }

    wait_ms(2000); //wait for 2 seconds


    char currLine = 0;
    while (1) {
        unsigned char data = uart_readByte();
        switch (data) {
            case 0x08: { //BS: Command mode
                unsigned char cmd = uart_readByte();
                switch (cmd) {
                    case 0x0A:
                    case 0x0D: { //clear display
                        lcd_clearDisplay();
                        data = 0x0A; //valid multiline command will result in LF.
                    } break;

                    case '~': { //restore defaults
                        if (readNothing()) {
                            settings_defaultContrast();
                            settings_defaultBacklight();
                            settings_writeContrast();
                            settings_writeBacklight();
                            lcd_useE(0x03);
                            data = 0x0A; //valid multiline command will result in LF.
                        }
                    } break;

                    case 'b': { //set backlight
                        int percent = readInt();
                        if ((percent >=0) && (percent<=100)) {
                            unsigned char value = (unsigned char)(percent * 255 / 100);
                            lcd_setBacklightPwm(value);
                            settings_setBacklight(value);
                            data = 0x0A; //valid multiline command will result in LF.
                        }
                    } break;

                    case 'B': { //store given backlight as a default
                        int percent = readInt();
                        if ((percent >=0) && (percent<=100)) {
                            unsigned char value = (unsigned char)(percent * 255 / 100);
                            lcd_setBacklightPwm(value);
                            settings_setBacklight(value);
                            settings_writeBacklight();
                            data = 0x0A; //valid multiline command will result in LF.
                        }
                    } break;

                    case 'c': { //set contrast
                        int percent = readInt();
                        if ((percent >=0) && (percent<=100)) {
                            unsigned char value = (unsigned char)((100-percent) * 255 / 100);
                            lcd_setContrastPwm(value);
                            settings_setContrast(value);
                            data = 0x0A; //valid multiline command will result in LF.
                        }
                    } break;

                    case 'C': { //store given contrast as a default
                        int percent = readInt();
                        if ((percent >=0) && (percent<=100)) {
                            unsigned char value = (unsigned char)((100-percent) * 255 / 100);
                            lcd_setContrastPwm(value);
                            settings_setContrast(value);
                            settings_writeContrast();
                            data = 0x0A; //valid multiline command will result in LF.
                        }
                    } break;
                }
            } break;

            case 0x09: { //HT: Next line
                currLine += 1;
                if (currLine <= 4) {
                    switch (currLine) {
                        case 1: lcd_setAddress(0x00); break;
                        case 2: lcd_setAddress(0x40); break;
                        case 3: lcd_setAddress(0x14); break;
                        case 4: lcd_setAddress(0x54); break;
                    }
                } else {
                    currLine = 4; //just reset it to highest line but do nothing
                }
            } break;

            case 0x0A: { //LF: Return home
                lcd_returnHome();
            } break;

            case 0x0B: { //VT: Reserved
            } break;

            case 0x0C: { //FF: LCD instruction mode
                unsigned char instruction = uart_readByte();
                lcd_writeInstruction(instruction);
            } break;

            case 0x0D: { //CR: Return home
                lcd_returnHome();
            } break;

            case 0x0E: { //SO: Select secondary display
                lcd_useE(0x02);
            } break;

            case 0x0F: { //SI: Select primary display
                lcd_useE(0x01);
            } break;

            default:
                lcd_writeData(data);
                break;
        }

        uart_writeByte(data); //echo
    }
}
