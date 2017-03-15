#include "ds18b20.h"

void delay1xus(u16 t){
	while(t--){
		_nop_();
		_nop_();
	}
}

//初始化DS18B20
bit DS18B20Init(){
	bit stat = 1;
	
	DS18B20_DQ = 0;
	delay1xus(600);
	DS18B20_DQ = 1;
    delay1xus(60);
	stat = DS18B20_DQ;
	while(!DS18B20_DQ);
	P27 = stat;
	
	return stat;
}

//DS18B20写一个字节
void DS18B20Writ(u8 dat){
	u8 i=0;
	
	for(;i < 8;i++){
		DS18B20_DQ = 0;
		delay1xus(2);
		if((dat>>i)&0x01)
			DS18B20_DQ = 1;
		delay1xus(60);
		DS18B20_DQ = 1;
		delay1xus(10);
	}

}

//DS18B20读一个字节
u8 DS18B20Read(){
	u8 i=0,dat = 0x00;

	for(;i < 8;i++){
		DS18B20_DQ = 0;
		delay1xus(2);
		DS18B20_DQ = 1;
		delay1xus(2);
		if(DS18B20_DQ)
			dat |= 1 << i;
		delay1xus(60);
	}
	return dat;
}

//开始转换温度命令
void DS1B20CT(){
	DS18B20Init();
	delay1xus(5);
	DS18B20Writ(0xCC);
	DS18B20Writ(0x1F);
	DS18B20Init();
	delay1xus(5);
	DS18B20Writ(0xCC);
	DS18B20Writ(0x44);
}

//给DS18B20发送读温度命令
void DS1B20ReadCmd(){
	DS18B20Init();
	delay1xus(5);
	DS18B20Writ(0xCC);
	DS18B20Writ(0xBE);
}

//获取温度
u16 getTemp(){
	u8 LSB,MSB;
	u16 temp;
	
	EA = 0;
	DS1B20CT();
	DS1B20ReadCmd();
	LSB = DS18B20Read();
	MSB = DS18B20Read();
	temp = ((int)MSB << 8) + LSB;
	
	EA = 1;
	return temp;
} 



