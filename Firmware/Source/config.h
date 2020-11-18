#ifndef CONFIG_H
#define CONFIG_H


#pragma config FOSC     = INTOSC
#pragma config WDTE     = ON
#pragma config PWRTE    = ON
#pragma config MCLRE    = OFF
#pragma config CP       = OFF
#pragma config BOREN    = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO     = OFF
#pragma config FCMEN    = ON

#pragma config WRT      = OFF
#pragma config VCAPEN   = ON
#pragma config STVREN   = ON
#pragma config BORV     = HI
#pragma config LPBOR    = ON
#pragma config LVP      = OFF


#define _XTAL_FREQ 16000000


#define LCD_RS LATB5
#define LCD_RW LATB4
#define LCD_E1 LATB3
#define LCD_E2 LATC0
#define LCD_D0 LATA0
#define LCD_D1 LATA1
#define LCD_D2 LATA2
#define LCD_D3 LATA3
#define LCD_D4 LATA4
#define LCD_D5 LATB0
#define LCD_D6 LATB1
#define LCD_D7 LATB2

#define LCD_D0_TRIS TRISA0
#define LCD_D0_PORT PORTAbits.RA0

#define LCD_CONTRAST_TRIS TRISC1
#define LCD_BACKLIGHT_TRIS TRISC2

#define LED    LATA7


const unsigned char ELSIDI_NAME[] = "Elsidi\0";
const unsigned char ELSIDI_URL[] = "www.jmedved.com\0";
const unsigned char ELSIDI_VERSION[] = "Elsidi L 2013-03-13\0";


void init();


#endif
