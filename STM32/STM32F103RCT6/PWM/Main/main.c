#include "sys.h"
#include "pwm.h"
#include "led.h"
#include "delay.h"

int main(){
    u16 led0pwmval=0;
    u8 dir=1;
    delay_init();  //延时函数初始化 
//    LedInit();  //初始化与 LED 连接的硬件接口
    Tim1PwmInit(899,0);//不分频。PWM 频率=72000/(899+1)=80Khz
    while(1){
        delay_ms(10);
        if(dir)led0pwmval++;
        else led0pwmval--;
        if(led0pwmval>300)dir=0;
        if(led0pwmval==0)dir=1; 
        TIM_SetCompare1(TIM1,led0pwmval); 
    }
}

//int main(){
//    delay_init();  //延时函数初始化 
//    LedInit();  //初始化与 LED 连接的硬件接口
//    
//    while(1){
//        GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_2);
//        delay_us(100);
//        GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_2);
//        delay_us(1000);
//    }
//}
