#include "STC15Wxx.H"
#include "INTRINS.H"

sbit DHT12_SDA = P0^4;
typedef unsigned char u8;
typedef unsigned int u16;

u8 Sensor_AnswerFlag=0;//定义传感器响应标志
u8 Sensor_ErrorFlag;  //定义读取传感器错误标志

//初始化
void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x8F;		//设定定时初值
	TH1 = 0xFD;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}

void sendData(unsigned char dat){
    SBUF = dat;
    while(!TI);
    TI = 0;
}

void delayXus(unsigned int x){//@12.000MHz
    while(x--){
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

void delayXms(unsigned int x){//@12.000MHz
    unsigned char i, j;
    while(x--){
        i = 12;
        j = 169;
        do
        {
            while (--j);
        } while (--i);      
    }
}

/********************************************\
|* 功能： 读传感器发送的单个字节	        *|
\********************************************/
unsigned char DHT12_Single_ReadByte(void)
{
	u8 i;
	u16 j;
	u8 Data=0;
	u8 Bit=0;
	
	for(i=0;i<8;i++)
	{
		while(!DHT12_SDA)//检测上次低电平是否结束
		{
			if(++j>=50000) //防止进入死循环
			{
				break;
			}
		}
		//延时Min=26us Max70us 跳过数据"0" 的高电平		 
		delayXus(30);

		//判断传感器发送数据位
		Bit=0;
		if(DHT12_SDA)Bit=1;
		j=0;
		while(DHT12_SDA)	//等待高电平结束
		{
			if(++j>=50000) //防止进入死循环
			{
				break;
			}		
		}
		Data<<=1;
		Data|=Bit;
	}
	return Data;
}


void Sensor_Read(void)
{	
	u16 j;
	//主机发送起始信号
	DHT12_SDA=0;	//主机把数据总线（SDA）拉低
	delayXms(20);//拉低一段时间（至少18ms）， 通知传感器准备数据
	DHT12_SDA=1;	 //释放总线
	delayXus(30);//延时30us

	Sensor_AnswerFlag=0;	//传感器响应标志
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	if(DHT12_SDA==0)
	{
		Sensor_AnswerFlag=1;	//收到起始信号

		j=0;
		while(!DHT12_SDA) //判断从机发出 80us 的低电平响应信号是否结束	
		{
			if(++j>=500) //防止进入死循环
			{
				Sensor_ErrorFlag=1;
				break;
			}
		}

		j=0;
		while(DHT12_SDA)//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		{
			if(++j>=800) //防止进入死循环
			{
				Sensor_ErrorFlag=1;
				break;
			}		
		}
    }
}
        
void main(){
    
    EA = 1;
    UartInit();
    delayXms(500);

    while(1){
        Sensor_Read();
        sendData(DHT12_Single_ReadByte());
        sendData(DHT12_Single_ReadByte());
        sendData(DHT12_Single_ReadByte());
        sendData(DHT12_Single_ReadByte());
        sendData(DHT12_Single_ReadByte());
        
        delayXms(2000);
    }
}
