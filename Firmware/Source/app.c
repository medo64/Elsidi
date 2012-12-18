#include <pic16f1936.h>
#include <pic.h>
#include "config.h"

#include "eeprom.h"
#include "uart.h"

#include "settings.h"
#include "lcd.h"


void splash() {
    LED = 1;
    __delay_ms(100);
    LED = 0;
    __delay_ms(100);
    LED = 1;
    __delay_ms(100);
    LED = 0;
    __delay_ms(100);
    LED = 1;
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
    lcd_returnHome();

    __delay_ms(100);
    lcd_setContrastPwm(settings_getContrast());
    lcd_setBacklightPwm(settings_getBacklight());
    LED = 0;


    while (1) {
        unsigned char data = uart_readByte();
        LED = 1;
        switch (data) {
            case 0x08: { //BS: Command mode
                unsigned char cmd = uart_readByte();
                switch (cmd) {
                    case 0x0A:
                    case 0x0D: { //clear display
                        lcd_clearDisplay();
                        data = 0x0A; //valid multiline command will result in LF.
                    } break;

                    case '?': { //ID
                        if (readNothing()) {
                            uart_writeBytes((unsigned char*)"Elsidi K 001", 12);
                            data = 0x0A; //valid multiline command will result in LF.
                        }
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
                            lcd_setBacklightPwm(percent);
                            settings_setBacklight(percent);
                            data = 0x0A; //valid multiline command will result in LF.
                        }
                    } break;

                    case 'B': { //store given backlight as a default
                        int percent = readInt();
                        if ((percent >=0) && (percent<=100)) {                            
                            lcd_setBacklightPwm(percent);
                            settings_setBacklight(percent);
                            settings_writeBacklight();
                            data = 0x0A; //valid multiline command will result in LF.
                        }
                    } break;

                    case 'c': { //set contrast
                        int percent = readInt();
                        if ((percent >=0) && (percent<=100)) {
                            lcd_setContrastPwm(percent);
                            settings_setContrast(percent);
                            data = 0x0A; //valid multiline command will result in LF.
                        }
                    } break;

                    case 'C': { //store given contrast as a default
                        int percent = readInt();
                        if ((percent >=0) && (percent<=100)) {
                            lcd_setContrastPwm(percent);
                            settings_setContrast(percent);
                            settings_writeContrast();
                            data = 0x0A; //valid multiline command will result in LF.
                        }
                    } break;

                    default: readNothing(); break;
                }
            } break;

            case 0x09: { //HT: Next line
                lcd_nextLine();
            } break;

            case 0x0A:
            case 0x0D: { //LF/CR: Return home
                lcd_returnHome();
            } break;

            case 0x0B: { //VT: Reserved
            } break;

            case 0x0C: { //FF: LCD instruction mode
                unsigned char instruction = uart_readByte();
                lcd_writeInstruction(instruction);
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
        LED = 0;
    }
}
