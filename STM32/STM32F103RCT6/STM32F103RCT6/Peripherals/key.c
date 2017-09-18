#include "key.h"

//按键初始化设置
void KeyInit(){
    //定义GPIOC结构体声明
    GPIO_InitTypeDef GPIOC_InitStruct;
    
    //使能GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    //初始化GPIO
    GPIOC_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIOC_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //模式为上拉输入
    GPIOC_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIOC_InitStruct);
    
}

//按键扫描
u8 KeyScan(){
//    static u16 keycache[] = {0x0000,0x0000,0x0000,0x0000};
    
    return Key0;
}

