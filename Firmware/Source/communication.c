#include "communication.h"
#include "config.h"
#include "lcd.h"
#include "settings.h"
#include "uart.h"

#define BS 0x08 //Return home
#define HT 0x09 //Command mode
#define LF 0x0A //Next line
#define VT 0x0B //Clear display
#define FF 0x0C //LCD instruction mode
#define CR 0x0D //Next line
#define SO 0x0E //Select secondary display (E2).
#define SI 0x0F //Select primary display (E1).

#define SUCCESS LF


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
        if ((data == CR) || (data == LF)) { break; } //both CR and LF are supported
        isValid = 0;
    }
    return (isValid) ? 1 : 0;
}


#define READPERCENT_STATE_DEFAULT     0
#define READPERCENT_STATE_NUMBER      1
#define READPERCENT_STATE_INCREMENT   2
#define READPERCENT_STATE_DECREMENT   3
#define READPERCENT_STATE_INVALID   255

bit readPercent(unsigned char *value, unsigned char *charCount) {
    *charCount = 0;
    unsigned char state = READPERCENT_STATE_DEFAULT;
    while(1) {
        unsigned char data = readByte();
        if ((data == CR) || (data == LF)) { break; } //both CR and LF are supported
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


bit readSmallNumber(unsigned char *value, unsigned char *charCount) { //0 to 100
    *charCount = 0;
    unsigned char isValid = 1;
    unsigned char newValue = 0;
    while(1) {
        unsigned char data = readByte();
        if ((data == CR) || (data == LF)) { break; } //both CR and LF are supported
        if (*charCount < 255) { *charCount += 1; }
        if (isValid && (data >= '0') && (data <= '9')) {
           unsigned char digitValue = (data - '0');
           if ((newValue < 10) || ((newValue == 10) && (digitValue == 0))) {
               newValue = (newValue * 10) + digitValue;
           } else {
               isValid = 0; //number is 10000 or larger
           }
        } else {
            isValid = 0;
        }
    }
    if (isValid) {
        if (*charCount > 0) { *value = newValue; }
        return 1;
    } else {
        return 0;
    }
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

void writeNumber(int value) {
    int currPosition = 10000;
    unsigned char leadingZero = 1;
    while (currPosition > 0) {
        int digit = (value / currPosition) % 10;
        if ((digit == 0) && leadingZero) {
        } else {
            writeByte('0' + (unsigned char)digit);
            leadingZero = 0;
        }
        currPosition /= 10;
    }
    if (leadingZero) { writeByte('0'); }
}


void processByte(unsigned char data) {
    switch (data) {
        case BS: { //Return home
            lcd_returnHome();
            data = SUCCESS;
        } break;

        case HT: { //Command mode
            unsigned char cmd = readByte();
            switch (cmd) {
                case CR:
                case LF: { //clear display
                    lcd_clearDisplay();
                    data = SUCCESS;
                } break;

                case '?': { //ID
                    if (readNothing()) {
                        int i = 0;
                        while (ELSIDI_VERSION[i] != '\0') {
                            writeByte(ELSIDI_VERSION[i]);
                            i++;
                        }
                        data = SUCCESS;
                    }
                } break;

                case '~': { //restore startup defaults
                    if (readNothing()) {
                        settings_init();
                        lcd_setBacklightPwm(settings_getBacklight());
                        lcd_setContrastPwm(settings_getContrast());
                        lcd_reinit(settings_getInterface(), settings_getDeviceCount());
                        data = SUCCESS;
                    }
                } break;

                case '*': { //set display
                    unsigned char cmdData = readByte();
                    if ((cmdData == CR) || (cmdData == LF)) {
                        writeByte(lcd_isDisplayOn() ? '+' : '-');
                        data = SUCCESS;
                    } else {
                        if (readNothing()) {
                            if (cmdData == '+') {
                                lcd_setDisplayOn(1);
                                data = SUCCESS;
                            } else if (cmdData == '-') {
                                lcd_setDisplayOn(0);
                                data = SUCCESS;
                            }
                        }
                    }
                } break;

                case '$': { //set cursor
                    unsigned char cmdData = readByte();
                    if ((cmdData == CR) || (cmdData == LF)) {
                        writeByte(lcd_isCursorOn() ? '+' : '-');
                        data = SUCCESS;
                    } else {
                        if (readNothing()) {
                            if (cmdData == '+') {
                                lcd_setCursorOn(1);
                                data = SUCCESS;
                            } else if (cmdData == '-') {
                                lcd_setCursorOn(0);
                                data = SUCCESS;
                            }
                        }
                    }
                } break;

                case '!': { //set cursor blink
                    unsigned char cmdData = readByte();
                    if ((cmdData == CR) || (cmdData == LF)) {
                        writeByte(lcd_isCursorBlink() ? '+' : '-');
                        data = SUCCESS;
                    } else {
                        if (readNothing()) {
                            if (cmdData == '+') {
                                lcd_setCursorBlinkOn(1);
                                data = SUCCESS;
                            } else if (cmdData == '-') {
                                lcd_setCursorBlinkOn(0);
                                data = SUCCESS;
                            }
                        }
                    }
                } break;

                case 'b':
                case 'B': { //set backlight
                    unsigned char percent = settings_getBacklight();
                    unsigned char charCount;
                    if (readPercent(&percent, &charCount)) {
                        if (charCount == 0) { //report back percentage
                            writePercent(percent);
                        } else {
                            lcd_setBacklightPwm(percent);
                            settings_setBacklight(percent);
                        }
                        data = SUCCESS;
                        if (cmd == 'B') { settings_writeBacklight(); }
                    }
                } break;

                case 'c':
                case 'C': { //set contrast
                    unsigned char percent = settings_getContrast();
                    unsigned char charCount;
                    if (readPercent(&percent, &charCount)) {
                        if (charCount == 0) { //report back percentage
                            writePercent(percent);
                        } else {
                            lcd_setContrastPwm(percent);
                            settings_setContrast(percent);
                        }
                        data = SUCCESS;
                        if (cmd == 'C') { settings_writeContrast(); }
                    }
                } break;

                case 'd':
                case 'D': { //set number of devices (control of E1 and E2)
                    unsigned char deviceCount = settings_getDeviceCount();
                    unsigned char charCount;
                    if (readSmallNumber(&deviceCount, &charCount)) {
                        if (charCount == 0) { //report back
                            writeNumber(deviceCount);
                            data = SUCCESS;
                        } else if (deviceCount == 1) {
                            settings_setInterface(1);
                            data = SUCCESS;
                        } else if (deviceCount == 2) {
                            settings_setDeviceCount(2);
                            data = SUCCESS;
                        }
                        if (data == SUCCESS) {
                            lcd_reinit(settings_getInterface(), settings_getDeviceCount());
                            if (cmd == 'D') { settings_writeDeviceCount(); }
                        }
                    }
                } break;


                case 'i':
                case 'I': { //set interface width
                    unsigned char interface = settings_getInterface();
                    unsigned char charCount;
                    if (readSmallNumber(&interface, &charCount)) {
                        if (charCount == 0) { //report back
                            writeNumber(interface);
                            data = SUCCESS;
                        } else if (interface == 4) {
                            settings_setInterface(4);
                            data = SUCCESS;
                        } else if (interface == 8) {
                            settings_setInterface(8);
                            data = SUCCESS;
                        }
                        if (data == SUCCESS) {
                            lcd_reinit(settings_getInterface(), settings_getDeviceCount());
                            if (cmd == 'I') { settings_writeInterface(); }
                        }
                    }
                } break;

                default: readNothing(); break;
            }
        } break;

        case LF:   //Next line
        case CR: { //Next line
            if (lcd_nextLine()) {
                data = SUCCESS;
            } else {
                data = CR; //cannot move to next line
            }
        } break;

        case VT: { //Clear display
            lcd_clearDisplay();
            data = SUCCESS;
        } break;

        case FF: { //LCD instruction mode
            unsigned char instruction = readByte();
            lcd_writeInstruction(instruction);
            data = SUCCESS;
        } break;

        case SO: { //Select secondary display
            lcd_useE(0x02);
            data = SUCCESS;
        } break;

        case SI: { //Select primary display
            lcd_useE(0x01);
            data = SUCCESS;
        } break;

        default:
            lcd_writeData(data);
            break;
    }

    writeByte(data); //echo
}
