#include "init.h"
#include "DS1302x.H"
unsigned char Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};

void Timer0Init();
void keyDriver();//按键驱动

//全局变量声明
u8 keySta[] = {1,1,1,1,1};//按键状态
u8 keyIndex;//按键索引

u8 ledIndex;
u8 ledTime;
u8 ledstr[] = {0x00,0x00,0x00,0x00};
u8 ledflash=5;
u8 flashsec;
bit flash;
u16 sec;

void main(){
   EA = 1;
    
    Timer0Init();
    DS1302Init();
    
    while(1){
        
        if(keyIndex == 2){
            P20 = ~P20;
            ledflash--;
            keyIndex = 0;
        }else if(keyIndex == 4){
            P21 = ~P21;
            ledflash++;
            keyIndex = 0;
        }else if(keyIndex == 5){
            P22 = ~P22;
            ledflash = 5;
            keyIndex = 0;
        }
        if(ledflash == 4){
            ledflash = 0;
        }else if(ledflash < 0){
            ledflash = 0;
        }
        if(sec > 200){
            sec=0;
            DS1302ReadTime();
            ledstr[0] = DS1302_Time[0] &0x0F;
            ledstr[1] = DS1302_Time[0] >> 4;
            ledstr[2] = DS1302_Time[1] &0x0F;
            ledstr[3] = DS1302_Time[1] >> 4;
        }
    }
}

void keyAction(){
    keyDriver();//按键驱动

}

void ledshow(u8 dat,u8 index){
    LED1 = LED2 = LED3 = LED4 = 0;
    P0 = dat;
    switch(index){
        case 0:
            LED1 = 1;
            break;
        case 1:
            LED2 = 1;
            break;
        case 2:
            LED3 = 1;
            break;
        case 3:
            LED4 = 1;
            break;
    }
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
    keybuf[3] = (keybuf[3] << 1) | K4;//按键左
    keybuf[4] = (keybuf[4] << 1) | K5;//按键中
    
    for(i=0;i<5;i++){//循环按键读取状态，消抖处理
    	if(((keybuf[i] & 0x0F) == 0x00)){//循环读取按键状态，如果4次都读取的是低电低，就说明按下了
    		keySta[i] = 0;//缓存状态
		}else if(keybuf[i] & 0x0F == 0x0F){//循环读取按键状态，如果4次都读取的是低电低，就说弹起了
			keySta[i] = 1;//缓存状态
		}
    }
}

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1;
}

void T0() interrupt 1{
    TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
    
    keyScan();
    
    sec++;
    ledTime++;
    if(ledTime > 3){
        ledTime=0;
        if(ledIndex == ledflash){
            flashsec++;
            if(flashsec > 15){
                flashsec = 0;
                flash = !flash;
            }
            if(flash){                
                ledshow(Disp_Tab[ledstr[ledIndex]],ledIndex);
            }else{
                LED1 = LED2 = LED3 = LED4 = 0;
            }
            
        }else
            ledshow(Disp_Tab[ledstr[ledIndex]],ledIndex);
        ledIndex++;
        ledIndex &= 0x03;
    }
}
