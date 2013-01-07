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


bit readSmallNumber(unsigned char *value, unsigned char *charCount) { //0 to 100
    *charCount = 0;
    unsigned char isValid = 1;
    unsigned char newValue = 0;
    while(1) {
        unsigned char data = readByte();
        if ((data==0x0A) || (data==0x0D)) { break; } //both LF and CR are supported
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
        case 0x08: { //BS: Command mode
            unsigned char cmd = readByte();
            switch (cmd) {
                case 0x0A:
                case 0x0D: { //clear display
                    lcd_clearDisplay();
                    data = 0x0A; //valid text command will result in LF.
                } break;

                case '?': { //ID
                    if (readNothing()) {
                        int i = 0;
                        while (ELSIDI_VERSION[i] != '\0') {
                            writeByte(ELSIDI_VERSION[i]);
                            i++;
                        }
                        data = 0x0A; //valid text command will result in LF.
                    }
                } break;

                case '~': { //restore startup defaults
                    if (readNothing()) {
                        settings_init();
                        lcd_setBacklightPwm(settings_getBacklight());
                        lcd_setContrastPwm(settings_getContrast());
                        lcd_reinit(settings_getInterface());
                        lcd_useE(0x03); //use both connectors for same output
                        data = 0x0A; //valid text command will result in LF.
                    }
                } break;

                case '*': { //set display
                    unsigned char cmdData = readByte();
                    if ((cmdData == 0x0A) || (cmdData == 0x0D)) {
                        writeByte(lcd_isDisplayOn() ? '+' : '-');
                        data = 0x0A; //valid text command will result in LF.
                    } else {
                        if (readNothing()) {
                            if (cmdData == '+') {
                                lcd_setDisplayOn(1);
                                data = 0x0A; //valid text command will result in LF.
                            } else if (cmdData == '-') {
                                lcd_setDisplayOn(0);
                                data = 0x0A; //valid text command will result in LF.
                            }
                        }
                    }
                } break;

                case '$': { //set cursor
                    unsigned char cmdData = readByte();
                    if ((cmdData == 0x0A) || (cmdData == 0x0D)) {
                        writeByte(lcd_isCursorOn() ? '+' : '-');
                        data = 0x0A; //valid text command will result in LF.
                    } else {
                        if (readNothing()) {
                            if (cmdData == '+') {
                                lcd_setCursorOn(1);
                                data = 0x0A; //valid text command will result in LF.
                            } else if (cmdData == '-') {
                                lcd_setCursorOn(0);
                                data = 0x0A; //valid text command will result in LF.
                            }
                        }
                    }
                } break;

                case '!': { //set cursor blink
                    unsigned char cmdData = readByte();
                    if ((cmdData == 0x0A) || (cmdData == 0x0D)) {
                        writeByte(lcd_isCursorBlink() ? '+' : '-');
                        data = 0x0A; //valid text command will result in LF.
                    } else {
                        if (readNothing()) {
                            if (cmdData == '+') {
                                lcd_setCursorBlinkOn(1);
                                data = 0x0A; //valid text command will result in LF.
                            } else if (cmdData == '-') {
                                lcd_setCursorBlinkOn(0);
                                data = 0x0A; //valid text command will result in LF.
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
                        data = 0x0A; //valid text command will result in LF.
                        if (cmd == 'C') { settings_writeBacklight(); }
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
                        data = 0x0A; //valid text command will result in LF.
                        if (cmd == 'C') { settings_writeContrast(); }
                    }
                } break;

                case 'i':
                case 'I': { //set interface width
                    unsigned char interface = settings_getInterface();
                    unsigned char charCount;
                    if (readSmallNumber(&interface, &charCount)) {
                        if (charCount == 0) { //report back
                            writeNumber(interface);
                            data = 0x0A; //valid text command will result in LF.
                        } else if (interface == 4) {
                            settings_setInterface(4);
                            lcd_reinit(4);
                            data = 0x0A; //valid text command will result in LF.
                        } else if (interface == 8) {
                            settings_setInterface(8);
                            lcd_reinit(8);
                            data = 0x0A; //valid text command will result in LF.
                        }
                        if ((cmd == 'I') && (data == 0x0A)) {
                            settings_writeInterface();
                        }
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
