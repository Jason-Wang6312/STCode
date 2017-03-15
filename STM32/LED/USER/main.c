#include "stm32f4xx.h"
#include "delay.h"
#include "LED.h"

int main() {

    delay_init(168);//初始化延时函数
    
    /********库函数版本**************/
//    LED_Init();//初始化LED
//    while(1){
//        GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
//        delay_ms(500);
//        GPIO_ResetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
//        delay_ms(500);
//    }
    
    /********寄存器版本**************/
    LED_SFR();
    while(1){
        GPIOF->ODR |= 1 << 9; 
        delay_ms(500);
        GPIOF->ODR &= 0 << 9;
        delay_ms(500);
    }
}
