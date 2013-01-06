#include "communication.h"

#include "lcd.h"
#include "settings.h"
#include "uart.h"


void communication_init() {
    uart_init(9600);
}

unsigned char readByte() {
    return uart_readByte();
}

void writeByte(unsigned char data) {
    uart_writeByte(data);
}


bit readNothing() {
    unsigned char isValid = 1;
    while(1) {
        unsigned char data = readByte();
        if ((data==0x0A) || (data==0x0D)) { break; } //both LF and CR are supported
        isValid = 0;
    }
    return (isValid) ? 1 : 0;
}


#define READPERCENT_STATE_DEFAULT 0
#define READPERCENT_STATE_NUMBER  1
#define READPERCENT_STATE_INCREMENT 2
#define READPERCENT_STATE_DECREMENT 3
#define READPERCENT_STATE_INVALID 255

bit readPercent(unsigned char *value, unsigned char *charCount) {
    *charCount = 0;
    unsigned char state = READPERCENT_STATE_DEFAULT;
    while(1) {
        unsigned char data = readByte();
        if ((data==0x0A) || (data==0x0D)) { break; } //both LF and CR are supported
        if (*charCount < 255) { *charCount += 1; }
        if (state == READPERCENT_STATE_DEFAULT) {
            if ((data >= '0') && (data <= '9')) {
                *value = 0; //start from scratch
                state = READPERCENT_STATE_NUMBER;
            } else if (data == '+') {
                state = READPERCENT_STATE_INCREMENT;
            } else if (data == '-') {
                state = READPERCENT_STATE_DECREMENT;
            } else {
                state = READPERCENT_STATE_INVALID;
            }
        }
        switch (state) {
            case READPERCENT_STATE_NUMBER: {
                if ((data >= '0') && (data <='9')) {
                    unsigned char digitValue = (data - '0');
                    if ((*value < 10) || ((*value == 10) && (digitValue == 0))) {
                        *value = (*value * 10) + digitValue;
                    } else {
                        state = READPERCENT_STATE_INVALID;
                    }
                } else {
                    state = READPERCENT_STATE_INVALID;
                }
            } break;

            case READPERCENT_STATE_INCREMENT: {
                if (data == '+') {
                    if (*value < 100) { *value += 1; }
                } else {
                    state = READPERCENT_STATE_INVALID;
                }
            } break;

            case READPERCENT_STATE_DECREMENT: {
                if (data == '-') {
                    if (*value > 0) { *value -= 1; }
                } else {
                    state = READPERCENT_STATE_INVALID;
                }
            } break;
        }
    }
    return (state != READPERCENT_STATE_INVALID);
}


void writePercent(unsigned char value) {
    unsigned char d1 = value / 100;
    unsigned char d2 = (value / 10) % 10;
    unsigned char d3 = value % 10;
    if (d1 == 0) {
        if (d2 == 0) {
            writeByte('0' + d3);
        } else {
            writeByte('0' + d2);
            writeByte('0' + d3);
        }
    } else {
        writeByte('0' + d1);
        writeByte('0' + d2);
        writeByte('0' + d3);
    }
}


void processByte(unsigned char data) {
    switch (data) {
        case 0x08: { //BS: Command mode
            unsigned char cmd = readByte();
            switch (cmd) {
                case 0x0A:
                case 0x0D: { //clear display
                    lcd_clearDisplay();
                    data = 0x0A; //valid multiline command will result in LF.
                } break;

                case '?': { //ID
                    if (readNothing()) {
                        int i = 0;
                        while (ELSIDI_VERSION[i] != '\0') {
                            writeByte(ELSIDI_VERSION[i]);
                            i++;
                        }
                        data = 0x0A; //valid multiline command will result in LF.
                    }
                } break;

                case '~': { //restore startup defaults
                    if (readNothing()) {
                        settings_init();
                        lcd_setBacklightPwm(settings_getBacklight());
                        lcd_setContrastPwm(settings_getContrast());
                        lcd_useE(0x01);
                        data = 0x0A; //valid multiline command will result in LF.
                    }
                } break;

                case 'b': { //set backlight
                    unsigned char percent = settings_getBacklight();
                    unsigned char charCount;
                    if (readPercent(&percent, &charCount)) {
                        if (charCount == 0) { //report back percentage
                            writePercent(percent);
                        } else {
                            lcd_setBacklightPwm(percent);
                            settings_setBacklight(percent);
                        }
                        data = 0x0A; //valid multiline command will result in LF.
                    }
                } break;

                case 'B': { //store given backlight as a default
                    unsigned char percent = settings_getBacklight();
                    unsigned char charCount;
                    if (readPercent(&percent, &charCount)) {
                        if (charCount > 0) {
                            lcd_setBacklightPwm(percent);
                            settings_setBacklight(percent);
                        }
                        settings_writeBacklight();
                        data = 0x0A; //valid multiline command will result in LF.
                    }
                } break;

                case 'c': { //set contrast
                    unsigned char percent = settings_getContrast();
                    unsigned char charCount;
                    if (readPercent(&percent, &charCount)) {
                        if (charCount == 0) { //report back percentage
                            writePercent(percent);
                        } else {
                            lcd_setContrastPwm(percent);
                            settings_setContrast(percent);
                        }
                        data = 0x0A; //valid multiline command will result in LF.
                    }
                } break;

                case 'C': { //store given contrast as a default
                    unsigned char percent = settings_getContrast();
                    unsigned char charCount;
                    if (readPercent(&percent, &charCount)) {
                        if (charCount > 0) {
                            lcd_setContrastPwm(percent);
                            settings_setContrast(percent);
                        }
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
            unsigned char instruction = readByte();
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

    writeByte(data); //echo
}
