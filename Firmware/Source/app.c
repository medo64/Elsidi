#include <pic16f1936.h>
#include <pic.h>

#include "communication.h"
#include "config.h"
#include "eeprom.h"
#include "lcd.h"
#include "settings.h"


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


void main() {
    init();
    settings_init();
    communication_init();

    
    LED = 1;
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
