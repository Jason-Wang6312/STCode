#include<reg51.h>
 
#define u8 unsigned char
#define u16 unsigned int

sfr P3M1=0xb1;
sfr P3M0=0xb2;
sfr AUXR=0x8e;
sfr T2H=0xd6;
sfr T2L=0xd7;
sfr IE2=0xaf;

sfr IAP_DATA=0xc2;
sfr IAP_ADDRH=0xc3;
sfr IAP_ADDRL=0xc4;
sfr IAP_CMD=0xc5;
sfr IAP_TRIG=0xc6;
sfr IAP_CONTR=0xc7;


sbit IN=P3^4;
sbit LED=P3^3;
sbit OA=P3^2;
sbit OB=P3^1;

bit turn;
bit stop,go;
bit ok,lose,error;
int proportion;
u16 MAX,MIN,MIDDLE,k;
u16 time,tt ;
u8  H_l,H_h,L_l,L_h;
u8 MARK1,MARK2,low,top,calabration,over,n;



void Delay100us()		//@12.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 39;
	do
	{
		while (--j);
	} while (--i);
}

void delay(u16 t)
{
	while(t--)
	Delay100us();
}

u8 EEPROM_read(u8 address)
{
	IAP_CMD=0x01;
	IAP_ADDRH=0;
	IAP_ADDRL=address;
	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5; 
	return IAP_DATA;
}

void EEPROM_write(u8 address,u8 byte)
{
	IAP_CMD=0x02;
	IAP_DATA=byte;
	IAP_ADDRH=0;
	IAP_ADDRL=address;
	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5; 
	
}

void EEPROM_clean(u8 address)
{
	IAP_CMD=0x03;
	IAP_ADDRH=0;
	IAP_ADDRL=address;
	IAP_TRIG=0x5a;
	IAP_TRIG=0xa5;
}

void DATA_read()
{
	IAP_CONTR=0x82;
	MARK1=EEPROM_read(0);
	MARK2=EEPROM_read(1);
	MAX=EEPROM_read(2);
	MAX<<=8;
	MAX+=EEPROM_read(3);
	MIDDLE=EEPROM_read(4);
	MIDDLE<<=8;
	MIDDLE+=EEPROM_read(5);
	IAP_CONTR=0;
}

void DATA_save()
{
	IAP_CONTR=0x82;

	EEPROM_clean(0);
	EEPROM_write(0,0x0f);
	EEPROM_write(1,0xa5);
	EEPROM_write(2,MAX>>8);
	EEPROM_write(3,MAX);
	EEPROM_write(4,MIDDLE>>8);
	EEPROM_write(5,MIDDLE);
	IAP_CONTR=0;
}

void measure()
{
	time=0;
	TL0=0,TH0=0;
	while(!IN);
	TR0=1; 
	while(IN);
	TR0=0; 
	time=TH0;
	time<<=8;
	time+=TL0;		
}

void shock(u8 n)//新增可变音调
{
	u8 i;
	for(i=0;i<250;i++)
	{
		OA=1;OB=0;
		delay(n);
		OA=0;OB=1;
		delay(n);	
	}
	OA=0;OB=0;
}

void initial()
{
	u8 i;	

	P3M0=0x0f;
	TMOD=0x01;
	OA=0;OB=0;
	LED=1;
	delay(2000);
	shock(2);	//新增 上电音乐，表明正常工作
	shock(3);
	shock(4);
	delay(5000);
	

	measure();
	measure();
	measure();
	measure();
	measure();
	LED=0;
	
	DATA_read();
	if(MARK1!=0x0f||MARK2!=0xa5)//新增 16bit验证，降低错误率
	{
		
		MAX=1950;MIDDLE=1500;//新增 自带1ms-2ms行程，免校准
		
	}
	
	over=1;
	while(over)
	{
		measure();
		
		if(time>1700)//重新校准行程判断
		{
			i++;
			if(i>100)over=0,calabration=1;
		}
		else		 
		{
			if(time<MIDDLE+50&&time>MIDDLE-50)//检测到中位点奏乐
			{
				over=0;
				shock(4);
				shock(3);
				shock(2);

			}
		}
	}	

	if(MAX>2200||MIDDLE<1300)//新增 数据验证，降低错误率
	{
		calabration=1;
		if(MAX<1700||MIDDLE>1700)calabration=1;
	}
	
	if(calabration)
	{
		over=1;
		while(over)
		{
			measure();
			if(time>1700)over=0;
		}
		delay(5000);delay(5000);
		shock(4);
		delay(2000);
		shock(4);
		measure();
		measure();
		if(time>1700&&time<2200)MAX=time;//最大行程判断
		else error=1;	 
		
		over=1;
		while(over)
		{
			measure();
			if(time<1700&&time>1300)over=0;
		}	 
		delay(5000);delay(5000);
		shock(4);
		delay(2000);
		shock(4);
		measure();
		measure();
		if(time>1300&&time<1700)MIDDLE=time;//中点判断
		else error=1;
		
		if(error==0)DATA_save();	//数据验证以免行程错误

		delay(2000);
		shock(4);//行程校准完毕奏乐
		shock(3);
		shock(2);
	}
	
}

main()
{
	initial();

	IE=0x80;
	IE2=0x04;
	stop=1;
	H_l=0x0b,H_h=0xfe;//定时初始化
	L_l=0x0b,L_h=0xfe;
	MIN=MIDDLE+MIDDLE-MAX;
	k=MAX-MIDDLE;

	AUXR=0x10;
	while(1)
	{
	 	measure();
		if(lose)
		{
			delay(1000);
			measure();
			measure();
		}

		if(time>800&&time<2200)
		{ 
			
			ok=1;lose=0;
			
			if(time>MIDDLE+50)
			{
				stop=0,go=1;
	
				if(time>MAX-20)time=MAX-20,top=1;
				else top=0;
		
				proportion=(long)(time-MIDDLE )*1000/k;

				H_l=65535-proportion ;	
				H_h=((65535-proportion)>>8);
				L_l=64535+proportion;
				L_h=((64535+proportion)>>8);	
			}
			else if(time<MIDDLE-50)
			{
				stop=0,go=0;
	
				if(time<MIN+20)time=MIN+20,low=1;
				else low=0;
		
				proportion=(long)(MIDDLE- time)*1000/k;

				H_l=65535-proportion ;	
				H_h=((65535-proportion)>>8);
				L_l=64535+proportion;
				L_h=((64535+proportion)>>8);
			}
			else
			{
				stop=1;
			}
			
			
		}
				 

	}
}

void et2()interrupt 12
{

  turn=!turn;
	if(stop)
	{
		OA=0,OB=0;
		T2L=0x0b,T2H=0xfe;
	}
	else
	{
		if(go)
		{
			OB=0;
			if(turn)
			{
				if(top)OA=1,T2L=0x0b,T2H=0xfe;
				else OA=0,T2L=H_l,T2H=H_h;
			}
			else 
			{
				OA=1,T2L=L_l,T2H=L_h;
			} 	
		}
		else
		{
			OA=0;
			if(turn)
			{
				if(low)OB=1,T2L=0x0b,T2H=0xfe;
				else OB=0,T2L=H_l,T2H=H_h;
			}
			else 
			{
				OB=1,T2L=L_l,T2H=L_h;
			}
		}	
	}
	
	if(lose)
	{
		n++;
		if(n==100)n=0,LED=!LED;//新增 信号丢失闪烁
	}
	else
	{
		if(OA|OB)LED=1;//新增 LED亮度指示舵量
		else LED=0;	
	}
	

	tt++;
	if(ok)tt=0;
	else {if(tt>2000)tt=2000,stop=1,lose=1;}//信号丢失切断输出
	ok=0;
}