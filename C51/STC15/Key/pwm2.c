/*******************************************************************/
/* 程序名：PWM直流电机调速 */
/* 晶振：11.00592 MHz CPU型号：STC89C52 */
/* 直流电机的PWM波控制，可以通过按键控制正反转并在0到20级之间调速 */ 
/*****************************************************************/ 
#include<reg52.h>

#define uint unsigned int 
#define uchar unsigned char

uchar time,count=50,flag=1;//低电平的占空比

sbit PWM1=P2^0;//PWM 通道 1，反转脉冲
sbit PWM2=P2^1;//PWM 通道 2，正转脉冲
sbit key_add=P3^5;//电机加速
sbit key_dec=P3^6;//电机减速
sbit key_turn=P3^7;//电机换向

/************函数声明**************/
void delayxms(uint z);
void Motor_turn();
void Motor_add();
void Motor_dec();
void timer0_init();

/*********主函数********************/
void main()
{
	timer0_init();
	while(1)
	{
		Motor_turn();
		Motor_add();
		Motor_dec();
	}
}

/****************延时处理**********************/
void delayxms(uint z)//延时xms程序
{
    uint x,y;
	for(y=z;x>0;x--)
		for(y=110;y>0;y--);
}

/************电机正反向控制**************/
void Motor_turn()
{
	if(key_turn==0)
	{
	  	delayxms(2);//此处时间不能太长，否者会的中断产生冲突
	  	if(key_turn==0)
	  	{
	    	flag=~flag;
	  	}
	  	while(!key_turn);
	}
}

void Motor_add()//电机加速
{
	if(key_add==0)
	{
	  	delayxms(2);//此处时间不能太长，否者会的中断产生冲突
	  	if(key_add==0)
	  	{
	    	count+=5;
			if(count>=100)
			{
				count=0;
			}
	  	}
	  	while(!key_add);
	}
}

void Motor_dec()//电机加减速
{
	if(key_dec==0)
	{
	  	delayxms(2);//此处时间不能太长，否者会的中断产生冲突
	  	if(key_dec==0)
	  	{
	    	count-=5;
			if(count>=100)
			{
				count=0;
			}
	  	}
	  	while(!key_dec);
	}
}

/***********定时器0初始化***********/
void timer0_init()
{
	TMOD=0x01; //定时器0工作于方式1
	TH0=(65536-10)/256;
	TL0=(65536-10)%256;
	TR0=1;
	ET0=1;
	EA=1;
}

/**************定时0中断处理******************/
void timer0_int() interrupt 1 
{
	
	TR0=0;//设置定时器初值期间，关闭定时器
	TH0=(65536-10)/256;
	TL0=(65536-10)%256;
	TR0=1;
	
	if(flag==1)//电机正转
	{
	   	PWM1=0; 
	   	time++;
	  	if(time<count)
	  	{
	    	PWM2=1;
	  	}
	    else 
	    PWM2=0;
	
	    if(time>=100)
	    {
	    	time=0;
	    }
	}
	else //电机反转
	{
		PWM2=0; 
	  	time++;
	    if(time<count)
	    {
	    	PWM1=1;
	    }
	    else 
	    	PWM1=0;
	    
	    if(time>=100)
	    {
	    	time=0;
	    }
	}
}