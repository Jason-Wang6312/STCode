#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "time.h"

int main(void){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    delay_init(168);//延时初始化
    LedFunInit(); //led 初始化
    Time3Init(5000-1,8400-1);//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms 
    
    while(1){
        LED0 = !LED0;
        delay_ms(250);//延时250ms
    }
}



