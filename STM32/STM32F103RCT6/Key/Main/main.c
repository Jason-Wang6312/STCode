#include "sys.h"
#include "led.h"
#include "key.h"

int main(){
    
    //初始化led
    LedInit();
    //按键初始化
    KeyInit();

    while(1){
        if(KeyScan()){
            GPIO_SetBits(GPIOD, GPIO_Pin_2);
        }else{
            GPIO_ResetBits(GPIOD, GPIO_Pin_2);
        }
    }
}
