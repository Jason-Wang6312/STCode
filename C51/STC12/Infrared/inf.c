#include "STC12C5A.H"


void T0INIT(){
	  INT0 = 1;
    IT0 = 1;                    //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
    EX0 = 1;                    //使能INT0中断
}

void T0() interrupt 0{
	
}
