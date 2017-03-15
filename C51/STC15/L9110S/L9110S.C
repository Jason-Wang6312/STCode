#include "STC15F2K.H"

sbit IA = P3^3;
sbit IB = P3^2;

void left(){
	IA = 1;
	IB = 0;
}

void right(){
	IA = 0;
	IB = 1;
}

void stop(){
	IA = 0;
	IB = 0;
}

void main(){
	while(1){
		left();
	}
}