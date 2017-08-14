#include "sys.h"
#include "led.h"
#include "delay.h"

 void Delay(u32 count)
 {
  u32 i=0;
  for(;i<count;i++);

 }

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
