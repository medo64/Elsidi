#include "config.h"

#ifndef LCD_H
#define LCD_H

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

void lcd_clearDisplay();
void lcd_init(unsigned char width);
void lcd_reinit(unsigned char width);
void lcd_nextLine();
void lcd_returnHome();
void lcd_setAddress(unsigned char address);
void lcd_setBacklightPwm(unsigned char percent);
void lcd_setContrastPwm(unsigned char percent);
void lcd_useE(unsigned char mask);
void lcd_writeData(unsigned char data);
void lcd_writeInstruction(unsigned char data);

bit lcd_isDisplayOn();
bit lcd_isCursorOn();
bit lcd_isCursorBlink();

void lcd_setDisplayOn(unsigned char state);
void lcd_setCursorOn(unsigned char state);
void lcd_setCursorBlinkOn(unsigned char state);

#endif
