#include<reg51.h>
#include<intrins.h" 

#define u8 unsigned char
#define u16 unsigned int

#define PWM_8kHz  0x00	
#define PWM_16kHz  0x0c	
#define PWM_23kHz  0x0a	
#define PWM_67kHz   0x02
#define PWM_94kHz   0x08


sfr IAP_DATA=0xc2;
sfr IAP_ADDRH=0xc3;
sfr IAP_ADDRL=0xc4;
sfr IAP_CMD=0xc5;
sfr IAP_TRIG=0xc6;
sfr IAP_CONTR=0xc7;

sfr P1M1=0x91;
sfr P1M0=0x92;
sfr P2M1=0x95;
sfr P2M0=0x96;
sfr P3M1=0xb1;
sfr P3M0=0xb2;
sfr P5M1=0xc9;
sfr P5M0=0xca;

sfr P5=0xc8;

sfr CMPCR1=0xe6;  //�Ƚ������ƼĴ���1
sfr CMPCR2=0xe7;  //�Ƚ������ƼĴ���2

sfr P1ASF=0x9d;
sfr ADC_CONTR=0xbc;
sfr ADC_RES=0xbd;
sfr ADC_RESL=0xbe;

sfr AUXR=0x8e;
sfr T2H=0xd6;
sfr T2L=0xd7;
sfr IE2=0xaf;






sfr CMOD=0xd9;                    //PCAģʽ�Ĵ���
sfr CCON=0xd8;                    //PCA���ƼĴ���
sfr CCAPM0=0xda;                 //PCAģ��0ģʽ�Ĵ���
sfr CCAPM1=0xdb;                 //PCAģ��1ģʽ�Ĵ���
sfr CCAPM2=0xdc;                     
sfr CCAP0H=0xfa;                 //PCAģ��0����Ĵ��� HIGH
sfr CCAP1H=0xfb;        
sfr CCAP2H=0xfc;
sfr CL=0xe9;                     //PCA��ʱ�����ֽ� 
sfr CH=0xf9;                     //PCA��ʱ�����ֽ�

/***************************************************************/

sbit PA=P3^6;
sbit PB=P3^3;
sbit PC=P3^1;
sbit NA=P1^1;
sbit NB=P1^0;
sbit NC=P3^7;

sbit in_A=P1^3;
sbit in_B=P1^4;
sbit in_C=P1^5;
sbit in_M=P5^4;

sbit P12=P1^2;

sbit IN=P3^2;


sbit LED=P3^0;
sbit out1=P5^5;

sbit out4=P5^0;//��������


/********************************************************/
	u16  time,Signal_time;
u8 speed;
u8 PWM_value;

u8 phase;

bit Motor_stop;
u8 time_out;

	
u16 Phase_time,Last_phase_time;
u8 New_CL,New_CH;
bit Phase_OK;
u8 Phase_ERROR;
u16 MAX,MIN;
u8 MARK1,MARK2;

u8 timer1,timer2;

u8 Phase_count,Motor_speed;
bit Running;
u8 Starting,Start_delay;


u8 delay_time;

bit PWM_OUT;
bit Signal_lose;
u8 phase_flag;

void Delay100us()		//@24.000MHz
{
	unsigned char i, j;

	i = 3;
	j = 82;
	do
	{
		while (--j);
	} while (--i);
}

void delay(u16 i)
{
	while(i--)
	Delay100us();
}

void Delay10us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 57;
	while (--i);
}

void delay_10us(u16 i)
{
	while(i--)
	Delay10us();
}
void Delay1us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 3;
	while (--i);
}
void  Wait_1250nS()
{
 Delay1us();
 _nop_();
	_nop_();
}
void delay_us(u16 i)
{
	while(i--)
	Delay1us();
}

void Delay5us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 27;
	while (--i);
}

void Fee()//�ر����
{
	
	NA=0;
	NB=0;
	NC=0;
	PA=1;  
	PB=1;  
	PC=1;  
	CCAPM0=0;
	CCAPM1=0;
	CCAPM2=0;
Wait_1250nS();
Wait_1250nS();
}



void Commutation()//����ˢ�������һ��6���Ĳ�������ͺ������
{
	phase++;
	if(phase>5)phase=0;
	Fee();
	switch(phase)
	{
		case 0:PC=1;PB=1;CCAPM0=0;CCAPM2=0;//�ϱ��л�0,0
						PA=0;//A-B 
						
						
						CCAP1H=255-PWM_value;CCAPM1=0x42;
						break;
		
		case 1:PC=1;PB=1;CCAPM0=0;CCAPM1=0;
	
						   PA=0;//A-C
						CCAPM2=0x42;CCAP2H=255-PWM_value;//�±��л� ����ΪPWMģʽ
						break;
		
		case 2: PA=1;PC=1;CCAPM0=0;CCAPM1=0;
								   PB=0;//B-C 
						CCAP2H=255-PWM_value;CCAPM2=0x42;//����ΪPWMģʽ
						break;
		
		case 3:PA=1;PC=1;CCAPM1=0;CCAPM2=0;///
						  PB=0;//B-A 
							CCAP0H=255-PWM_value;//����ΪPWMģʽ		
             CCAPM0=0x42;  							 
						break;
		
		case 4:PA=1;PB=1;CCAPM1=0;CCAPM2=0;///0,0
						PC=0;//C-A 
						
						
						CCAP0H=255-PWM_value;
						CCAPM0=0x42;
						break;
		
		case 5:PA=1;PB=1;CCAPM2=0;CCAPM0=0;
					    	PC=0;//C-B   
						   CCAP1H=255-PWM_value;			CCAPM1=0x42;		
						break;
	}
	
	
}


void Set_Comparator()//���ñȽ���
{
	switch(phase)
	{
		case 0:ADC_CONTR=0x85;CMPCR1=0x9e;
	
						break;
		case 1:ADC_CONTR=0x84;CMPCR1=0xae;
						break;
		case 2:ADC_CONTR=0x83;CMPCR1=0x9e;

						break;
		case 3:ADC_CONTR=0x85;CMPCR1=0xae;
		       
						break;
		case 4:ADC_CONTR=0x84;CMPCR1=0x9e;
		       
						break;
		case 5:ADC_CONTR=0x83;CMPCR1=0xae;
		      
						break;
	}
		
		
}

void shutdown()//�ر����
{
	
	NA=0;
	NB=0;
	NC=0;
	PA=1;  
	PB=1;  
	PC=1;  
	CCAPM0=0;
	CCAPM1=0;
	CCAPM2=0;
	
}

void Start_Motor()//�������������һ���𽥼ӿ����ת�ų�
{
	u16 i=160;
	PWM_value=40;//15%ռ�ձ�����



	PA=0;PB=1;PC=0;//A+C+B-//1,0,1						
	
	CCAPM0=0;CCAPM2=0;
	CCAP1H=255-PWM_value;CCAPM1=0x42;
	
	delay(800);//ת�Ӷ�λ
	
	phase=5;
	
	while(i>11)//������ת�ų�
	{
		PWM_value++;//�Ľ������㷨���ӿ�����ͬʱ����PWM
		Commutation();
		delay(i);
		i=i*14/15;
	}
	
	shutdown(); //�ȴ��Ƚ����ж�
	
	delay(10);
	Motor_stop=0;
	time_out=0;
	Running=1;Starting=2;
	phase=4;
	Set_Comparator();
}

void Fileter_Spike()//�˳����ż��
{
	u8 i=0;
	Delay10us();
	if(phase==0||phase==2||phase==4)
	{
		while((CMPCR1&0x01)&&i<200)
		{
			i++;out4=!out4;
		}
	}	
	else
	{
		while(!(CMPCR1&0x01)&&i<200)
		{
			i++;out4=!out4;
		}
	}
	
	if(i>160)
	{
		out4=!out4;out4=!out4;out4=!out4;
//		Motor_stop=1;
//		Starting=0;
	}
}


/*----------------------------------------*/
/*----------------���ֳ���--------------*/
/*----------------------------------------*/
void shock_AB(u8 n)
{
	u8 i;
	for(i=0;i<250;i++)
	{
		PB=1,NA=0; 
		delay_10us(n);
		
		PA=0,NB=1;  
		delay_10us(n);
		
		PA=1,NB=0;  
		delay_10us(n);
				
		PB=0,NA=1;  
		delay_10us(n);	
	}
	NA=0;  
	NB=0;
	NC=0;
	PA=1;  
	PB=1;   
	PC=1;   
	delay_10us(n);
}

void shock_BC(u8 n)
{
	u8 i;
	for(i=0;i<250;i++)
	{
		PC=1,NB=0;
		delay_10us(n);
		PB=0,NC=1; 

		delay_10us(n);
		
		PB=1,NC=0;
		delay_10us(n);
		PC=0,NB=1;

		delay_10us(n);	
	}
	NA=0;
	NB=0;
	NC=0;
	PA=1;  
	PB=1;  
	PC=1; 
	delay_10us(n);
}

void shock_CA(u8 n)
{
	u8 i;
	for(i=0;i<250;i++)
	{
		PA=1,NC=0;
		delay_10us(n);
		PC=0,NA=1;  

		delay_10us(n);
		
		PC=1,NA=0;  
		delay_10us(n);
		PA=0,NC=1;

		delay_10us(n);	
	}
	NA=0;
	NB=0;
	NC=0;
	PA=1; 
	PB=1;  
	PC=1;  
	delay_10us(n);
}




void initial()   ///��ʼ��
{
	shutdown();//�ȴ��Ƚ����ж�
	
	P1M0=0x03;	
	P3M0=0xca;
	
	P1M1=0x38;
	P5M1=0x10;
	
	
	CMOD=0x00;	//24KHz PWM
	CCON=0x40;
}

main()
{
	initial();
	
	CMPCR1=0x8e;
	CMPCR2=0x3f;
	ADC_CONTR=0x88;
	P1ASF = 0X38;
	

	shock_CA(20);//�ϵ�����
	shock_BC(15);
	shock_AB(10);
	
	delay(100);
	
	TMOD = 0x09;
	TCON=0x01;

	
	IE=0x81;
	IE2=0x04;
		TR0=1;
	while(1)	//�ȴ��ź�
	{
		if(time>1000&&time<1160)
		{
			break;
		}
	}
	Signal_time=time;
	
	shock_CA(20);	//���ź�����
	shock_BC(15);
	shock_AB(10);
	delay(100);
T2L = 0x60;		//���ö�ʱ��ֵ
	T2H = 0xF0;		//���ö�ʱ��ֵ
		
	AUXR=0x10;

	
	
	while(1)
	{
		if(time>1000&&time<=2020)
		{
			
				Signal_time=(Signal_time*7+time)/8;	

			
			
			speed=(Signal_time-1000)/4;
				
				
				if(speed<40)//�رյ��
				{
					Starting=0;
					speed=0;
					PWM_OUT=0;
					shutdown();
					
				}
				else if(speed>245)
				{
					speed=255;PWM_OUT=1;
				}
				
				
				else 
				{
					Running=1;PWM_OUT=1;
				}
				
			if(Starting==0)
			{
				

				
				
				if(Motor_stop&&speed>=40)//�������
				{
					PWM_OUT=0;
					shutdown();
					Starting=1;
					Running=0;
					CMPCR1=0;
					
					delay(100);
					Start_Motor();
					
				}	
			}
					}

		delay(20);


					}
}

void EX0_isr()interrupt 0	  //�������	 
{
  u16  width=0;
	width= ((u16)TH0 << 8) + TL0;
	TH0 = 0;
	TL0 = 0;
	
	width=width/2;
	if((width>= 700) && (width <= 2500) )
	{
	  timer2=0;
		time=width;
		if(time<1100)time=1100;
		if(time>2000)time=2020;//2020

  }
	
}


void ET2_isr()interrupt 12//��ʱ���ж�2.5ms
{
	
	if(Running)  ///�������Running=0��
	{
		time_out++;
		if(time_out>11)//30ms��ת
		{
			time_out=0;
			Motor_stop=1;
			Starting=0;
		}	
	}
	
	
	timer1++;
	if(timer1>1)//5ms���㻻���ٶ�
	{
		timer1=0;
		Motor_speed=Phase_count;
		Phase_count=0;
	}
	
	timer2++;
	if(timer2>200)//0.5S���źű���
	{
		timer2=200;
		time=1020;Signal_lose=1;  //�źŶ�ʧ
	}

}


void Comparator_isr()interrupt 21//�Ƚ����ж�
{
	out1=1;
	delay_time=175/(Motor_speed+1);//����ʱ���30%150     /Motor_speed*1500us   1500us/Motor_speed*3/10
	if(delay_time>150)delay_time=150;
	if(delay_time<20)delay_time=1;
	if(PWM_OUT)//�˳�խ����
	{
		Fileter_Spike();
		Fileter_Spike();
		Fileter_Spike();
		//Fileter_Spike();
		if(Motor_stop)
		{
			CMPCR1&=~(0x40);out1=0;
				return;
		}
	}
	
	if(Starting==0&&Motor_speed==0)//ת�ٹ����ж�Ϊֹͣ
	{
		Motor_stop=1;
		CMPCR1&=~(0x40);out1=0;
		return;
	}
	
	Phase_OK=1;
	
	if(Phase_OK)
	{
		if(PWM_OUT)
		{
			if(Starting)//���������
			{
				if(PWM_value<110||PWM_value<speed)
				{
					Start_delay++;
					if(Start_delay>3)Start_delay=0,PWM_value++;
					
				}
				
			else 
				{
					Starting=0;
				}
				
				if(Starting==2)Starting=3;

			}
			
			else if(Signal_lose)//��ʧ�źŻ�����
			{
				if(PWM_value<speed)
				{
					Start_delay++;
					if(Start_delay>3)Start_delay=0,PWM_value++;					
				}
				else 
				{
					Signal_lose=0;
				}
			}
			else//�����ɹ�
			{
				PWM_value=speed;
			}
			Commutation();
			delay_us(delay_time);///10us
				
		}
		else
		{
			if(Starting==0)//������ת�׶�
			{
				phase++;
				if(phase>5)phase=0;	
			}
			
		}
		

		

		Set_Comparator();
		
		Phase_count++;
		time_out=0;Motor_stop=0;
		Running=1;
	}
	
	out1=0;CMPCR1&=~(0x40);
	
	if(Starting!=2)//�ȴ��жϽ׶�
	{
		if(phase==0||phase==2||phase==4)//���������½��ض�ʧ
		{
			if(!(CMPCR1&0x01))
			{
				Delay5us();
				if(!(CMPCR1&0x01))
				{
	
					CMPCR1|=0x40;return;	
				}
				
			}
		}	
		else
		{
			if(CMPCR1&0x01)
			{
				Delay5us();
				if(CMPCR1&0x01)
				{
		
					CMPCR1|=0x40;
					return;	
				}
			}
		}	
	}
}
