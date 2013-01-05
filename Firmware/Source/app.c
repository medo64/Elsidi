#include <pic16f1936.h>
#include <pic.h>

#include "config.h"

#include "eeprom.h"
#include "uart.h"

#include "settings.h"
#include "lcd.h"


#define LED    LATC0


void init() {
    GIE = 0; //disable interrupts

    IRCF3 = 1; //8 MHz oscillator (maximum for PLL)
    IRCF2 = 1;
    IRCF1 = 1;
    IRCF0 = 0;

    ANSELA = 0;
    ANSELB = 0;
    TRISA = 0b00000001; // D7 D6 D5 D4 D3 D2 D1 --
    TRISB = 0b11000010; // -- -- E1 RW RS E2 -- D0
    TRISC = 0b10111110; // RX TX -- -- -- CT BL AC
    LATA = 0;
    LATB = 0;
    LATC = 0;

    PR2 = 0xFF;  //31.25 kHz PWM

    //contast
    STR1A   = 1; //P1A is output
    CCP1M3  = 1; //pwm mode (CCP2CON)
    CCP1M2  = 1;
    CCP1M1  = 0;
    CCP1M0  = 0;
    CCPR1L  = 0; //start with 0
    DC2B1   = 0;
    DC2B0   = 0;

    //backlight
    STR2A   = 1; //P2A is output
    CCP2M3  = 1; //pwm mode
    CCP2M2  = 1;
    CCP2M1  = 0;
    CCP2M0  = 0;
    CCPR2L  = 0xFF; //start with max
    DC2B1   = 1;
    DC2B0   = 1;

    //pwm timer
    TMR2IF  = 0; //clear interrupt
    T2CKPS1 = 0; //set prescaler (1:1)
    T2CKPS0 = 0;
    TMR2ON  = 1; //enable timer
    while (!TMR2IF); //wait until timer overflows (to start at complete duty cycle)

    TRISC2 = 0; //turn on contrast
    TRISC1 = 0; //turn on backlight
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
        unsigned char data = uart_readByte();
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
                    if ((*value < 10) || ((*value == 10) && (data == 0))) {
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

bit readNothing() {
    unsigned char isValid = 1;
    while(1) {
        unsigned char data = uart_readByte();
        if ((data==0x0A) || (data==0x0D)) { break; } //both LF and CR are supported
        isValid = 0;
    }
    return (isValid) ? 1 : 0;
}

void writePercent(unsigned char value) {
    unsigned char d1 = value / 100;
    unsigned char d2 = (value / 10) % 10;
    unsigned char d3 = value % 10;
    if (d1 == 0) {
        if (d2 == 0) {
            uart_writeByte('0' + d3);
        } else {
            uart_writeByte('0' + d2);
            uart_writeByte('0' + d3);
        }
    } else {
        uart_writeByte('0' + d1);
        uart_writeByte('0' + d2);
        uart_writeByte('0' + d3);
    }
}

const unsigned char ELSIDI_NAME[] = "Elsidi\0";
const unsigned char ELSIDI_URL[] = "www.jmedved.com\0";
const unsigned char ELSIDI_VERSION[] = "Elsidi K 2013-01-02\0";


void main() {
    init();
    settings_init();    
    uart_init(9600);

    
    LED = 1;
    lcd_init();

    int i = 0;
    while (ELSIDI_NAME[i] != '\0') {
        lcd_writeData(ELSIDI_NAME[i]);
        i++;
    }
    i = 0;
    lcd_setAddress(0x40); //line2
    while (ELSIDI_URL[i] != '\0') {
        lcd_writeData(ELSIDI_URL[i]);
        i++;
    }
    lcd_returnHome();

    lcd_setContrastPwm(settings_getContrast());
    lcd_setBacklightPwm(settings_getBacklight());

    __delay_ms(1000);
    lcd_clearDisplay();
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
                            int i = 0;
                            while (ELSIDI_VERSION[i] != '\0') {
                                uart_writeByte(ELSIDI_VERSION[i]);
                                i++;
                            }
                            data = 0x0A; //valid multiline command will result in LF.
                        }
                    } break;

                    case '~': { //restore defaults
                        if (readNothing()) {
                            settings_setContrast(50);
                            settings_setBacklight(1);
                            settings_writeContrast();
                            settings_writeBacklight();
                            lcd_useE(0x03);
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
