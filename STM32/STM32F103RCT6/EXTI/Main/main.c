#include "sys.h"
#include "led.h"
#include "time.h"
#include "delay.h"


int main(){
//    delay_init();
    
    //设置 NVIC 中断分组 2
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    //led初始化
    LedInit();
    //定时器3初始化
    Time3Init(4999,7199);//10Khz 的计数频率，计数到 5000 为 500ms
    
    while(1){
//        LED0 = !LED0;
//        delay_ms(200);
    }
}
