#ifndef __LCD12864_UC1701X_H
#define __LCD12864_UC1701X_H
#include "STC15F2K.H"

#define LCDReverse 0 //正反显示 1.为正 0.为负
#define LCDSize 0 //显示屏大小 1.大屏 0.小屏

/******** define pin **********/


sbit SPI_SCLK = P1^1;
sbit SPI_SDA  = P1^0;
sbit SPI_RS   = P0^7;
sbit SPI_REST = P0^6;
sbit SPI_CS   = P0^5;
sbit LEDA     = P2^5;

/*********函数声明**********/

//uc1701x 显示屏初始化
void initial_LCD();
//清屏
void clearScreen();
//全屏填充
void fullDisplay();
//led光标定位
void location(unsigned char page,unsigned char col);
//显示8x16 点阵图像、汉字
void graphic_8x16(unsigned char page,unsigned char col,unsigned char *dp);
//显示16x16 点阵图像、汉字
void graphic_16x16(unsigned char page,unsigned char col,unsigned char *dp);
//显示32x32 点阵图像、汉字
void graphic_32x32(unsigned char page,unsigned char col,unsigned char *dp);
//显示ascii字符 5x7
void displayStr_5x7(unsigned char page,unsigned char col,unsigned char *str);
//显示ascii字符 8x16
void displayStr_8x16(unsigned char page,unsigned char col,unsigned char *str);
//清处指定区域
void clearXYArea(unsigned char x,unsigned char y,unsigned char length,unsigned char width);

/**********变量定义***********/

#endif
