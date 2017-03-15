#include "STC12C5A.H"

sbit Inf_IO = P3^2;

void main(){
	
}

unsigned int GetHighTime(){
	TH1 = 0;
	TL1 = 0;
	TR1 = 1;
	while(Inf_IO){
		if(TH1 >= 0x40){
			break;
		}
	}
}

unsigned int GetLowTime(){
	TH1 = 0;
	TL1 = 0;
	TR1 = 1;
	while(!Inf_IO){
		if(TH1 >= 0x40){
			break;
		}
	}
}

void exint() interrupt 2{
	u16 time;
	u8 i,j,byt;
	
	if((time < 7833)) || (time > 8755)){
		IE1 = 0;
		return ;
	}
}
