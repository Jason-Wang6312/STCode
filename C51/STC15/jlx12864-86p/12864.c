#include  "config.h"
#include  "zk.h"
#include  "12864.h"
#include <string.h>
#include <intrins.h>




					  
/************************************************以下是12864底层驱动*********************************************************/
/*------------------------
说明: 发送子函数
--------------------------*/
void SPI_Send( u8 Data )
{	
	u8 i;
	CS = 0; 
	SCL = 0;
	for(  i = 0; i < 8; i++ )
    {    
       if( Data & 0x80 ) 
	   { 
	   		SI = 1;	
	   }
       else              
	   { 
	   		SI = 0;  
	   }
       Data <<= 1;
       SCL = 0;
	   _nop_();
	   _nop_();	  
	   SCL = 1; 
	   _nop_();
	   _nop_();  
    }
}
/*------------------------
说明: 写一个字命令函数
--------------------------*/
void WriteCom(u8 ComData)
{
	A0 = 0; 
	SPI_Send( ComData );
}

/*------------------------
说明: 写一个字数据函数
--------------------------*/
void WriteData(u8 Data)
{
	A0 = 1; 
	SPI_Send( Data );
}

/*------------------------
说明: 液晶初始化
--------------------------*/
void InitST7565()
{
	CS=0;
	RES=0;
	delay500ms();
	RES=1;
	delay500ms();
	//液晶显示初始化配置
	WriteCom( 0xaf ); //液晶使能,打开LCD显示 AE--off   AF--on
	WriteCom( 0x40 ); //开始显示,显示起始行设置,设置显示起始行0x40-0x7f共64行  
	WriteCom( 0xa6 ); //设置显示命令  1 = 点亮，0 = 点灭,设置显示模式（a6正，a7反）
						   
	//扫描次序配置
	WriteCom( 0xa0 ); //设置列显示方向,a0,正常,列扫描从左至右，a1，COM与SEG互换，从右至左扫描
	WriteCom( 0xc8 ); //选择COM扫描方向，C7为正常方向，C8为反向方向,从上至下扫描
	
	//内部电源配置
	WriteCom( 0xa4 ); //屏幕全亮CLEAR 
	WriteCom( 0xa2 ); //偏光设置，1/9BIAS  设置LCD电压偏置 a2--1/9,a3---1/7	               
	WriteCom( 0x2f ); //选择内部电源供给模式，启动内部LCD驱动电源,x2f升压电压调节器电压跟随器开
	                       
	WriteCom( 0x81 ); //背光LED配置命令，设置V0电压寄存器
	WriteCom( 0x20 ); //背光LED配置值，在6~32之间时，亮度较好	 设置亮度，从00~3F。必须跟在81的命令后面	           
}
/*===================================================================
函数名称：void LCDFill(u8 Data)
函数功能：全屏填充以Data的数据至各点中
入口参数: Data 要填充的颜色数据
出口参数:无
函数说明: 
======================================================================*/ 
void LCDFill(u8 Data)
{
	u8 i, j = 0;
	u8 uiTemp;
	uiTemp = Dis_Y_MAX;
	uiTemp = uiTemp>>3;
	for(i=0;i<=uiTemp;i++)				//往LCD中填充初始化的显示数据
	{
		 WriteCom(0xb0+i);
		 WriteCom(0x00);
		 WriteCom(0x10);
		 for(j=0;j<=Dis_X_MAX;j++)
		 {
		 	WriteData(Data);
		 }
	}
	WriteCom(0xe0);
}

/*===================================================================
函数名称：void Displayen(U8 page,U8 column,U8 number)
函数功能：显示一个英文字符(8*16),可选择是否反白显示，1，反白
入口参数: 页地址，列地址，字符首地址		
出口参数: 
函数说明: 
======================================================================*/
void Displayen(u8 page,u8 column,u8 number,bit Reserve)
{
	u8 i; //pagr选页参数，column选列参数，number选第几个字输出
	u8 column_low, column_hight;	
	column_low = (column&0x0F);				//定位列地址设置的低位指令
	column_hight = ((column>>4)&0x0F)+0x10;	//定位列地址设置的高位指令
	number -= 0;							
	WriteCom(page+0xB0);	//写上半页	
	WriteCom(column_hight);
	WriteCom(column_low);
   	for(i=0;i<8;i++)		//横向只要扫描8个点就好了
	{
		if(Reserve)
		{
			WriteData(~(ASCII[i+16*number]));
		}
		else
		{
			WriteData(ASCII[i+16*number]);		 //WriteData(ASCII[i+16*number]);
		}
	}
		
	WriteCom(page+0xB0+1);	//写下半页	
	WriteCom(column_hight);
	WriteCom(column_low);
   	for(i=0;i<8;i++)
	{
		if(Reserve)
		{
			WriteData(~(ASCII[i+16*number+8]));
		}
		else
		{
			WriteData(ASCII[i+16*number+8]);		 //	WriteData(ASCII[i+16*number+8]);
		}
	}
}

//在某个起始地方显示某个字库中的一个中文字符16*16
void Displaych(u8 page,u8 column,u8 number,u8 *name,bit Reserve)
{
	u8 i; //pagr选页参数，column选列参数，number选第几个字输出
	u8 column_low, column_hight;	
	column_low = (column&0x0F);				//定位列地址设置的低位指令
	column_hight = ((column>>4)&0x0F)+0x10;	//定位列地址设置的高位指令
	number -= 0;	//?
    //由于汉字是16*16的，所以要分两页去显示
	WriteCom(page+0xB0);	//写上半页	
	WriteCom(column_hight);
	WriteCom(column_low);
   	for(i=0;i<16;i++)
	{
		if(Reserve)
		{
			WriteData(~(name[i+32*number]));
		}
		else
		{
			WriteData(name[i+32*number]);		 //WriteData(ASCII[i+16*number]);
		}
	}
		
	WriteCom(page+0xB0+1);	//写下半页	
	WriteCom(column_hight);
	WriteCom(column_low);
   	for(i=0;i<16;i++)
	{
		if(Reserve)
		{
			WriteData(~(name[i+32*number+16]));	
		}
		else
		{
			WriteData(name[i+32*number+16]);	 //WriteData(ASCII[i+16*number+8]);
		}
	}
}
/************************************************12864驱动程序结束*******************************************************/
void displaychinese()
{

    Displaych(0,0,4,zk,0);	  //电压
	Displaych(0,16,5,zk,0);
	Displayen(0,32,11,0);
	Displayen(0,80,15,0);
	Displayen(0,88,16,0);
	Displayen(0,96,17,0);
	Displayen(0,104,11,0);
//	Displaych(0,112,1,zk,0);
	Displaych(3,0,0,zk,0);	  //闭合
	Displaych(3,16,1,zk,0);
	Displaych(3,32,6,zk,0);	  //时间
	Displaych(3,48,7,zk,0);	  
	Displayen(3,64,11,0);	  //:
	Displayen(3,112,12,0);	  //MS
	Displayen(3,120,13,0);
	Displaych(6,0,2,zk,0);	  //断开
	Displaych(6,16,3,zk,0);
	Displaych(6,32,6,zk,0);	  //时间
	Displaych(6,48,7,zk,0);	  
	Displayen(6,64,11,0);     //:
	Displayen(6,112,12,0);	  //MS
	Displayen(6,120,13,0);	


}

void displayhe()				 //合
    {
	Displaych(0,112,1,zk,0);
	}

void displaykai()				//开
    {
	Displaych(0,112,3,zk,0);
	}

void displayok()				//OK
    {
	Displayen(0,112,16,0);
	Displayen(0,120,19,0);

	}

void displayno()			   //NO
    {
	Displayen(0,112,18,0);
	Displayen(0,120,16,0);
	}