/*****************************************************************************
 *版权信息：深圳天微电子有限公司
 *文 件 名：TM1637-V1.0
 *当前版本：V1.0
 *MCU 型号：STC12C5608AD
 *开发环境：Keil uVision4
 *晶震频率：11.0592MHZ       
 *完成日期：2013-07-19
 *程序功能：数码管驱动和按键：驱动8段6位LED共阳数码管显示,当对应按键按下时显示1~7,原理图请参考TM1637规格书；          
 *免责声明：1.此程序为TM1637驱动共阳LED数码管和按键演示程序，仅作参考之用。
            2.如有直接使用本例程程序造成经济损失的，本公司不承担任何责任             
********************************************************************************/
#include <STC15Wxx.h>                                                //头文件
#include "intrins.h"                                                //包含_nop_()指令头文件

#define nop _nop_();_nop_();_nop_();_nop_();_nop_();                 //宏定义


/********************定义控制端口**********************/
sbit CLK=P5^4;                                                                 //定义CLK
sbit DIO=P5^5;                                                                 //定义DIO


/********************定义数据*************************/
unsigned char code CODE[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8, 0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e}; //共阳显示数据0-F
                                                        /* 0   1    2    3    4     5    6    7     8    9   a     b    c    d    e    f*/
unsigned char code TAB[10]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};//数码管不显示

/********************延时函数，延时nms******************/
void delay_nms(unsigned int n)
{
 unsigned int i;
 while(n--)
   for(i=0;i<550;i++);
}

/********************Start函数*************************/
void I2CStart()
{
 DIO=1;
 CLK=1;
 nop;
 DIO=1;
 nop;
 DIO=0;
 nop;
 CLK=0;
}

/********************Stop函数*************************/
void I2CStop()
{
    CLK=0;
        nop;
        nop;
        DIO=0;
        nop;
        nop;
        CLK=1;
        nop;
        nop;
        nop;
        DIO=1;
        nop;
        CLK=0;
        DIO=0;
}

/***************发送8bit数据，从低位开始**************/
void I2CWritebyte(unsigned char oneByte)
{
  unsigned char i;
  for(i=0;i<8;i++)
  {
    CLK=0;
        if(oneByte&0x01) 
          DIO=1;
        else 
          DIO=0;
        nop;
    CLK=1;
    oneByte=oneByte>>1;
  }
                                                                                  //8位数据传送完                 
        CLK = 0;                                                                //判断芯片发过来的ACK应答信号
        nop;
        while(DIO==1);
        nop;
        CLK = 1;
        nop;
}

/***************读按键程序**************/
unsigned char read_key()
{
unsigned char rekey,i;
I2CStart();
I2CWritebyte(0x42);                                                         //写读键指令0x42
DIO=1;
for(i=0;i<8;i++)
{
        CLK=0;
        nop;
        nop;
        rekey=rekey>>1;                                                           //先读低位
        nop;
        nop;
        CLK=1;
        if(DIO) 
          rekey=rekey|0x80;
        else 
          rekey=rekey|0x00;
        nop;
}
        CLK = 0;                                                          //判断芯片发过来的ACK应答信号
        nop;
        nop;
        while(DIO==1);
        nop;
        nop;
        CLK = 1;
        nop;
        nop;
        I2CStop();
        
        return rekey;
}

/************显示函数，地址自加一************/
void disp0(unsigned char *p)                              
{
 unsigned char i;
 I2CStart();
 I2CWritebyte(0x40);                                  //数据命令设置：地址自动加1，写数据到显示寄存器
 I2CStop();

 I2CStart();
 I2CWritebyte(0xc0);                                  //地址命令设置：初始地址为00H
 for(i=0;i<6;i++)                                  //发送4字节数据到显存
 {
 I2CWritebyte(*p);
 p++;
 }
 I2CStop();

 I2CStart();
 I2CWritebyte(0x8C);                                 //显示控制命令：开显示，脉冲宽度为11/16.
 I2CStop();

}

/************显示函数，固定地址写数据************/
void disp(unsigned char add, unsigned char value)
{
 I2CStart();
 I2CWritebyte(0x44);                                 //数据命令设置：固定地址，写数据到显示寄存器
 I2CStop();

 I2CStart();
 I2CWritebyte(add);                                //地址命令设置：写入add对应地址

 I2CWritebyte(CODE[value]);                        //给add地址写数据
 I2CStop();

 I2CStart();
 I2CWritebyte(0x8C);                                //显示控制命令：开显示，脉冲宽度为11/16.
 I2CStop();

}

/************按键处理函数，按键数据低位在前高位在后************/
void key_process()
{
 unsigned char temp;
 temp=read_key();                                   //读取按键返回值
 if(temp!=0xff)
 {
  disp0(TAB);                                           //清屏
  switch(temp)
  {
    case 0xf7 : disp(0xc0,1);break;                  //K1与SG1对应按键按下,显示1
        case 0xf6 : disp(0xc1,2);break;                  //K1与SG2对应按键按下,显示2
        case 0xf5 : disp(0xc2,3);break;                  //K1与SG3对应按键按下,显示3
        case 0xf4 : disp(0xc3,4);break;                  //K1与SG4对应按键按下,显示4
        case 0xef : disp(0xc4,5);break;                  //K2与SG1对应按键按下,显示5
        case 0xee : disp(0xc5,6);break;                  //K2与SG2对应按键按下,显示6
        case 0xed : disp(0xc0,7);break;                  //K2与SG3对应按键按下,显示7
        default   : break;
  }
 }
}

void main()
{
　　disp0(CODE);                                      //上电数码管显示0~5
　　delay_nms(1);
　　while(1)                                     //按键后显示按键内容
　　{
//　　　　key_process();
//　　　　delay_nms(100);
　　}
}