#pragma config FOSC     = INTOSC
#pragma config WDTE     = OFF
#pragma config PWRTE    = ON
#pragma config MCLRE    = OFF
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


#define bitSet(var, bitno)    ((var) |= 1UL << (bitno))
#define bitClear(var, bitno)  ((var) &= ~(1UL << (bitno)))


void init() {
    GIE = 0; //disable interrupts

    IRCF3 = 1; //8 MHz oscillator (maximum for PLL)
    IRCF2 = 1;
    IRCF1 = 1;
    IRCF0 = 0;

    ANSELA = 0;
    ANSELB = 0;
    TRISA = 0b11111111; // D7 D6 D5 D4 D3 D2 D1 --
    TRISB = 0b11000010; // -- -- E1 RW RS E2 -- D0
    TRISC = 0b10111110; // RX TX -- -- -- CT BL AC
    LATA = 0;
    LATB = 0;
    LATC = 0;

    PR2 = 0x3F; //125 kHz PWM
    CCP1CON = 0; //contrast
    CCPR1L = 0;  //start with 0 kHz
    CCP2CON = 0; //backlight
    CCPR2L = 0;  //start with 0 kHz
    TMR2IF = 0; //clear interrupt
    T2CKPS1 = 0; //set prescaler
    T2CKPS0 = 0;
    TMR2ON = 1; //enable timer
    while (TMR2IF == 0); //wait until timer overflows (to start at complete duty cycle)

    TRISC1 = 0; //turn on contrast
    TRISC2 = 0; //turn on backlight
}
