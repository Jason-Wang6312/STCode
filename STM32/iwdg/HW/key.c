#include "key.h"

void KeyInit(){
    GPIO_InitTypeDef KEY_GPIO_Struct;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA,ENABLE);
    
    KEY_GPIO_Struct.GPIO_Mode = GPIO_Mode_IN;
    KEY_GPIO_Struct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
    KEY_GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
    KEY_GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE,&KEY_GPIO_Struct);
    
    KEY_GPIO_Struct.GPIO_Pin = GPIO_Pin_0;
    KEY_GPIO_Struct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA,&KEY_GPIO_Struct);
}

//按键抛锚
u8 KeyScan(u8 isK){
    static u8 KeySt;
    if(isK) KeySt = 1;
    if(KeySt && (KeyU || !KeyD || !KeyL || !KeyR)){
        delay_ms(10);
        KeySt = 0;
        if(KeyU)
            return 1;
        else if(!KeyR)
            return 2;
        else if(!KeyD)
            return 3;
        else if(!KeyL)
            return 4;
        else 
            return 0;
    }else if(!KeyU && KeyD && KeyL && KeyR)
        KeySt = 1;
    return 0;
}

