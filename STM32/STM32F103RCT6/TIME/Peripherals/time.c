#include "time.h"

void Time3Init(u16 arr,u16 psc){
    //定时器3初始化结构体定义
    TIM_TimeBaseInitTypeDef TIM_Time3BaseInitStruct;
    //中断优先级结构体定义
    NVIC_InitTypeDef NVIC_InitStructure;

    //使能定时器3时钟
    RCC_AHBPeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    //定时器3初始化
    TIM_Time3BaseInitStruct.TIM_Period = arr; //自动重载值
    TIM_Time3BaseInitStruct.TIM_Prescaler = psc; //分频系数
    TIM_Time3BaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //计时方式，向上计数
    TIM_Time3BaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseInit(TIM3,&TIM_Time3BaseInitStruct);
    
    //开始定时器中断更新
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    
    //中断优先级 NVIC 设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 0 级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级 3 级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
    NVIC_Init(&NVIC_InitStructure); //④初始化 NVIC 寄存器
    
    //使能定时器3
    TIM_Cmd(TIM3,ENABLE);
}

void TIM3_IRQHandler(){
    //检查 TIM3 更新中断发生与否
    if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //清除 TIM3 更新中断标志
        
    }
}

