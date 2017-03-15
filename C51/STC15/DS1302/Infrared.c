#include "infrared.h"

bit Irprot_LastState = 0;   // 端口状态位
u8 irTime=0;                   // 码时间，用于以125us时间计时
u8 IR_data[4] = {0x00,0x00,0x00,0x00}; // 接收数据缓存
u8 codeCnt=0;

void Timer0Init(void){		//125微秒@12.000MHz
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x24;		//设置定时初值
	TH0 = 0xFA;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0停止计时
	ET0=1;			//打开定时器0中断
	EA=1;			//打开全部中断
    IT0 = 0;	//INT0中断类型：1仅下降沿中断 0上下都中断
	EX0 = 1;	// 使能INT0中断
}

//定时器0中断函数 125us
void Timer0() interrupt 1{
   irTime++; //计时增加125us         	
   if(irTime==240) {irTime--;codeCnt=0x3f;} // ir解码后码值存放时间， 240*125us = 30ms  0x3f=64
   if(IR_IO)   Irprot_LastState=1; // 记录IO状态
   else if(Irprot_LastState){       // 有下降沿，并且上个状态是高电平，表示红外管收到数据
      Irprot_LastState = 0;        // 下降沿后IO状态记录为0
      if(irTime<24){ // 小于24*125us=3ms的间隔才进行处理	因为红外线的0的周期1.125ms，1的周期2.25ms
         codeCnt++; //数据码位计数+1
		 codeCnt &= 0x1f; //等效if(codeCnt>0x1f) codeCnt=0x00; 这种操作比if判断更节约时间 0x1f=31
         IR_data[codeCnt>>3] <<= 1; //codeCnt>>3的范围(0~3)，等效于IR_data[i]向左移动1位 (i范围0~3)
         if( irTime>15 )  IR_data[codeCnt>>3]++;  //大于15*125us=1.875ms的间隔为数据1 ，1就+1，0就不变。
      }
      irTime = 0;                  // 下降沿处理完成，将时间清0
	  if(codeCnt==31) TR0=0,EX0=1;//解码完成后，掉定时器0，顺便打开中断0
   }	
}

void INT0_ISR(void) interrupt 0{
	irTime=239;//给irTime赋予初始值，定时器0无法持续中断了，所以这里赋予默认值
	EX0=0; // INT0 暂时关闭
	TR0=1; //定时器0只有检测到有红外数据，才开始进行解码。
}



