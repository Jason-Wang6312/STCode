#include "STC15Wxx.h"
#include "eeprom.h"

typedef unsigned char u8;
typedef unsigned int u16;

//四位数码管编码
u8 code LEDAnodeCode[16] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
//u8 code LEDCathodeCode[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

//IO定义
//按键
sbit K1=P3^3;
sbit K2=P3^1;
sbit K3=P3^0;

//红外
sbit IR_IO = P3^2;          // IR管脚 任意IO

//led
sbit LA = P1^0;
sbit LB = P1^1;
sbit LC = P1^2;
sbit LD = P1^3;
sbit LE = P1^4;
sbit LF = P1^5;
sbit LG = P5^4;
sbit LH = P3^7;

//风扇控制
sbit fanIO = P3^6;

//函数声明
void Timer0Init();//定时器0初始化
void Timer2Init();//定时器2
void DelayXms(u16 n);//软件延时
void keyDriver();//按键驱动
void LedShow(u8 num);//LED显示
void setFanTime();
void initFanTime();
void writeFanTime();
void UartInit();
void SendData(unsigned char data_buf);
 
//全局变量声明

//按键
u8 keySta[] = {1,1,1,1,1};//按键状态
u8 keyIndex;//按键索引
u8 keyFanStatus = 0;
bit keyIsDown = 0;
u16 keyDelayTime = 0;

//红外线
bit Irprot_LastState = 0;   // 端口状态位
u8 codeCnt = 0;          // 数据码位计数
u8 irTime;                   // 码时间，用于以125us时间计时
u8 IR_data[4]={48,49,50,51}; // 接收数据缓存

//风扇时间
u8 fanRun = 6;
u8 fanStop = 10;
u8 fanTime = 0;
bit fanStatus = 0;
u8 LedNum = 0;

u16 cnt = 0;

void main(){
    EA = 1;
    
    //初始化定时器2
    Timer2Init();
//    UartInit();
    //初始化风扇运、停时间
    initFanTime();
    DelayXms(500);
    fanIO = 0;
    
    while(1){
        if(cnt > 1000){
            cnt = 0;
            fanTime--;
            LedShow(fanTime);
        }
        if(fanTime <= 0){
            if(fanStatus){
                fanTime = fanRun;
            }else{
                fanTime = fanStop;
            }
            fanStatus = ~fanStatus;
            fanIO = ~fanIO;
        }
        setFanTime();
        //
        if(keyDelayTime > 5000){
            keyDelayTime = 0;
            keyIsDown = 0;
            writeFanTime();
        }
    }
}

//初始化风扇运、停时间
void initFanTime(){
    fanRun = EepromReadByte(0x0000);
    fanStop = EepromReadByte(0x0001);
    if(fanRun == 0xFF){
        fanRun = 5;
        fanStop = 10;
        writeFanTime();
        SendData(0xAA);
    }
}

//风扇运、停时间并写入eeprom
void writeFanTime(){
    EepromEraseSector(0x0000);
    EepromWriteByte(0x0000,fanRun);
    EepromWriteByte(0x0001,fanStop);
}

//设置风扇运行、停止时间
void setFanTime(){
    //按键驱动
    keyDriver();
    switch(keyIndex){
        case 1:
            keyFanStatus = ~keyFanStatus;
            P55 = ~keyFanStatus;
            break;
        case 2:
            if(keyFanStatus)
                fanRun++;
            else
                fanStop++;
            //按键按下延时把风扇运、停时间写入到eeprom
            keyIsDown = 1;
            keyDelayTime = 0;
            break;
        case 3:
            if(keyFanStatus)
                fanRun--;
            else
                fanStop--;
            //按键按下延时把风扇运、停时间写入到eeprom
            keyIsDown = 1;
            keyDelayTime = 0;
            break;
    }
    keyIndex = 0;//擦除按键
}


//LED显示
void LedShow(u8 num){
    static u8 dat;
    dat = LEDAnodeCode[num];
    P1 = dat;
    LG = (0x40&dat);
    LH = fanStatus;
}

//按键驱动
void keyDriver(){
    static keybak[] = {1,1,1,1,1};//按键上次状态备份
    u8 i;
     
    for(i=0;i<5;i++){//循环按键状态处理
        if(keybak[i] != keySta[i]){//如果这次状态和上次状态不一样，说明有按键动作
            if(keybak[i] == 1){//如果上次是弹起，又与上次状态不一样，说明按下了
                keyIndex = i+1;//输出按键索引
            }
        }
        keybak[i] = keySta[i];//把这次状态保存
    }
}
 
//按键扫描程序
void keyScan(){
    u8 i;
    static keybuf[] = {0xFF,0xFF,0xFF,0xFF,0xFF};//按键扫描缓存
     
    //将当前的状态移入缓存
    keybuf[0] = (keybuf[0] << 1) | K1;//按键上
    keybuf[1] = (keybuf[1] << 1) | K2;//按键右
    keybuf[2] = (keybuf[2] << 1) | K3;//按键下
     
    for(i=0;i<3;i++){//循环按键读取状态，消抖处理
        if(((keybuf[i] & 0x0F) == 0x00)){//循环读取按键状态，如果4次都读取的是低电低，就说明按下了
            keySta[i] = 0;//缓存状态
        }else if(keybuf[i] & 0x0F == 0x0F){//循环读取按键状态，如果4次都读取的是低电低，就说弹起了
            keySta[i] = 1;//缓存状态
        }
    }
}

void Timer0Init(void)        //125微秒@12.000MHz
{
    AUXR |= 0x80;        //定时器时钟1T模式
    TMOD &= 0xF0;        //设置定时器模式
    TL0 = 0x24;        //设置定时初值
    TH0 = 0xFA;        //设置定时初值
    TF0 = 0;        //清除TF0标志
    TR0 = 1;        //定时器0开始计时
    ET0=1;            //打开定时器0中断
    EA=1;            //打开全部中断
}

void Timer2Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x20;		//设置定时初值
	T2H = 0xD1;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
    IE2 = 0x04;
//    IE2 = 1; //开启定时器2中断
}

void UartInit(void)        //4800bps@12.000MHz
{
    SCON = 0x50;        //8位数据,可变波特率
    AUXR |= 0x01;        //串口1选择定时器2为波特率发生器
    AUXR |= 0x04;        //定时器2时钟为Fosc,即1T
    T2L = 0x8F;        //设定定时初值
    T2H = 0xFD;        //设定定时初值
    AUXR |= 0x10;        //启动定时器2
}

//定时器0中断函数 125us
void Timer0() interrupt 1
{
   irTime++; //计时增加125us
   if(irTime==240) {irTime--;  codeCnt=0x3f;} // ir解码后码值存放时间， 240*125us = 30ms  0x3f=64
   if(IR_IO)   Irprot_LastState=1; // 记录IO状态
   else if(Irprot_LastState)       // 有下降沿，并且上个状态是高电平，表示红外管收到数据
   {
      Irprot_LastState = 0;        // 下降沿后IO状态记录为0
      if(irTime<24) // 小于24*125us=3ms的间隔才进行处理    因为红外线的0的周期1.125ms，1的周期2.25ms
      {
         codeCnt++; //数据码位计数+1
         codeCnt &= 0x1f; //等效if(codeCnt>0x1f) codeCnt=0x00; 这种操作比if判断更节约时间 0x1f=31
         IR_data[codeCnt>>3] <<= 1; //codeCnt>>3的范围(0~3)，等效于IR_data[i]向左移动1位 (i范围0~3)
         if( irTime>15 )  IR_data[codeCnt>>3]++;  //大于15*125us=1.875ms的间隔为数据1 ，1就+1，0就不变。
      }
      irTime = 0;                  // 下降沿处理完成，将时间清0
   }    
}
//定时器2中断函数 1ms
void Timer2() interrupt 12{
    keyScan();//按键扫描
    //LED显示
    cnt++;
    
    if(keyIsDown) keyDelayTime++;
}

void DelayXms(unsigned int x){ //@12.000MHz
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

void SendData(unsigned char data_buf) //发送一个字符
{
	SBUF = data_buf;
	while(!TI);//TI是发送成功标志
	TI = 0;
}