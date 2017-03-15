/*
2007-2-1 12:06
nokia 5110 driver program for 51 mcu
by zl0801

zhaoliang0801@gmail.com

*/

#include <reg51.h>

// pin define for n5110lcd_8key board
// change this if your hardware changed!

sbit SCLK = P2^4;		// pin 2	 header	5
sbit SDIN = P2^3;		// pin 3	 header	4
sbit LCD_DC = P2^2;		// pin 4	 header	3
sbit LCD_CE = P2^1;		// pin 5	 header	2
sbit LCD_RST = P2^0;	// pin 9	 header	1


void LCD_init(void);
void LCD_clear(void);
void LCD_move_chinese_string(unsigned char X, unsigned char Y, unsigned char T); 
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
void LCD_write_chinese_string(unsigned char X, unsigned char Y,
                   unsigned char ch_with,unsigned char num,
                   unsigned char line,unsigned char row);
void chinese_string(unsigned char X, unsigned char Y, unsigned char T);                 
void LCD_write_char(unsigned char c);
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,unsigned char *map,
                  unsigned char Pix_x,unsigned char Pix_y);
void LCD_write_byte(unsigned char dat, unsigned char dc);
void LCD_set_XY(unsigned char X, unsigned char Y);
void delay_1us(void);                 
 