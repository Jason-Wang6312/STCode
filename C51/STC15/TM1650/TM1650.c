 /*****************************************************************************
 *版权信息：深圳天微电子有限公司
 *文 件 名：TM1650-V1.0
 *当前版本：V1.0
 *MCU 型号：STC12C5608AD
 *开发环境：Keil uVision4
 *晶震频率：11.0592MHZ       
 *完成日期：2013-07-29
 *程序功能：1.上电8段4位共阴数码管显示1、2、3、4.
            2.按下K11与DIG1，K12与DIG2 ，K13与DIG3之间的按键，数码管第一位分别显示5、6、7。 
 *免责声明：1.此程序为TM1650驱动LED数码管演示程序，仅作参考之用。
            2.如有直接使用本例程程序造成经济损失的，本公司不承担任何责任             
********************************************************************************/
#include<STC15Wxx.h>				  //MCU头文件
#include<intrins.h>				  //包含nop指令头文件
#define uint unsigned int			  //数据类型宏定义
#define uchar unsigned char			  //数据类型宏定义

#define nop _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();  //宏定义


/********************定义控制端口**********************/
sbit SCL=P5^5;                       //时钟线
sbit SDA=P5^4;                       //数据线

uchar keya;	                     //定义读出按键返回值

/*************1ms延时*晶振11.0592M********************/
void delay(uint n)
{
  uint i;
  while(n--)
	 for(i=0;i<550;i++);
}

/**************共阴数码管显示0-F**********************/
 uchar display[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71}; //共阴极字段码

/************ START信号*******************************/
void TM1650_START()
{
	SCL=1;
	SDA=1;
	nop;
	SDA=0;
	nop;
	SCL=0;
}
/******************** STOP信号************************/
void TM1650_STOP()
{
	SDA=0;
	nop;
	SCL=1;
	nop;
	SDA=1;
	nop;
	SCL=0;
	SDA=0;
}
/****************写1个字节给TM1650********************/
void write_8bit( uchar dat)
{
 	uchar i;
	SCL=0;
	for(i=0;i<8;i++)
		{
		if(dat&0x80)
		{
			SDA=1;
			nop;
			nop;
			SCL=1;
			nop;
			nop;
			nop;
			nop;
			nop;
			SCL=0;	 
		}
		else
		{
			SDA=0;
			nop;
			nop;
			SCL=1;
			nop;
			nop;
			nop;
			nop;
			nop;
			SCL=0;
		}	
			dat<<=1;	 
		}
		SDA=1;			//ACK信号
		nop;
		nop;
		nop;
		nop;
		SCL=1;
		nop;
		nop;
		nop;
		nop;
		nop;
		SCL=0;
		nop;
		nop;	 
}

/**********************读8bit**************************/
uchar read_8bit()
{
	uchar dat,i;
	SDA=1;
	dat=0;
	for(i=0;i<8;i++)
	{
	SCL=1;                        //时钟上沿
	nop;
	nop;
	nop;
	dat<<=1;
	if(SDA)
	 dat++;
	SCL=0;
	nop;
	nop;
	nop;
	nop;
	}
	SDA=0;			    //ACK信号
	nop;
	nop;
	nop;
	SCL=1;
	nop;
	nop;
	nop;
	nop;
	SCL=0;
	nop;
	
	return dat ;

} 
/*******************读按键命令************************/
uchar TM1650_read()
{
	uchar key;
	TM1650_START();
    write_8bit(0x49);//读按键指令	
	key=read_8bit();
	TM1650_STOP();
	return key;
} 
/*****************发送命令信号***********************/
void TM1650_send(uchar date1,uchar date2)
{
 	TM1650_START();
	write_8bit(date1);
	write_8bit(date2);
	TM1650_STOP();
}

/*****************显示函数***********************/
void disp()
{
	TM1650_send(0x48,0x11);                     // 开启显示模式：8段显示，1级亮度
	TM1650_send(0X68,display[3]);  //GID1
	TM1650_send(0X6A,display[2]);  //GID2
	TM1650_send(0X6C,display[6]);  //GID3
	TM1650_send(0X6E,display[8]);  //GID4
}


/**************主函数**************************/
void main(void)
{ 
   disp();			                    //上电显示1、2、3、4
   delay(1);
   while(1)
	{
//		keya=TM1650_read();
//		delay(50);
//		if (keya!=0)
//		{
//			switch(keya)                 //控制数码管显示数字 	  
//			{	    
//			case 0x44:TM1650_send(0X68,display[5]);  break;    //K11与DIG1     
//			case 0x4D:TM1650_send(0X68,display[6]);  break;    //K12与DIG2           	
//			case 0x56:TM1650_send(0X68,display[7]);  break;    //K13与DIG3  
//			default:  break;
//			} 
//		}	
    } 
} 




 