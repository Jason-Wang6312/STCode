#include "led.h"

void LedInit(){
    GPIO_InitTypeDef LED_GPIO_Struct;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); //开启GPIOF的时钟
    
    LED_GPIO_Struct.GPIO_Mode = GPIO_Mode_OUT; //输入输出模式
    LED_GPIO_Struct.GPIO_OType = GPIO_OType_PP; //输出类型推挽
    LED_GPIO_Struct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;//IO口
    LED_GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    LED_GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;//时钟速度
    GPIO_Init(GPIOF,&LED_GPIO_Struct);//初始化
    
    GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);//点亮LED灯
}

void LedFalsh(void){
    static u8 poweroff = 1;
    
    if(poweroff)
        GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);//点亮LED灯
    else
        GPIO_ResetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);//点亮LED灯
    poweroff = !poweroff;
}
