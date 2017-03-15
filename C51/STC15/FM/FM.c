#include <reg52.h> 
#include"NOKIA5110.h"    
#include <intrins.h> 
#define uchar unsigned char 
#define uint unsigned int 
#define DELAY5US _nop_();_nop_();_nop_();_nop_();_nop_(); 
uchar idata adress_write;    // 写TEA5767地址 
uchar idata adress_read; //读TEA5767址 
uchar idata sbuf[5];     // 数据发送缓冲区 
uchar idata rbuf[5];     // 数据接收缓冲区 
uchar idata ampint[5]; 
uchar bdata I2C_byte1;  //发送的五字节ＴＥＡ５7６７可位寻址的设置值 
uchar bdata I2C_byte2; 
uchar bdata I2C_byte3; 
uchar bdata I2C_byte4; 
uchar bdata I2C_byte5; 
uchar   byte1;   
uchar   byte2; 
uchar   byte3; 
uchar   byte4; 
uchar   byte5; 
uchar bdata  rec_byte1;  //接收的五字节ＴＥＡ５7６７可位寻址的状态数据 
uchar bdata  rec_byte2; 
uchar bdata  rec_byte3; 
uchar bdata  rec_byte4; 
uchar bdata  rec_byte5; 
uchar idata ADDRESS_SEND;    //ＴＥＡ５7６７发送地址 
uchar idata ADDRESS_RECEIVE; //ＴＥＡ５7６７接收地址 
uchar idata ADDRESS_AMP; 
uchar idata numbyte; 
uchar idata numbyte_AMP; 
uchar num1,num2,num3,num4; 
unsigned long int  FM_FREQ;  //频率 
unsigned short int FM_PLL;   //PLL 
/*发送到TEA5767的５个字节设置数据中位的定义*/ 
sbit MUTE =I2C_byte1^7;//如果MUTE=1，则左右声道被静音；MUTE=0，左右声道正常工作   
sbit SM = I2C_byte1^6; //SM=1,则处于搜索模式；SM=0，不处于搜索模式 
uchar bdata PLL_HIGH;   
uchar bdata PLL_LOW;   //设定用于搜索和预设的可编程频率合成器 
sbit SUD=I2C_byte3^7;  //SUD=1，增加频率搜索；SUD=0，减小频率搜索 
sbit SSL1=I2C_byte3^6;  
sbit SSL0=I2C_byte3^5; //SSL1和SSL2，搜索停止标志 
sbit HLSI=I2C_byte3^4; //高/低充电电流切换：HLSI=1，高充电电流；HLSI=0，低充电电流 
sbit MS=I2C_byte3^3;   //MS=1，单声道；MS=0，立体声 
sbit MR=I2C_byte3^2;   //右声道静音，MR=1，右声道静音并置立体声；MR=0，右声道正常 
sbit ML=I2C_byte3^1;   //左声道静音，ML=1，左声道静音并置立体声；ML=0，左声道正常 
sbit SWP1=I2C_byte3^0; //软件可编程端口1：SWP1=1，端口1高电平；SWP1=0，端口1低电平 
sbit SWP2=I2C_byte4^7; //软件可编程端口2：SWP2=1，端口2高电平；SWP2=0，端口2低电平 
sbit STBY=I2C_byte4^6; //等待：STBY=1，处于待机状态；STBY=0，退出待机状态 
sbit BL=I2C_byte4^5;   //波段制式：BL=1，日本调频制式；BL=0，美国/欧洲调频制式 
sbit XTAL=I2C_byte4^4; //XTAL=1，fxtal=32.768khz;XTAL=0，fxtal=13khz 
sbit SMUTE=I2C_byte4^3;//软件静音，SMUTE=1，软静音打开；SMUTE=0，软静音关闭 
sbit HCC=I2C_byte4^2;  //白电平切割，HCC=1，高电平切割打开；HCC=0，高电平切割关闭 
sbit SNC=I2C_byte4^1;  //立体声噪声去除，SNC=1，立体声消噪打开；SNC=0，立体声消噪关闭。 
sbit SI=I2C_byte4^0;   //搜索标志位，SI=1，SWPORT1输出准备好信号；SI=0，SWPORT1作为软件可编程的端口1用 
sbit PLLREF=I2C_byte5^7;//PLLREF=1,6.5MHZ的锁相环参考频率启用；PLLREF=0,6.5MHZ的锁相环参考频率关闭 
sbit DTC=I2C_byte5^6;   //DTC=1，去加重时间常数为75us；DTC=0，去加重时间常数为50us 
sbit SDA=P2^6;      
sbit SCL=P2^7;  
bit   bdata NACK;      // 错误标志位 
bit   bdata nackFlag;    // 非应答标志位 /*接收I2C５个字节设置数据中位的定义*/ 
sbit RF=rec_byte1^7;     //准备好标志：RF=1，有一个频道被搜索到或者一个制式已经符合；RF=0，没有频道被搜到 
sbit BLF=rec_byte1^6;    //波段制式：BLF=1，一个制式已经符合；BLF=0，没有制式符合。 
sbit STEREO=rec_byte3^7; //立体声标志位，STEREO=1，立体声接收；STEREO，单声道接收 
uchar idata IF;          //中频计数器结果IF[6:0] 
uchar idata LEVEL;       //DAC输出 LEV[3:0] 
uchar idata CI;          //芯片验证CI[3:1] 
uchar temp1;  
uchar temp2; 
/*  函数预定义   */ 
void sendnbyte(uchar idata *sla, uchar n);//与sendbyte函数构成I2C 数据发送函数 
void sendbyte(uchar idata *ch);      
void I2C_start(void); //I2C 传输开始 
void stop(void);  //I2C传输结束 
void delay1ms(void); //延迟1ms 
void delay100ms(void); //延迟100ms 
void delay600ms(void);  //延迟600ms 
void search_up(void);   //接收频率向上加 
void search_down(void); //接收频率向下减 
void autosearch(void); //自动频率搜索 
void init(void);     //TEA5767初始化 
void key_scan(void); //键扫描 
void ask(void);   //问答讯号 
void n_ask(void);     //非应答讯号 
void checkack(void);   //检查应答讯号 
void recnbyte(uchar idata *sla, uchar n); //与recbyte一起构成接收I2C数据函数 
void recbyte(uchar idata *ch); 
void setByte1Byte2(void);  //设置第一第二字节频率 
void AUTO_search_up(void); //自动频率搜索 
void volume_up(void);//音量增加 
void volume_down(void);//音量减小 
void AMP_sendnbyte(uchar idata *sla,uchar numbyte_AMP); 

void init(void) {  
	uchar idata sbuf[5]={0XF0,0X2C,0XD0,0X10,0X40};  //ＦＭ模块预设值   
	uchar idata rbuf[5]={0X00,0X00,0X00,0X00,0X00};
	uchar idata ampint[5]={0X27,0X40,0X42,0X46,0XC3};   
	FM_PLL=0X302C; 
	FM_FREQ=100800000; //开机预设频率
	PLL_HIGH=0;   
	PLL_LOW=0;    
	delay100ms();    
	delay100ms();    
	P1=0XFF;    
	P2=0XFF; 
	I2C_byte1=0XF0;  //ＦＭ模块预设值    
	I2C_byte2=0X2C;    
	I2C_byte3=0XD0;    
	I2C_byte4=0X10;    
	I2C_byte5=0X40;    
	byte1=0X27;      
	byte2=0X40;    
	byte3=0X42;    
	byte4=0X46;    
	byte5=0XC3; 
	sendnbyte(&ADDRESS_SEND,numbyte);    
	delay100ms(); 
	AMP_sendnbyte(&ADDRESS_AMP,numbyte_AMP); 
} 

void I2C_start(void)   //在ＳＣＬ为高时，ＳＤＡ由高变低即为I2C传输开始 { 
	SDA=1;    
	SCL=1; 
	DELAY5US;    
	SDA=0;    
	DELAY5US;    
	SCL=0; 
} 

void stop(void){  //在ＳＣＬ为高时，ＳＤＡ由低变高即为I2C传输结束 
	SDA=0;  
	SCL=1;  
	DELAY5US;  
	SDA=1;  
	DELAY5US;  
	SCL=0; 
} 

/* 应答位检查子函数 */ 
void checkack(void) {   
	SDA = 1;      // 应答位检查（将p1.0设置成输入，必须先向端口写1）  
	SCL = 1;  
	F0 = 0;    
	DELAY5US; 
	// SDA=0;  
	if(SDA == 1)     // 若SDA=1表明非应答，置位非应答标志F0   
		F0 = 1;  
	SCL = 0; 
}  

/* 发送一个字节数据子函数 */ 
void sendbyte(uchar idata *ch)  {   
	uchar idata n = 8;    
	uchar idata temp;  
	temp = *ch;  
	while(n--)  {    
		if((temp&0x80) == 0x80){    // 若要发送的数据最高位为1则发送位1       
			SDA = 1;    // 传送位1    
			SCL = 1;    
			DELAY5US;       
			SCL = 0;     
			SDA = 0;         
		}else{      
			SDA = 0;    // 否则传送位0    
			SCL = 1;    
			DELAY5US;    
			SCL = 0;     
		}   
		temp = temp<<1;    // 数据左移一位  
	} 
} 

/* 发送n字节数据子程序 */ 
void sendnbyte(uchar idata *sla, uchar n) {            
	uchar idata *p; 
    sbuf[0]=I2C_byte1;     
	sbuf[1]=I2C_byte2;     
	sbuf[2]=I2C_byte3;     
	sbuf[3]=I2C_byte4;  
	I2C_start();   // 发送启动信号  
	sendbyte(sla);      // 发送从器件地址字节  
	checkack();       // 检查应答位     
	if(F0 == 1) {    
		NACK = 1;   
		return;      // 若非应答表明器件错误置错误标志位NACK  
	}  
	p = &sbuf[0];  
	while(n--)  {    
		sendbyte(p);   
		checkack();     // 检查应答位   
		if (F0 == 1){    
			NACK=1;    return;     // 若非应答表明器件错误置错误标志位NACK   
		}  
		p++;  
	}  
	stop();       // 全部发完则停止 
} 

void delay1ms(void) {           //延迟1ms 
	uchar i;  
	for(i=1000;i>0;i--){
		;
	} 
} 

void delay100ms() {   //延迟100ms 
	uchar i;  
	for(i=100;i>0;i--){
		delay1ms();
	} 
} 

void delay600ms() { 
	uchar i; 
	for(i=600;i>0;i--){
		delay1ms();
	} 
} 
void delay10ms(){    //延迟10ms 
    uchar i,j;   
	for(i=900;i>0;i--)   {
		for(j=100;j>0;j--){;}
	} 
} 

