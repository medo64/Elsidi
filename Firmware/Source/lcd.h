#ifndef LCD_H
#define LCD_H

void lcd_clearDisplay();
void lcd_init(unsigned char busWidth, unsigned char width, unsigned char height);
void lcd_reinit(unsigned char busWidth, unsigned char width, unsigned char height);
bit lcd_nextLine();
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
