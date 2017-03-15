#include <STC15F2K60S2.H>
#include <intrins.h>

#define u8 unsigned char
#define u16  unsigned int

#define ID_ADDR_ROM 0x1ff7//8K程序空间的MCU

#define ADC_POWER	0x80//ADC电源控制位
#define ADC_FLAG	0x10//ADC完成标志
#define ADC_START	0x08//ADC起始控制位
#define ADC_SPEEDLL	0x00//540个时钟转换一次
#define ADC_SPEEDL	0x20//360时钟
#define ADC_SPEEDH	0x40//180时钟
#define ADC_SPEEDHH	0x60//90时钟


sbit QC_CS=P2^5;	    //片选信号
sbit QC_SID=P2^4;		//数据信号
sbit QC_SCLK=P2^3;		//时钟信号 
//sbit QC_RST=P3^2;		//复位信号	 实测可以不用
//sbit CH = P3^4;		//并行、串行选择信号   R10焊死，不用选择串口和并口了

sbit Key1=P2^6;
sbit Key2=P2^7;
sbit Charge=P2^0;//控制充电的MOS

u8 lcd[16];
u8 cnt=0;
u8 cnt2=0;
u8 cnt3=0;
u16 Voltag=14400;//充电截止电压 14400V
bit IsSave=0;//是否要保存参数
u16 V1,I1,V2,I2;
bit IsFull=0;//电池是否充满
u16 bandgap;//bandgap预储存校准值，单位毫伏
u16	adc0,adc1,adc2,adc3,adc9;//测量第九通道（bandgap）值
u16 adcsum;
/************************12864代码*****************************/

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
//	QC_RST = 0;	 //测试不用重启也能用的样子，暂时去掉了
//	QC_Delay(100);
//	QC_RST = 1;

	QC_WriteCom(0x30);//显示模式设置
	QC_Delay(50);
	QC_WriteCom(0x30);//2次显示模式
	QC_Delay(50);
	QC_WriteCom(0x08);//设置显示、光标、闪烁全关。
	QC_Delay(50);
	QC_WriteCom(0x01);//显示清屏
	QC_Delay(50);
	QC_WriteCom(0x06);//进入模式设置
	QC_Delay(50);
	QC_WriteCom(0x0C);//打开显示
	QC_Delay(50);

}

//设置行列地址，QC12864地址有4行，8列，每列2个字节。
void QC_WriteAC(unsigned char hang,unsigned char lie)
{
    if(hang == 1)	QC_WriteCom(0x80+lie-1);
	if(hang == 2)	QC_WriteCom(0x90+lie-1);
	if(hang == 3)	QC_WriteCom(0x88+lie-1);
	if(hang == 4)	QC_WriteCom(0x98+lie-1);
}

//汉字，4行，每行8个汉字
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
//汉字，4行，每行8个汉字
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
//反白指定的一行。
void ShowWhiteHang(unsigned char hang)
{
  unsigned char i,j;
  QC_WriteCom(0x36);//开启画图指令
  //*******显示上半屏内容设置
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
	  for(j=9;j<15;j++)QC_WriteData(0xFF);//反白14.4V 这5个字符
	  QC_WriteData(0x00);
    }	   

  QC_WriteCom(0x30);//返回字符指令
}


//取消反白指定的一行。
void ShowBlackHang(unsigned char hang)
{
  unsigned char i,j;
  QC_WriteCom(0x36);//开启画图指令
  //*******显示上半屏内容设置
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

  QC_WriteCom(0x30);//返回字符指令
}


//自定义字符，没啥用

void QC_ShowMyChar()
{
    unsigned char i;
	unsigned char code defchar[32] =  //太阳符号
	 {0x01,0x00,0x41,0x04,0x21,0x08,0x11,0x10,0x07,0xC0,0x08,0x20,0x08,0x20,0xF8,0x3E,
      0x08,0x20,0x08,0x20,0x07,0xC0,0x11,0x10,0x21,0x08,0x41,0x04,0x01,0x00,0x00,0x00}; 
	unsigned char code defchar2[32] = //电池符号
	{0x00,0x00,0x03,0xC0,0x0F,0xF0,0x10,0x08,0x10,0x08,0x17,0xE8,0x17,0xE8,0x10,0x08,
    0x17,0xE8,0x17,0xE8,0x10,0x08,0x17,0xE8,0x17,0xE8,0x10,0x08,0x0F,0xF0,0x00,0x00};
    
 	QC_WriteCom(0x40);//自定义符号最大4个16*16 地址分别是0x40 50 60 70

    for(i = 0; i < 32; i++)	//写第一个自定义符号的点阵数据
	{
	  QC_WriteData(defchar[i]);
    }
	QC_WriteCom(0x80);//0x80表示第一行第1个位置
    QC_WriteData(0x00);//列
	QC_WriteData(0x00);//0x00表示显示第一个自定义符号

    //第二个电池符号
    QC_WriteCom(0x50);

    for(i = 0; i < 32; i++)
	{
	  QC_WriteData(defchar2[i]);
    }
	QC_WriteCom(0x80+7);//0x80+7表示第一行第8个位置
    QC_WriteData(0x00);
	QC_WriteData(0x02);//0x02表示显示第二个自定义符号

	//第三个0x60 0x04
	//第四个0x70 0x06
}

//清除画图
void QC_ClearLCDGraphic()
{
  unsigned char i,j;
  QC_WriteCom(0x36);//开启画图指令
  //*******显示上半屏内容设置
   for(i=0;i<32;i++)              //
    { 
      QC_WriteCom(0x80 + i); //SET  垂直地址 VERTICAL ADD	  
      QC_WriteCom(0x80);       //SET  水平地址 HORIZONTAL ADD  
      for(j=0;j<16;j++)	QC_WriteData(0x00);
    }	   
//*******显示下半屏内容设置
   for(i=0;i<32;i++)              //
    {
      QC_WriteCom(0x80 + i); //SET  垂直地址 VERTICAL ADD	  
      QC_WriteCom(0x88);       //SET  水平地址 HORIZONTAL ADD  
      for(j=0;j<16;j++)	QC_WriteData(0x00);
    }
	QC_WriteCom(0x30);//返回字符指令
}
/**********************12864代码结束**********************************/

void Delay1ms()		//@12.000MHz   软件延时1ms
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}

void DelayMs(unsigned int i)  //延时N毫秒，0~65535ms
{
   while(--i)
   {
	 Delay1ms();
   }  
}

/*************EEPROM读写*************/
/*********************开始复制EEPROM的读写代码**************************/ 
//  扇区擦除 
//---------------------------------------------------------------- 
void EEPROM_Eares(unsigned int addres) 
{      
    IAP_ADDRL=addres;     //低位地址 
    IAP_ADDRH=addres>>8;  //高位地址 
    IAP_CONTR=0x01;  
    IAP_CONTR=IAP_CONTR|0x82; //设时间与充ISP操作。 
    IAP_CMD=0x03;         //扇区命命令 
    IAP_TRIG=0x5a;        //触发 
    IAP_TRIG=0xa5;        //触发启动。 
	_nop_();_nop_();_nop_();_nop_();
    IAP_ADDRL=0xff; 
    IAP_ADDRH=0xff; 
    IAP_CONTR=0x00; 
    IAP_CMD=0x00; 
    IAP_TRIG=0x00;  
} 
//**************************************************************** 
//  读数据 
//---------------------------------------------------------------- 
unsigned char EEPROM_Read(unsigned int addres) 
{ 
    IAP_ADDRL=addres;     //低位地址 
    IAP_ADDRH=addres>>8;  //高位地址 
    IAP_CONTR=0x01;  
    IAP_CONTR=IAP_CONTR|0x82; //设时间与充ISP操作。 
    IAP_CMD=0x01;         //写命令 
    IAP_TRIG=0x5a;        //触发 
    IAP_TRIG=0xa5;        //触发启动。 
    _nop_();_nop_();_nop_();_nop_(); 
    IAP_ADDRL=0xff; 
    IAP_ADDRH=0xff; 
    IAP_CONTR=0x00; 
    IAP_CMD=0x00; 
    IAP_TRIG=0x00; 
    
    return(IAP_DATA);         
} 
//**************************************************************** 
//  写数据 
//---------------------------------------------------------------- 
void EEPROM_Write(unsigned int addres,unsigned char write_data) 
{ 
    IAP_DATA=write_data;  //要写入的数据。 
    IAP_ADDRL=addres;     //低位地址 
    IAP_ADDRH=addres>>8;  //高位地址 
    IAP_CONTR=0x01;  
    IAP_CONTR=IAP_CONTR|0x82; //设时间与充ISP操作。 
    IAP_CMD=0x02;         //写命令 
    IAP_TRIG=0x5a;        //触发 
    IAP_TRIG=0xa5;        //触发启动。 
    _nop_();_nop_();_nop_();_nop_();
    IAP_ADDRL=0xff; 
    IAP_ADDRH=0xff; 
    IAP_CONTR=0x00; 
    IAP_CMD=0x00; 
    IAP_TRIG=0x00;   
} 
/*********************结束复制EEPROM的读写代码**************************/
void ReadVoltage()//读取参数
{
  //是否读PWM频率的标志，放在存储空间的地址0和1的地方。
  //地址0放0xa0，地址1放0xa1。
  u8 a0,a1;
  a0=EEPROM_Read(0); 
  a1=EEPROM_Read(1);
  if(a0==0xa0&&a1==0xa1)
  {
	  //u16的Fre，放在存储空间的地址2和3的地方。
	  //地址2放Fre的高8位，地址3放Fre的低8位。
	  Voltag=EEPROM_Read(2); //先读高8位
	  Voltag<<=8;			 //左移8位
	  Voltag+=EEPROM_Read(3);//再加上低8位的数据，搞定
  }
}
void SaveVoltage()//保存参数
{
  EEPROM_Eares(0);//先清除闪存地址为0的数值，也就是擦除第0扇区
  EEPROM_Write(0,0xa0);//向存储的地址0，保存0xa0
  EEPROM_Write(1,0xa1);//向存储的地址1，保存0xa1
  EEPROM_Write(2,Voltag>>8);//向存储的地址2，保存Fre的高8位
  EEPROM_Write(3,Voltag);//向存储的地址3，保存Fre的低8位
}

//u8 Get_ADC8bitResult(u8 channel) //获取8位精度的ADC值
//{
//	ADC_CONTR = ADC_POWER|ADC_SPEEDLL| ADC_START | channel; //打开指定通道的ADC转换
//	_nop_();
//	_nop_();
//	_nop_();
//	_nop_(); //对ADC_CONTR操作后要4T之后才能访问
//	while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
//	ADC_CONTR &= ~ADC_FLAG;	//ADC_FLAG手工清0 ,关闭 ADC
//    return ADC_RES;
//}


void InitADC(void)	//ADC初始化
{
  P1ASF=0x00;//0x00表示测量第九通道bandgap的adc值，也就是单片机内部基准电压的值
  ADC_RES=0;//ADC的值清0
  ADC_RESL=0;
  ADC_CONTR=ADC_POWER | ADC_SPEEDLL;//打开ADC的电源 540个时钟转换一次
  DelayMs(2);//延迟2ms等待稳定
}

u16 Get_ADC10bitResult(u8 channel) //获取10位精度的ADC值
{
    u16	adc;
	if(channel > 7)	return	1024;	//错误,返回1024, ADC通道号0~7，不能超过7	
	ADC_RES = 0; //高位ADC清零
	ADC_RESL = 0;//低位ADC清零

	ADC_CONTR = ADC_POWER|ADC_SPEEDLL| ADC_START | channel; //打开指定通道的ADC转换
	_nop_();
	_nop_();
	_nop_();
	_nop_(); //对ADC_CONTR操作后要4T之后才能访问
							
	while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
	ADC_CONTR &= ~ADC_FLAG;	//ADC_FLAG手工清0 ,关闭 ADC

    adc=ADC_RES;      //得到高8位
    adc<<=2;
    adc+=ADC_RESL;     //得到低2位

	return	adc;
} 

void Timer0Init(void)		//50毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xB0;		//设置定时初值
	TH0 = 0x3C;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1;		//开通定时器0中断
  	EA  = 1;        //开启全部中断
}

void GetBandGap()
{
	u8 code *cptr; //定义ROM(代码)区指针
	cptr = ID_ADDR_ROM;//从程序区读取BandGap电压值(单位:毫伏mV)
	bandgap=*cptr++;
	bandgap<<=8;
	bandgap+=*cptr;
}


void main()		
{
	DelayMs(100);//等待系统稳定
	GetBandGap();
	P2M0=0x01;//P2.0开强推挽	
	ReadVoltage();//读取存储的充电截止电压
	InitADC();
	QC_LcdInit();
	QC_ClearLCDGraphic();//清屏。否则反白会有白点。
	QC_ShowMyChar(); //太阳符号和电池符号
	QC_ShowString2(1,2,"太阳能控制器");
	QC_ShowString2(4,1,"截止电压:");
	Timer0Init();//定时器0初始化
	while(1)
	{
		if(Key1==0)
		{
			DelayMs(10);
			if(Voltag>=100)Voltag-=100;
			cnt2=0;
			IsSave=1;
 			while(Key1==0);//松开再按下才有效
		}
		
		if(Key2==0)
		{
			DelayMs(10);
			if(Voltag<65500)Voltag+=100; //u16的上限65535
			cnt2=0;
			IsSave=1;
 			while(Key2==0);//松开再按下才有效
		}
	}
}

void Timer0() interrupt 1  //50ms周期
{
	u8 i;
	cnt++;
	if(cnt==1)//测量第九通道bandgap的adc值，也就是单片机内部基准电压的值
	{
		adcsum=0;
		for(i=0;i<10;i++)
		{
			adc9=Get_ADC10bitResult(0);
			adcsum+=adc9;
		}
		adc9=adcsum/10;//平均数法
		P1ASF = 0x0F;//打开P1.0~P1.3的ADC转换功能 0x0F=00001111 对应P1.0~P1.3
	}
	
	if(cnt==2)//测量电流1
	{
		adcsum=0;
		for(i=0;i<10;i++)
		{
			adc0=Get_ADC10bitResult(0);
			adcsum+=adc0;
		}
		adc0=adcsum/10;//平均数法
	}
	if(cnt==3)//测量电流2
	{
		adcsum=0;
		for(i=0;i<10;i++)
		{
			adc3=Get_ADC10bitResult(3);
			adcsum+=adc3;
		}
		adc3=adcsum/10;//平均数法
	}

	if(cnt==4)//测量电压1
	{
		adcsum=0;
		for(i=0;i<10;i++)
		{
			adc1=Get_ADC10bitResult(1);
			adcsum+=adc1;
		}
		adc1=adcsum/10;//平均数法
	}

	if(cnt==5)//测量电压2
	{
		adcsum=0;
		for(i=0;i<10;i++)
		{
			adc2=Get_ADC10bitResult(2);
			adcsum+=adc2;
		}
		adc2=adcsum/10;//平均数法
	}

	if(cnt==6)//显示电压
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

	if(cnt==7)//显示电流
	{
		I1=(float)bandgap*adc0*5/adc9;//0.2V表示1A，所以放大5倍，就是1A
		I2=(float)bandgap*adc3*5/adc9;//0.2V表示1A，所以放大5倍，就是1A
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

	if(cnt==8)//显示最新的截止电压
	{
		lcd[0]=':';
		lcd[1]=	Voltag/10000+48;
		lcd[2]=	Voltag%10000/1000+48;
		lcd[3]='.';	
		lcd[4]=Voltag%1000/100+48;	
		lcd[5]=Voltag%100/10+48;
		lcd[6]='V';
		lcd[7]=0;

		if(lcd[1]=='0')lcd[1]=' ';//让08.20V前面的那个0不显示。
		QC_ShowString(4,5,lcd);
		if(IsSave)ShowWhiteHang(4); //反白第四行，表示正在调整
		cnt=0;
		P1ASF = 0x00;//测量基准电压
	}

	cnt2++;
	if(IsSave==1&&cnt2==40)//停止调整后2秒保存参数
	{
		SaveVoltage();
		cnt2=0;
		IsSave=0;
		ShowBlackHang(4);//取消反白
	}

	cnt3++;
	if(cnt3==1200)//60秒检测一次电池电压
	{
		Charge=0;//先打开MOS管，
	}
	if(cnt3==1240)//打开MOS的2秒后判断电压
	{
 		//然后判断V2的电压
		if(!IsFull)//如果电池没有被充满过
		{
			if(V2<Voltag)Charge=0;//只要电池的电源没到截止电压，就一直充
			else
			{
				IsFull=1;
				Charge=1;//关掉充电
			}
		}
		else//电池被充满过一次后，只有电压下降到比截止电压小0.5V的时候，才重新充电
		{
			if (V2>=Voltag-500)Charge=1;//关掉充电电路
			else Charge=0;
		}

	    cnt3=0;
	}

}
