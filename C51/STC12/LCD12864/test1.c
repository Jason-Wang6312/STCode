#include <reg51.h>
#include <intrins.h>
#define uchar unsigned char
#define uint  unsigned int
#define LCD_data  P0       //数据口
sbit LCD_RS  =  P2^6;      //寄存器选择输入
sbit LCD_RW  =  P2^5;      //液晶读/写控制
sbit LCD_EN  =  P2^7;      //液晶使能控制
sbit LCD_PSB =  P3^2;      //串/并方式控制
sbit LCD_RST =  P3^4;      //液晶复位端口
uchar code  DIS1[] = {"朝辞白帝彩云间，"};
uchar code  DIS2[] = {"千里江陵一日还，"};
uchar code  DIS3[] = {"两岸猿声啼不住，"};
uchar code  DIS4[] = {"轻舟已飞万重山。"};
void delay(int ms)
{
    while(ms--)
 {
      uchar i;
   for(i=0;i<250;i++) 
    {
     _nop_();     
  _nop_();
  _nop_();
  _nop_();
    }
 }
}    
void lcd_wcmd(uchar cmd)
{                         
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 1;
 delay(5);
    LCD_data=cmd;
 delay(5);
    LCD_EN = 0; 
}
void lcd_wdat(uchar dat)
{                         
  
    LCD_RS=1;
    LCD_RW=0;
 LCD_EN=1;
 delay(5);
    P0=dat;
 delay(5);
    LCD_EN = 0;
}
void lcd_init()
{
    LCD_PSB=1;         //并口方式
    LCD_EN=0;
    LCD_RST=0;
    delay(5);
   LCD_RST=1;
    lcd_wcmd(0x34);      //扩充指令操作
    delay(5);
    lcd_wcmd(0x30);      //基本指令操作
    delay(5);
   lcd_wcmd(0x01);      //清除LCD的显示内容
    delay(5);
    lcd_wcmd(0x0c);      //显示开，关光标
    delay(5);
 lcd_wcmd(0x06);     
    delay(5);
   
}
/*********************************************************/
/*                                                       */
/* 设定显示位置                                          */
/*                                                       */
/*********************************************************/
void lcd_pos(uchar X,uchar Y)
{                         
   uchar pos;
   if (X==1)
     {X=0x80;}
   else if (X==2)
     {X=0x90;}
   else if (X==3)
     {X=0x88;}
   else if (X==4)
     {X=0x98;}
   pos=X+Y ;
   lcd_wcmd(pos);     //显示地址
}
void main()
{
   uchar i;
   delay(100);                 //上电，等待稳定
   lcd_init();                 //初始化LCD
    while(1)
 {
     lcd_pos(1,0);             //设置显示位置为第一行
     for(i=0;i<16;i++)
     {
       lcd_wdat(DIS1[i]);
       delay(30);
     }
  delay(50);
     lcd_pos(2,0);             //设置显示位置为第二行
     for(i=0;i<16;i++)
     {
       lcd_wdat(DIS2[i]);
       delay(30);
     }
  delay(5);
     lcd_pos(3,0);             //设置显示位置为第三行
     for(i=0;i<16;i++)
     {
       lcd_wdat(DIS3[i]);
       delay(30);
     }
 delay(50);
     lcd_pos(4,0);             //设置显示位置为第四行
     for(i=0;i<16;i++)
     {
       lcd_wdat(DIS4[i]);
       delay(30);
     }
     delay(1000);
   }
}