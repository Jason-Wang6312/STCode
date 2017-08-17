/******************************* 
 **** Describe: TM1673控制芯片，可以设置时间的倒计时(定时不准，可以自己校准)，四个数码管0xc0,0xc1,0xc2,0xc3 
 ****     Time: 2015.05.14 
 ****   Author: zys 
 ********************************/  
#include "STC15Wxx.H"  
#include<intrins.h>  
  
sbit CLK = P5^4;  //时钟信号  
sbit DIO = P5^5;  //数据/地址数据  
                                                                                 //共阴极数码管  
unsigned char code SEGData[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//1-9  
unsigned char code SEGDataDp[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef}; //有小数点只用于地址0xc1  
  
unsigned char dpFlag = 0; //控制第二个数码管的dp的显示  
  
void TM1637_start( void );  
void TM1637_stop( void );  
void TM1637_write1Bit(unsigned char mBit);  
void TM1637_write1Byte(unsigned char mByte);  
void TM1637_writeCammand(unsigned char mData);  
void TM1637_writeData(unsigned char addr, unsigned char mData);  
void time_set(char hour, char min);  
void time_judge( void );  
void time_display( void );  
void timer0_init( void );  
  
void delay_140us( void );  
void delay_1ms(unsigned int i);  
  
unsigned int countTime = 0;  
unsigned char time, sec;  
struct {  
    char shi;  
    char ge;  
}Hour;  
  
struct {  
    char shi;  
    char ge;  
}Min;  
  
/*********************************************************** 
***** 
***** 主函数 
***** 
***********************************************************/  
void main( void )  
{  
    timer0_init();  
    time_set(5, 0);  //设置5小时倒计时  
    while(1)  
    {  
        time_display();  //显示时间  
        if(countTime <= 10)  
        {  
            dpFlag = 0;           
        }  
        else  
        if(countTime <= 20)  
        {  
            dpFlag = 1;   
        }  
        else  
        {  
            countTime = 0;  
            time_judge();   
        }  
    }  
}  
  
/******************************************************************** 
* 名称 : void time_set(char hour, char min) 
* 功能 : 设置时间 
* 输入 : char hour, char min 
* 输出 : 无 
**************************************************************/  
void time_set(char hour, char min)  
{  
    if((hour < 0) || (min<0))  
    {  
        hour = 0;  
        min = 0;  
    }  
    Hour.shi = hour/10;  
    Hour.ge = hour%10;  
    Min.shi = min/10;  
    Min.ge = min%10;          
}  
  
/************************************************************* 
* 名称 : time_judge( void ) 
* 功能 : 判断时间变化 
* 输入 : void 
* 输出 : 无 
**************************************************************/  
void time_judge( void )  
{  
    if(++sec == 60)    //60秒  
    {     
        sec = 0;  
        if((Hour.shi || Hour.ge || Min.shi || Min.ge) && (--Min.ge <= -1))  
        {  
            Min.ge = 9;  
            if((Hour.shi) ||(Hour.ge) && (--Min.shi <= -1))  
            {  
                Min.shi = 5;  
                if(Hour.shi && (--Hour.ge <= -1))  
                {  
                    Hour.ge = 9;  
                    if(--Hour.shi <= -1)  
                    {  
                        Hour.shi = 0;     
                    }  
                }  
                else  
                if((!Hour.shi) && (--Hour.ge <= -1))  
                {  
                    Hour.ge = 0;  
                }  
            }  
            else  
            if((!Hour.shi) && (!Hour.ge) && (--Min.shi <= -1))  
            {  
                Min.shi = 0;  
            }  
        }  
        else  
        if(((!Hour.shi) && (!Hour.ge) && (!Min.shi) ) && (Min.ge <= -1))  
        {  
            Min.ge = 0;  
        }  
    }     
}  
  
/******************************************************************** 
* 名称 : void time_display( void ) 
* 功能 : 显示时间 
* 输入 : void 
* 输出 : 无 
**************************************************************/  
void time_display( void )  
{  
    TM1637_writeCammand(0x44);  
    TM1637_writeData(0xc0, SEGData[Hour.shi]);  
    if(dpFlag)  
        TM1637_writeData(0xc1, SEGDataDp[Hour.ge]); //小数点标志为1则用小数点那个数组   
    else  
        TM1637_writeData(0xc1, SEGData[Hour.ge]);  
    TM1637_writeData(0xc2, SEGData[Min.shi]);  
    TM1637_writeData(0xc3, SEGData[Min.ge]);  
    TM1637_writeCammand(0x8a);                
}  
  
/******************************************************************** 
* 名称 : void TM1637_start( void ) 
* 功能 : start信号 
* 输入 : void 
* 输出 : 无 
**************************************************************/  
void TM1637_start( void )  
{  
    CLK = 1;  
    DIO = 1;  
    delay_140us();  
    DIO = 0;  
    delay_140us();  
    CLK = 0;  
    delay_140us();  
}  
  
/******************************************************************** 
* 名称 : void TM1637_stop( void ) 
* 功能 : stop信号 
* 输入 : void 
* 输出 : 无 
**************************************************************/  
void TM1637_stop( void )  
{  
    CLK = 0;  
    delay_140us();  
    DIO = 0;  
    delay_140us();  
    CLK = 1;  
    delay_140us();  
    DIO = 1;  
    delay_140us();  
}  
  
/******************************************************************** 
* 名称 : void TM1637_write1Bit(unsigned char mBit ) 
* 功能 : 写1bit 
* 输入 : unsigned char mBit 
* 输出 : 无 
**************************************************************/  
void TM1637_write1Bit(unsigned char mBit )  
{  
    CLK = 0;  
    delay_140us();  
    if(mBit)  
        DIO = 1;  
    else  
        DIO = 0;  
    delay_140us();    
    CLK = 1;  
    delay_140us();  
}  
  
/******************************************************************** 
* 名称 : void TM1637_write1Byte(unsigned char mByte) 
* 功能 : 写1byte 
* 输入 : unsigned char mByte 
* 输出 : 无 
**************************************************************/  
void TM1637_write1Byte(unsigned char mByte)  
{  
    char loop = 0;  
    for(loop = 0; loop < 8; loop++)  
    {  
        TM1637_write1Bit((mByte>>loop)&0x01); //取得最低位   
    }  
    CLK = 0;  
    delay_140us();  
    DIO = 1;  
    delay_140us();  
    CLK = 1;  
    delay_140us();  
    while(DIO == 1);  //获得应答位      
}  
  
/******************************************************************** 
* 名称 : void TM1637_writeCammand(unsigned char mData) 
* 功能 : 写指令1byte 
* 输入 : unsigned char mData 
* 输出 : 无 
**************************************************************/  
void TM1637_writeCammand(unsigned char mData)  
{  
    TM1637_start();  
    TM1637_write1Byte(mData);  //数据  
    TM1637_stop();    
}  
  
/******************************************************************** 
* 名称 : void TM1637_writeData(unsigned char addr, unsigned char mData) 
* 功能 : 固定地址写数据1byte 
* 输入 : unsigned char addr, unsigned char mData 
* 输出 : 无 
**************************************************************/  
void TM1637_writeData(unsigned char addr, unsigned char mData)  
{  
    TM1637_start();  
    TM1637_write1Byte(addr);  //地址  
    TM1637_write1Byte(mData);  //数据  
    TM1637_stop();    
}  
  
/******************************************************************** 
* 名称 : void timer0_init() 
* 功能 : 定时50ms，实际运行中由于指令运行造成的延时，实际时间肯定大于50ms 
* 输入 : 无 
* 输出 : 无 
**************************************************************/  
void timer0_init( void )  
{  
    TMOD=0X01;  
    TH0=(65535-50000)/256;  
    TL0=(65535-50000)%256;  
    ET0=1;  
    EA=1;  
    TR0=1;    
}           
  
  
/******************************************************************** 
* 名称 :  
* 功能 : 定时50ms，实际运行中由于指令运行造成的延时，实际时间肯定大于50ms 
* 输入 : 无 
* 输出 : 无 
**************************************************************/  
void timer0_isr() interrupt 1  
{  
    TH0=(65535-50000)/256;  //50ms  
    TL0=(65535-50000)%256;  //  
    countTime++;  
}  
  
  
/******************************************************************** 
* 名称 : Delay_1ms(unsigned int i) 
* 功能 : 延时子程序，延时时间为 140us 
* 输入 :  
* 输出 : 无 
**************************************************************/  
void delay_140us( void )  
{  
    int i;  
    for(i=0; i<20; i++)  
      _nop_();  
}  
  
/******************************************************************** 
* 名称 : Delay_1ms(unsigned int i) 
* 功能 : 延时子程序，延时时间为 1ms * x 
* 输入 : x (延时一毫秒的个数) 
* 输出 : 无 
***********************************************************************/  
/* 
void Delay_1ms(unsigned int i)//1ms延时 
{ 
    unsigned char x,j; 
    for(j=0;j<i;j++) 
    for(x=0;x<=148;x++);  
} 
*/  /******************************* 
 **** Describe: TM1673控制芯片，可以设置时间的倒计时(定时不准，可以自己校准)，四个数码管0xc0,0xc1,0xc2,0xc3 
 ****     Time: 2015.05.14 
 ****   Author: zys 
 ********************************/  
#include <reg52.h>  
#include<intrins.h>  
  
sbit CLK = P0^0;  //时钟信号  
sbit DIO = P0^1;  //数据/地址数据  
                                                                                 //共阴极数码管  
unsigned char code SEGData[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//1-9  
unsigned char code SEGDataDp[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef}; //有小数点只用于地址0xc1  
  
unsigned char dpFlag = 0; //控制第二个数码管的dp的显示  
  
void TM1637_start( void );  
void TM1637_stop( void );  
void TM1637_write1Bit(unsigned char mBit);  
void TM1637_write1Byte(unsigned char mByte);  
void TM1637_writeCammand(unsigned char mData);  
void TM1637_writeData(unsigned char addr, unsigned char mData);  
void time_set(char hour, char min);  
void time_judge( void );  
void time_display( void );  
void timer0_init( void );  
  
void delay_140us( void );  
void delay_1ms(unsigned int i);  
  
unsigned int countTime = 0;  
unsigned char time, sec;  
struct {  
    char shi;  
    char ge;  
}Hour;  
  
struct {  
    char shi;  
    char ge;  
}Min;  
  
/*********************************************************** 
***** 
***** 主函数 
***** 
***********************************************************/  
void main( void )  
{  
    timer0_init();  
    time_set(5, 0);  //设置5小时倒计时  
    while(1)  
    {  
        time_display();  //显示时间  
        if(countTime <= 10)  
        {  
            dpFlag = 0;           
        }  
        else  
        if(countTime <= 20)  
        {  
            dpFlag = 1;   
        }  
        else  
        {  
            countTime = 0;  
            time_judge();   
        }  
    }  
}  
  
/******************************************************************** 
* 名称 : void time_set(char hour, char min) 
* 功能 : 设置时间 
* 输入 : char hour, char min 
* 输出 : 无 
**************************************************************/  
void time_set(char hour, char min)  
{  
    if((hour < 0) || (min<0))  
    {  
        hour = 0;  
        min = 0;  
    }  
    Hour.shi = hour/10;  
    Hour.ge = hour%10;  
    Min.shi = min/10;  
    Min.ge = min%10;          
}  
  
/************************************************************* 
* 名称 : time_judge( void ) 
* 功能 : 判断时间变化 
* 输入 : void 
* 输出 : 无 
**************************************************************/  
void time_judge( void )  
{  
    if(++sec == 60)    //60秒  
    {     
        sec = 0;  
        if((Hour.shi || Hour.ge || Min.shi || Min.ge) && (--Min.ge <= -1))  
        {  
            Min.ge = 9;  
            if((Hour.shi) ||(Hour.ge) && (--Min.shi <= -1))  
            {  
                Min.shi = 5;  
                if(Hour.shi && (--Hour.ge <= -1))  
                {  
                    Hour.ge = 9;  
                    if(--Hour.shi <= -1)  
                    {  
                        Hour.shi = 0;     
                    }  
                }  
                else  
                if((!Hour.shi) && (--Hour.ge <= -1))  
                {  
                    Hour.ge = 0;  
                }  
            }  
            else  
            if((!Hour.shi) && (!Hour.ge) && (--Min.shi <= -1))  
            {  
                Min.shi = 0;  
            }  
        }  
        else  
        if(((!Hour.shi) && (!Hour.ge) && (!Min.shi) ) && (Min.ge <= -1))  
        {  
            Min.ge = 0;  
        }  
    }     
}  
  
/******************************************************************** 
* 名称 : void time_display( void ) 
* 功能 : 显示时间 
* 输入 : void 
* 输出 : 无 
**************************************************************/  
void time_display( void )  
{  
    TM1637_writeCammand(0x44);  
    TM1637_writeData(0xc0, SEGData[Hour.shi]);  
    if(dpFlag)  
        TM1637_writeData(0xc1, SEGDataDp[Hour.ge]); //小数点标志为1则用小数点那个数组   
    else  
        TM1637_writeData(0xc1, SEGData[Hour.ge]);  
    TM1637_writeData(0xc2, SEGData[Min.shi]);  
    TM1637_writeData(0xc3, SEGData[Min.ge]);  
    TM1637_writeCammand(0x8a);                
}  
  
/******************************************************************** 
* 名称 : void TM1637_start( void ) 
* 功能 : start信号 
* 输入 : void 
* 输出 : 无 
**************************************************************/  
void TM1637_start( void )  
{  
    CLK = 1;  
    DIO = 1;  
    delay_140us();  
    DIO = 0;  
    delay_140us();  
    CLK = 0;  
    delay_140us();  
}  
  
/******************************************************************** 
* 名称 : void TM1637_stop( void ) 
* 功能 : stop信号 
* 输入 : void 
* 输出 : 无 
**************************************************************/  
void TM1637_stop( void )  
{  
    CLK = 0;  
    delay_140us();  
    DIO = 0;  
    delay_140us();  
    CLK = 1;  
    delay_140us();  
    DIO = 1;  
    delay_140us();  
}  
  
/******************************************************************** 
* 名称 : void TM1637_write1Bit(unsigned char mBit ) 
* 功能 : 写1bit 
* 输入 : unsigned char mBit 
* 输出 : 无 
**************************************************************/  
void TM1637_write1Bit(unsigned char mBit )  
{  
    CLK = 0;  
    delay_140us();  
    if(mBit)  
        DIO = 1;  
    else  
        DIO = 0;  
    delay_140us();    
    CLK = 1;  
    delay_140us();  
}  
  
/******************************************************************** 
* 名称 : void TM1637_write1Byte(unsigned char mByte) 
* 功能 : 写1byte 
* 输入 : unsigned char mByte 
* 输出 : 无 
**************************************************************/  
void TM1637_write1Byte(unsigned char mByte)  
{  
    char loop = 0;  
    for(loop = 0; loop < 8; loop++)  
    {  
        TM1637_write1Bit((mByte>>loop)&0x01); //取得最低位   
    }  
    CLK = 0;  
    delay_140us();  
    DIO = 1;  
    delay_140us();  
    CLK = 1;  
    delay_140us();  
    while(DIO == 1);  //获得应答位      
}  
  
/******************************************************************** 
* 名称 : void TM1637_writeCammand(unsigned char mData) 
* 功能 : 写指令1byte 
* 输入 : unsigned char mData 
* 输出 : 无 
**************************************************************/  
void TM1637_writeCammand(unsigned char mData)  
{  
    TM1637_start();  
    TM1637_write1Byte(mData);  //数据  
    TM1637_stop();    
}  
  
/******************************************************************** 
* 名称 : void TM1637_writeData(unsigned char addr, unsigned char mData) 
* 功能 : 固定地址写数据1byte 
* 输入 : unsigned char addr, unsigned char mData 
* 输出 : 无 
**************************************************************/  
void TM1637_writeData(unsigned char addr, unsigned char mData)  
{  
    TM1637_start();  
    TM1637_write1Byte(addr);  //地址  
    TM1637_write1Byte(mData);  //数据  
    TM1637_stop();    
}  
  
/******************************************************************** 
* 名称 : void timer0_init() 
* 功能 : 定时50ms，实际运行中由于指令运行造成的延时，实际时间肯定大于50ms 
* 输入 : 无 
* 输出 : 无 
**************************************************************/  
void timer0_init( void )  
{  
    TMOD=0X01;  
    TH0=(65535-50000)/256;  
    TL0=(65535-50000)%256;  
    ET0=1;  
    EA=1;  
    TR0=1;    
}           
  
  
/******************************************************************** 
* 名称 :  
* 功能 : 定时50ms，实际运行中由于指令运行造成的延时，实际时间肯定大于50ms 
* 输入 : 无 
* 输出 : 无 
**************************************************************/  
void timer0_isr() interrupt 1  
{  
    TH0=(65535-50000)/256;  //50ms  
    TL0=(65535-50000)%256;  //  
    countTime++;  
}  
  
  
/******************************************************************** 
* 名称 : Delay_1ms(unsigned int i) 
* 功能 : 延时子程序，延时时间为 140us 
* 输入 :  
* 输出 : 无 
**************************************************************/  
void delay_140us( void )  
{  
    int i;  
    for(i=0; i<20; i++)  
      _nop_();  
}  
  
/******************************************************************** 
* 名称 : Delay_1ms(unsigned int i) 
* 功能 : 延时子程序，延时时间为 1ms * x 
* 输入 : x (延时一毫秒的个数) 
* 输出 : 无 
***********************************************************************/  
/* 
void Delay_1ms(unsigned int i)//1ms延时 
{ 
    unsigned char x,j; 
    for(j=0;j<i;j++) 
    for(x=0;x<=148;x++);  
} 
*/  