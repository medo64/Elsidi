#ifndef CONFIG_H
#define CONFIG_H


#pragma config FOSC     = INTOSC
#pragma config WDTE     = OFF
#pragma config PWRTE    = ON
#pragma config MCLRE    = ON
#pragma config CP       = OFF
#pragma config CPD      = OFF
#pragma config BOREN    = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO     = OFF
#pragma config FCMEN    = ON

#pragma config WRT      = OFF
#pragma config VCAPEN   = RA0
#pragma config PLLEN    = ON
#pragma config STVREN   = ON
#pragma config BORV     = LO
#pragma config LVP      = OFF


#define _XTAL_FREQ 32000000


#define LCD_RS LATB3
#define LCD_RW LATB4
#define LCD_E1 LATB5
#define LCD_E2 LATB2
#define LCD_D0 LATB0
#define LCD_D1 LATA1
#define LCD_D2 LATA2
#define LCD_D3 LATA3
#define LCD_D4 LATA4
#define LCD_D5 LATA5
#define LCD_D6 LATA6
#define LCD_D7 LATA7

#define LCD_D0_TRIS TRISB0
#define LCD_D0_PORT PORTBbits.RB0

#define LED    LATC0


const unsigned char ELSIDI_NAME[] = "Elsidi\0";
const unsigned char ELSIDI_URL[] = "www.jmedved.com\0";
const unsigned char ELSIDI_VERSION[] = "Elsidi K 2013-01-14\0";


void init();


#endif
