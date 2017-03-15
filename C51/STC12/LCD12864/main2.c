#include <STC15F2K60S2.H>
#include <intrins.h>

#define u8 unsigned char
#define u16  unsigned int

#define ID_ADDR_ROM 0x1ff7//8K����ռ��MCU

#define ADC_POWER	0x80//ADC��Դ����λ
#define ADC_FLAG	0x10//ADC��ɱ�־
#define ADC_START	0x08//ADC��ʼ����λ
#define ADC_SPEEDLL	0x00//540��ʱ��ת��һ��
#define ADC_SPEEDL	0x20//360ʱ��
#define ADC_SPEEDH	0x40//180ʱ��
#define ADC_SPEEDHH	0x60//90ʱ��


sbit QC_CS=P2^5;	    //Ƭѡ�ź�
sbit QC_SID=P2^4;		//�����ź�
sbit QC_SCLK=P2^3;		//ʱ���ź� 
//sbit QC_RST=P3^2;		//��λ�ź�	 ʵ����Բ���
//sbit CH = P3^4;		//���С�����ѡ���ź�   R10����������ѡ�񴮿ںͲ�����

sbit Key1=P2^6;
sbit Key2=P2^7;
sbit Charge=P2^0;//���Ƴ���MOS

u8 lcd[16];
u8 cnt=0;
u8 cnt2=0;
u8 cnt3=0;
u16 Voltag=14400;//����ֹ��ѹ 14400V
bit IsSave=0;//�Ƿ�Ҫ�������
u16 V1,I1,V2,I2;
bit IsFull=0;//����Ƿ����
u16 bandgap;//bandgapԤ����У׼ֵ����λ����
u16	adc0,adc1,adc2,adc3,adc9;//�����ھ�ͨ����bandgap��ֵ
u16 adcsum;
/************************12864����*****************************/

void QC_Delay100us()		//@12.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 39;
	do
	{
		while (--j);
	} while (--i);
}

void QC_Delay(unsigned int t)
{
	do
	{
		QC_Delay100us();
	
	}while(t--); 
}

void QC_SendByte(unsigned char zdata)
{
	unsigned char i;
	for(i=0; i<8; i++)
	{
		if((zdata << i) & 0x80)
		{
			QC_SID = 1;
		}
		else 
		{
			QC_SID = 0;
		}
		QC_SCLK = 0;
		QC_SCLK = 1;
	}
}

void QC_WriteCom(unsigned char cmd)
{
	QC_CS = 1;
	QC_SendByte(0xf8);
	QC_SendByte(cmd & 0xf0);
	QC_SendByte((cmd << 4) & 0xf0);
	QC_Delay(2);
}


void QC_WriteData(unsigned char dat)
{
	QC_CS = 1;
	QC_SendByte(0xfa);
	QC_SendByte(dat & 0xf0);
	QC_SendByte((dat << 4) & 0xf0);
	QC_Delay(2);
}

void QC_LcdInit()
{  
//	QC_RST = 0;	 //���Բ�������Ҳ���õ����ӣ���ʱȥ����
//	QC_Delay(100);
//	QC_RST = 1;

	QC_WriteCom(0x30);//��ʾģʽ����
	QC_Delay(50);
	QC_WriteCom(0x30);//2����ʾģʽ
	QC_Delay(50);
	QC_WriteCom(0x08);//������ʾ����ꡢ��˸ȫ�ء�
	QC_Delay(50);
	QC_WriteCom(0x01);//��ʾ����
	QC_Delay(50);
	QC_WriteCom(0x06);//����ģʽ����
	QC_Delay(50);
	QC_WriteCom(0x0C);//����ʾ
	QC_Delay(50);

}

//�������е�ַ��QC12864��ַ��4�У�8�У�ÿ��2���ֽڡ�
void QC_WriteAC(unsigned char hang,unsigned char lie)
{
    if(hang == 1)	QC_WriteCom(0x80+lie-1);
	if(hang == 2)	QC_WriteCom(0x90+lie-1);
	if(hang == 3)	QC_WriteCom(0x88+lie-1);
	if(hang == 4)	QC_WriteCom(0x98+lie-1);
}

//���֣�4�У�ÿ��8������
void QC_ShowString(unsigned char hang,unsigned char lie,unsigned char *s)
{
	if((hang == 0) || (hang > 4)) return; 
	if((lie == 0) || (lie > 8))	return;
	//QC_WriteCom(0x03);
	QC_WriteAC(hang,lie);
	while(*s > 0)
    { 
		QC_WriteData(*s);
		s++;
    }
}
//���֣�4�У�ÿ��8������
void QC_ShowString2(unsigned char hang,unsigned char lie,unsigned char *s)
{
	if((hang == 0) || (hang > 4)) return; 
	if((lie == 0) || (lie > 8))	return;
	QC_WriteAC(hang,lie);
	while(*s > 0)
    { 
		QC_WriteData(*s);
		s++;
    }
}
//����ָ����һ�С�
void ShowWhiteHang(unsigned char hang)
{
  unsigned char i,j;
  QC_WriteCom(0x36);//������ͼָ��
  //*******��ʾ�ϰ�����������
   for(i=0;i<16;i++)              //
    { 
      if(hang==1)
	  {
	    QC_WriteCom(0x80+ i);
	    QC_WriteCom(0x80);
	  }
      if(hang==2)
	  {
	    QC_WriteCom(0x90+ i);
	    QC_WriteCom(0x80);
	  }
      if(hang==3)
	  {
	    QC_WriteCom(0x80+ i);
	    QC_WriteCom(0x88);
	  }
      if(hang==4)
	  {
	    QC_WriteCom(0x90+ i);
	    QC_WriteCom(0x88);
	  }

      for(j=0;j<9;j++)	QC_WriteData(0x00);
	  for(j=9;j<15;j++)QC_WriteData(0xFF);//����14.4V ��5���ַ�
	  QC_WriteData(0x00);
    }	   

  QC_WriteCom(0x30);//�����ַ�ָ��
}


//ȡ������ָ����һ�С�
void ShowBlackHang(unsigned char hang)
{
  unsigned char i,j;
  QC_WriteCom(0x36);//������ͼָ��
  //*******��ʾ�ϰ�����������
   for(i=0;i<16;i++)              //
    { 
      if(hang==1)
	  {
	    QC_WriteCom(0x80+ i);
	    QC_WriteCom(0x80);
	  }
      if(hang==2)
	  {
	    QC_WriteCom(0x90+ i);
	    QC_WriteCom(0x80);
	  }
      if(hang==3)
	  {
	    QC_WriteCom(0x80+ i);
	    QC_WriteCom(0x88);
	  }
      if(hang==4)
	  {
	    QC_WriteCom(0x90+ i);
	    QC_WriteCom(0x88);
	  }


      for(j=0;j<16;j++)	QC_WriteData(0x00);
    }	   

  QC_WriteCom(0x30);//�����ַ�ָ��
}


//�Զ����ַ���ûɶ��

void QC_ShowMyChar()
{
    unsigned char i;
	unsigned char code defchar[32] =  //̫������
	 {0x01,0x00,0x41,0x04,0x21,0x08,0x11,0x10,0x07,0xC0,0x08,0x20,0x08,0x20,0xF8,0x3E,
      0x08,0x20,0x08,0x20,0x07,0xC0,0x11,0x10,0x21,0x08,0x41,0x04,0x01,0x00,0x00,0x00}; 
	unsigned char code defchar2[32] = //��ط���
	{0x00,0x00,0x03,0xC0,0x0F,0xF0,0x10,0x08,0x10,0x08,0x17,0xE8,0x17,0xE8,0x10,0x08,
    0x17,0xE8,0x17,0xE8,0x10,0x08,0x17,0xE8,0x17,0xE8,0x10,0x08,0x0F,0xF0,0x00,0x00};
    
 	QC_WriteCom(0x40);//�Զ���������4��16*16 ��ַ�ֱ���0x40 50 60 70

    for(i = 0; i < 32; i++)	//д��һ���Զ�����ŵĵ�������
	{
	  QC_WriteData(defchar[i]);
    }
	QC_WriteCom(0x80);//0x80��ʾ��һ�е�1��λ��
    QC_WriteData(0x00);//��
	QC_WriteData(0x00);//0x00��ʾ��ʾ��һ���Զ������

    //�ڶ�����ط���
    QC_WriteCom(0x50);

    for(i = 0; i < 32; i++)
	{
	  QC_WriteData(defchar2[i]);
    }
	QC_WriteCom(0x80+7);//0x80+7��ʾ��һ�е�8��λ��
    QC_WriteData(0x00);
	QC_WriteData(0x02);//0x02��ʾ��ʾ�ڶ����Զ������

	//������0x60 0x04
	//���ĸ�0x70 0x06
}

//�����ͼ
void QC_ClearLCDGraphic()
{
  unsigned char i,j;
  QC_WriteCom(0x36);//������ͼָ��
  //*******��ʾ�ϰ�����������
   for(i=0;i<32;i++)              //
    { 
      QC_WriteCom(0x80 + i); //SET  ��ֱ��ַ VERTICAL ADD	  
      QC_WriteCom(0x80);       //SET  ˮƽ��ַ HORIZONTAL ADD  
      for(j=0;j<16;j++)	QC_WriteData(0x00);
    }	   
//*******��ʾ�°�����������
   for(i=0;i<32;i++)              //
    {
      QC_WriteCom(0x80 + i); //SET  ��ֱ��ַ VERTICAL ADD	  
      QC_WriteCom(0x88);       //SET  ˮƽ��ַ HORIZONTAL ADD  
      for(j=0;j<16;j++)	QC_WriteData(0x00);
    }
	QC_WriteCom(0x30);//�����ַ�ָ��
}
/**********************12864�������**********************************/

void Delay1ms()		//@12.000MHz   �����ʱ1ms
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}

void DelayMs(unsigned int i)  //��ʱN���룬0~65535ms
{
   while(--i)
   {
	 Delay1ms();
   }  
}

/*************EEPROM��д*************/
/*********************��ʼ����EEPROM�Ķ�д����**************************/ 
//  �������� 
//---------------------------------------------------------------- 
void EEPROM_Eares(unsigned int addres) 
{      
    IAP_ADDRL=addres;     //��λ��ַ 
    IAP_ADDRH=addres>>8;  //��λ��ַ 
    IAP_CONTR=0x01;  
    IAP_CONTR=IAP_CONTR|0x82; //��ʱ�����ISP������ 
    IAP_CMD=0x03;         //���������� 
    IAP_TRIG=0x5a;        //���� 
    IAP_TRIG=0xa5;        //���������� 
	_nop_();_nop_();_nop_();_nop_();
    IAP_ADDRL=0xff; 
    IAP_ADDRH=0xff; 
    IAP_CONTR=0x00; 
    IAP_CMD=0x00; 
    IAP_TRIG=0x00;  
} 
//**************************************************************** 
//  ������ 
//---------------------------------------------------------------- 
unsigned char EEPROM_Read(unsigned int addres) 
{ 
    IAP_ADDRL=addres;     //��λ��ַ 
    IAP_ADDRH=addres>>8;  //��λ��ַ 
    IAP_CONTR=0x01;  
    IAP_CONTR=IAP_CONTR|0x82; //��ʱ�����ISP������ 
    IAP_CMD=0x01;         //д���� 
    IAP_TRIG=0x5a;        //���� 
    IAP_TRIG=0xa5;        //���������� 
    _nop_();_nop_();_nop_();_nop_(); 
    IAP_ADDRL=0xff; 
    IAP_ADDRH=0xff; 
    IAP_CONTR=0x00; 
    IAP_CMD=0x00; 
    IAP_TRIG=0x00; 
    
    return(IAP_DATA);         
} 
//**************************************************************** 
//  д���� 
//---------------------------------------------------------------- 
void EEPROM_Write(unsigned int addres,unsigned char write_data) 
{ 
    IAP_DATA=write_data;  //Ҫд������ݡ� 
    IAP_ADDRL=addres;     //��λ��ַ 
    IAP_ADDRH=addres>>8;  //��λ��ַ 
    IAP_CONTR=0x01;  
    IAP_CONTR=IAP_CONTR|0x82; //��ʱ�����ISP������ 
    IAP_CMD=0x02;         //д���� 
    IAP_TRIG=0x5a;        //���� 
    IAP_TRIG=0xa5;        //���������� 
    _nop_();_nop_();_nop_();_nop_();
    IAP_ADDRL=0xff; 
    IAP_ADDRH=0xff; 
    IAP_CONTR=0x00; 
    IAP_CMD=0x00; 
    IAP_TRIG=0x00;   
} 
/*********************��������EEPROM�Ķ�д����**************************/
void ReadVoltage()//��ȡ����
{
  //�Ƿ��PWMƵ�ʵı�־�����ڴ洢�ռ�ĵ�ַ0��1�ĵط���
  //��ַ0��0xa0����ַ1��0xa1��
  u8 a0,a1;
  a0=EEPROM_Read(0); 
  a1=EEPROM_Read(1);
  if(a0==0xa0&&a1==0xa1)
  {
	  //u16��Fre�����ڴ洢�ռ�ĵ�ַ2��3�ĵط���
	  //��ַ2��Fre�ĸ�8λ����ַ3��Fre�ĵ�8λ��
	  Voltag=EEPROM_Read(2); //�ȶ���8λ
	  Voltag<<=8;			 //����8λ
	  Voltag+=EEPROM_Read(3);//�ټ��ϵ�8λ�����ݣ��㶨
  }
}
void SaveVoltage()//�������
{
  EEPROM_Eares(0);//����������ַΪ0����ֵ��Ҳ���ǲ�����0����
  EEPROM_Write(0,0xa0);//��洢�ĵ�ַ0������0xa0
  EEPROM_Write(1,0xa1);//��洢�ĵ�ַ1������0xa1
  EEPROM_Write(2,Voltag>>8);//��洢�ĵ�ַ2������Fre�ĸ�8λ
  EEPROM_Write(3,Voltag);//��洢�ĵ�ַ3������Fre�ĵ�8λ
}

//u8 Get_ADC8bitResult(u8 channel) //��ȡ8λ���ȵ�ADCֵ
//{
//	ADC_CONTR = ADC_POWER|ADC_SPEEDLL| ADC_START | channel; //��ָ��ͨ����ADCת��
//	_nop_();
//	_nop_();
//	_nop_();
//	_nop_(); //��ADC_CONTR������Ҫ4T֮����ܷ���
//	while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
//	ADC_CONTR &= ~ADC_FLAG;	//ADC_FLAG�ֹ���0 ,�ر� ADC
//    return ADC_RES;
//}


void InitADC(void)	//ADC��ʼ��
{
  P1ASF=0x00;//0x00��ʾ�����ھ�ͨ��bandgap��adcֵ��Ҳ���ǵ�Ƭ���ڲ���׼��ѹ��ֵ
  ADC_RES=0;//ADC��ֵ��0
  ADC_RESL=0;
  ADC_CONTR=ADC_POWER | ADC_SPEEDLL;//��ADC�ĵ�Դ 540��ʱ��ת��һ��
  DelayMs(2);//�ӳ�2ms�ȴ��ȶ�
}

u16 Get_ADC10bitResult(u8 channel) //��ȡ10λ���ȵ�ADCֵ
{
    u16	adc;
	if(channel > 7)	return	1024;	//����,����1024, ADCͨ����0~7�����ܳ���7	
	ADC_RES = 0; //��λADC����
	ADC_RESL = 0;//��λADC����

	ADC_CONTR = ADC_POWER|ADC_SPEEDLL| ADC_START | channel; //��ָ��ͨ����ADCת��
	_nop_();
	_nop_();
	_nop_();
	_nop_(); //��ADC_CONTR������Ҫ4T֮����ܷ���
							
	while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
	ADC_CONTR &= ~ADC_FLAG;	//ADC_FLAG�ֹ���0 ,�ر� ADC

    adc=ADC_RES;      //�õ���8λ
    adc<<=2;
    adc+=ADC_RESL;     //�õ���2λ

	return	adc;
} 

void Timer0Init(void)		//50����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xB0;		//���ö�ʱ��ֵ
	TH0 = 0x3C;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
    ET0 = 1;		//��ͨ��ʱ��0�ж�
  	EA  = 1;        //����ȫ���ж�
}

void GetBandGap()
{
	u8 code *cptr; //����ROM(����)��ָ��
	cptr = ID_ADDR_ROM;//�ӳ�������ȡBandGap��ѹֵ(��λ:����mV)
	bandgap=*cptr++;
	bandgap<<=8;
	bandgap+=*cptr;
}


void main()		
{
	DelayMs(100);//�ȴ�ϵͳ�ȶ�
	GetBandGap();
	P2M0=0x01;//P2.0��ǿ����	
	ReadVoltage();//��ȡ�洢�ĳ���ֹ��ѹ
	InitADC();
	QC_LcdInit();
	QC_ClearLCDGraphic();//���������򷴰׻��а׵㡣
	QC_ShowMyChar(); //̫�����ź͵�ط���
	QC_ShowString2(1,2,"̫���ܿ�����");
	QC_ShowString2(4,1,"��ֹ��ѹ:");
	Timer0Init();//��ʱ��0��ʼ��
	while(1)
	{
		if(Key1==0)
		{
			DelayMs(10);
			if(Voltag>=100)Voltag-=100;
			cnt2=0;
			IsSave=1;
 			while(Key1==0);//�ɿ��ٰ��²���Ч
		}
		
		if(Key2==0)
		{
			DelayMs(10);
			if(Voltag<65500)Voltag+=100; //u16������65535
			cnt2=0;
			IsSave=1;
 			while(Key2==0);//�ɿ��ٰ��²���Ч
		}
	}
}

void Timer0() interrupt 1  //50ms����
{
	u8 i;
	cnt++;
	if(cnt==1)//�����ھ�ͨ��bandgap��adcֵ��Ҳ���ǵ�Ƭ���ڲ���׼��ѹ��ֵ
	{
		adcsum=0;
		for(i=0;i<10;i++)
		{
			adc9=Get_ADC10bitResult(0);
			adcsum+=adc9;
		}
		adc9=adcsum/10;//ƽ������
		P1ASF = 0x0F;//��P1.0~P1.3��ADCת������ 0x0F=00001111 ��ӦP1.0~P1.3
	}
	
	if(cnt==2)//��������1
	{
		adcsum=0;
		for(i=0;i<10;i++)
		{
			adc0=Get_ADC10bitResult(0);
			adcsum+=adc0;
		}
		adc0=adcsum/10;//ƽ������
	}
	if(cnt==3)//��������2
	{
		adcsum=0;
		for(i=0;i<10;i++)
		{
			adc3=Get_ADC10bitResult(3);
			adcsum+=adc3;
		}
		adc3=adcsum/10;//ƽ������
	}

	if(cnt==4)//������ѹ1
	{
		adcsum=0;
		for(i=0;i<10;i++)
		{
			adc1=Get_ADC10bitResult(1);
			adcsum+=adc1;
		}
		adc1=adcsum/10;//ƽ������
	}

	if(cnt==5)//������ѹ2
	{
		adcsum=0;
		for(i=0;i<10;i++)
		{
			adc2=Get_ADC10bitResult(2);
			adcsum+=adc2;
		}
		adc2=adcsum/10;//ƽ������
	}

	if(cnt==6)//��ʾ��ѹ
	{
		
		V1=(float)bandgap*adc1*11/adc9;
		V2=(float)bandgap*adc2*11/adc9;

		lcd[0]=	V1/10000+48;
		lcd[1]=	V1%10000/1000+48;
		lcd[2]='.';	
		lcd[3]=	V1%1000/100+48;
		lcd[4]=	V1%100/10+48;
		lcd[5]='V';
		lcd[6]=' ';
		lcd[7]=' ';
		lcd[8]=' ';

		lcd[9]=	V2/10000+48;
		lcd[10]=V2%10000/1000+48;
		lcd[11]='.';	
		lcd[12]=V2%1000/100+48;
		lcd[13]=V2%100/10+48;
		lcd[14]='V';
		lcd[15]=0;

		if(lcd[0]=='0')lcd[0]=' ';
		if(lcd[9]=='0')lcd[9]=' '; 
		QC_ShowString(2,1,lcd);
	}

	if(cnt==7)//��ʾ����
	{
		I1=(float)bandgap*adc0*5/adc9;//0.2V��ʾ1A�����ԷŴ�5��������1A
		I2=(float)bandgap*adc3*5/adc9;//0.2V��ʾ1A�����ԷŴ�5��������1A
	    lcd[0]=	I1/10000+48;
		lcd[1]=	I1%10000/1000+48;
		lcd[2]='.';	
		lcd[3]=	I1%1000/100+48;
		lcd[4]=	I1%100/10+48;
		lcd[5]='A';
		lcd[6]=' ';
		lcd[7]=' ';
		lcd[8]=' ';

		lcd[9]=	I2/10000+48;
		lcd[10]=I2%10000/1000+48;
		lcd[11]='.';	
		lcd[12]=I2%1000/100+48;
		lcd[13]=I2%100/10+48;
		lcd[14]='A';
		lcd[15]=0;

		if(lcd[0]=='0')lcd[0]=' ';
		if(lcd[9]=='0')lcd[9]=' '; 
		QC_ShowString(3,1,lcd);
	}

	if(cnt==8)//��ʾ���µĽ�ֹ��ѹ
	{
		lcd[0]=':';
		lcd[1]=	Voltag/10000+48;
		lcd[2]=	Voltag%10000/1000+48;
		lcd[3]='.';	
		lcd[4]=Voltag%1000/100+48;	
		lcd[5]=Voltag%100/10+48;
		lcd[6]='V';
		lcd[7]=0;

		if(lcd[1]=='0')lcd[1]=' ';//��08.20Vǰ����Ǹ�0����ʾ��
		QC_ShowString(4,5,lcd);
		if(IsSave)ShowWhiteHang(4); //���׵����У���ʾ���ڵ���
		cnt=0;
		P1ASF = 0x00;//������׼��ѹ
	}

	cnt2++;
	if(IsSave==1&&cnt2==40)//ֹͣ������2�뱣�����
	{
		SaveVoltage();
		cnt2=0;
		IsSave=0;
		ShowBlackHang(4);//ȡ������
	}

	cnt3++;
	if(cnt3==1200)//60����һ�ε�ص�ѹ
	{
		Charge=0;//�ȴ�MOS�ܣ�
	}
	if(cnt3==1240)//��MOS��2����жϵ�ѹ
	{
 		//Ȼ���ж�V2�ĵ�ѹ
		if(!IsFull)//������û�б�������
		{
			if(V2<Voltag)Charge=0;//ֻҪ��صĵ�Դû����ֹ��ѹ����һֱ��
			else
			{
				IsFull=1;
				Charge=1;//�ص����
			}
		}
		else//��ر�������һ�κ�ֻ�е�ѹ�½����Ƚ�ֹ��ѹС0.5V��ʱ�򣬲����³��
		{
			if (V2>=Voltag-500)Charge=1;//�ص�����·
			else Charge=0;
		}

	    cnt3=0;
	}

}
