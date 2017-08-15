#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "iwdg.h"


int main() {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    delay_init(168);
    LedInit();
    KeyInit();
    delay_ms(100);
    IwdgInit(4,500);
    LED0 = 0;
    
    while(1){
//        LedFalsh();
//        delay_ms(500);
        if(KeyScan(0)){
            IWDG_ReloadCounter();
        };
    }
}
