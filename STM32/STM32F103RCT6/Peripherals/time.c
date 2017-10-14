#include "time.h"

/**
    系统初始化的时候在默认的系统初始化函数 SystemInit 函数里面已经初始化 APB1 的时钟为 2 分频，
    所以 APB1 的时钟为 36M，而从 STM32 的内部时钟树图得知：当 APB1 的时钟分频数为 1 的
    时候，TIM2~7 的时钟为 APB1 的时钟，而如果 APB1 的时钟分频数不为 1，那么 TIM2~7 的时
    钟频率将为 APB1 时钟的两倍。因此，TIM3 的时钟为 72M，再根据我们设计的 arr 和 psc 的值，
    就可以计算中断时间了。计算公式如下：
    Tout= ((arr+1)*(psc+1))/Tclk；
    其中：
    Tclk：TIM3 的输入时钟频率（单位为 Mhz）。
    Tout：TIM3 溢出时间（单位为 us）。
*/


//定时器3初始化设置
void Time3Init(u16 arr,u16 psc){
    //定时器3初始化结构体定义
    TIM_TimeBaseInitTypeDef TIM_Time3BaseInitStruct;
    //中断优先级结构体定义
    NVIC_InitTypeDef NVIC_InitStructure;

    //使能定时器3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
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
    NVIC_Init(&NVIC_InitStructure); //初始化 NVIC 寄存器
    
    //使能定时器3
    TIM_Cmd(TIM3,ENABLE);
}

void TIM3_IRQHandler(){
    static u8 i = 0;
    //检查 TIM3 更新中断发生与否
    if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除 TIM3 更新中断标志
        if(i == 1){
            i = 0;
            GPIO_SetBits(GPIOD,GPIO_Pin_2);
        }else{
            i = 1;
            GPIO_ResetBits(GPIOD,GPIO_Pin_2);
        }
    }
}

