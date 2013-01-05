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


const unsigned char ELSIDI_NAME[] = "Elsidi\0";
const unsigned char ELSIDI_URL[] = "www.jmedved.com\0";
const unsigned char ELSIDI_VERSION[] = "Elsidi K 2013-01-05\0";

#endif
