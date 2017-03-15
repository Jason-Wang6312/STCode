#include "DS18B20.H"

/**
 *	DS18B20 ≥ı ºªØ
 *	Author:MaWei
 *	Date:2016/06/27
**/
bit DS18B20_Init(){
	bit ack;

	DS18B20_IO = 0;
	delayX1us(600);
	DS18B20_IO = 1;
	delayX1us(60);
	ack = DS18B20_IO;
	while(!DS18B20_IO);

	return ack;
}

void W_DS18B20(uint8 dat){
	
}

