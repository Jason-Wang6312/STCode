#include "sys.h"
#include "led.h"
#include "delay.h"

int main(){
    
    //延时函数初始化
    delay_init();  
    //初始化led
    LedInit();

    while(1){
        GPIO_SetBits(GPIOD,GPIO_Pin_2);
        delay_ms(300);
        GPIO_ResetBits(GPIOD,GPIO_Pin_2);
        delay_ms(300);
    }
}
