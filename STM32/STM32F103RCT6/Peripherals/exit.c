#include "exit.h"
#include "delay.h"
#include "key.h"

//外部中断初始化
void ExitInit(){
    //中断配置结构体定义
    NVIC_InitTypeDef NVIC_InitStruct;
    
    //中断线配置结构体定义
    EXTI_InitTypeDef EXTI4_InitStruct;
    
    //开启外部中断时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    
    //初始化按键
    KeyInit();
    
    //中断线与IO口映射
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
    
    //中断线初始化设置
    EXTI4_InitStruct.EXTI_Line = EXTI_Line4;
    EXTI4_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI4_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
    EXTI4_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI4_InitStruct);
    
    //中断配置
    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
     
}

void EXTI4_IRQHandler(){
    delay_ms(10);
    EXTI_ClearITPendingBit(EXTI_Line4);
}
