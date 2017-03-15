#include "stm32f4xx.h"
#include "delay.h"
#include "LED.h"
#include "Beep.h"

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
    Beep_Init_Fun();
    while(1){
        GPIOF->ODR |= 3 << 8; 
        delay_ms(500);
        GPIOF->ODR &= ~(3 << 8);
        delay_ms(500);
    }
}
