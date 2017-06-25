#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "pwm.h"

int main(void){

    u16 led0pwmval = 0;
    u8 dir = 1;
    delay_init(168);
    TIM14_PWM_Init(400-1,84-1);
    
    while(1){
        delay_ms(10);
        
        if(dir)led0pwmval++;//dir==1 led0pwmval递增
		else led0pwmval--;	//dir==0 led0pwmval递减 
 		if(led0pwmval>200)dir=0;//led0pwmval到达300后，方向为递减
		if(led0pwmval==0)dir=1;	//led0pwmval递减到0后，方向改为递增
 
		TIM_SetCompare1(TIM14,led0pwmval);	//修改比较值，修改占空比
    }
}



