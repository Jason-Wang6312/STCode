#ifndef __12864_H__
#define __12864_H__
#define Dis_Y_MAX 64-1
#define Dis_X_MAX 132-1
 /************************************************12864引脚配置*********************************************************/
//液晶接口
sbit SI =P0^5;	  				//串行数据输入引脚
sbit SCL=P0^1;	  				//串行时钟信号引脚
sbit A0 =P0^2;	  				//命令或者数据选择引脚
sbit RES=P0^3;					//复位引脚
sbit CS =P0^4;	  				//使能信号引脚，低电平有效
sbit LCD_ON =P0^0;				//控制背光


void InitST7565();
void LCDFill(u8 Data);
void Displayen(u8 page,u8 column,u8 number,bit Reserve);
void Displaych(u8 page,u8 column,u8 number,u8 *name,bit Reserve);

void displaychinese();
void displayhe();
void displaykai();
void displayok();
void displayno();
#endif