#include "config.h"
#include <pic.h>


void init() {
    GIE = 0; //disable interrupts

    IRCF3 = 1; //16 MHz oscillator
    IRCF2 = 1;
    IRCF1 = 1;
    IRCF0 = 1;

    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0b00011000;
    TRISA = 0b01100000; // D7 -- -- D4 D3 D2 D1 D0
    TRISB = 0b11000000; // -- -- RS RW E1 D7 D6 D5
    TRISC = 0b11111110; // RX TX -- DA CL BL CT E2
    LATA = 0;
    LATB = 0;
    LATC = 0;

    PR2 = 0x0F; //250 kHz PWM / 6-bit

    //backlight
    CCP1M3  = 1; //pwm mode (CCP2CON)
    CCP1M2  = 1;
    CCP1M1  = 0;
    CCP1M0  = 0;
    CCPR1L  = PR2; //start with max
    DC2B1   = 1;
    DC2B0   = 1;

    //contrast
    CCP2M3  = 1; //pwm mode
    CCP2M2  = 1;
    CCP2M1  = 0;
    CCP2M0  = 0;
    CCPR2L  = 0x00; //start with 0
    DC2B1   = 0;
    DC2B0   = 0;

    //pwm timer
    TMR2IF  = 0; //clear interrupt
    T2CKPS1 = 0; //set prescaler (1:1)
    T2CKPS0 = 0;
    TMR2ON  = 1; //enable timer
    while (!TMR2IF); //wait until timer overflows (to start at complete duty cycle)

    TRISC1 = 0; //turn on contrast
    TRISC2 = 0; //turn on backlight
}
