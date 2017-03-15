#include "time.h"

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void Time3Init(u16 arr,u16 psc){
    //定时器3初始化结构体
    TIM_TimeBaseInitTypeDef TIM3_Struct;
    //中断初始化结构体
    NVIC_InitTypeDef NVIC_Struct;
    
    //开启定时器3的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    TIM3_Struct.TIM_Prescaler = psc; //时钟预分频数
    TIM3_Struct.TIM_Period = arr; //自动重装值
    TIM3_Struct.TIM_CounterMode = TIM_CounterMode_Up; //计数器的模式，向上
    TIM3_Struct.TIM_ClockDivision = TIM_CKD_DIV1; //
    TIM_TimeBaseInit(TIM3,&TIM3_Struct); //定时器3初始化
    
    //开启定时器3的中断
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    
    //使能定时器3
    TIM_Cmd(TIM3,ENABLE);
    
    //开启中断初始化
    NVIC_Struct.NVIC_IRQChannel = TIM3_IRQn;//选择中断触发模块，定时器3中断
    NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
    NVIC_Struct.NVIC_IRQChannelSubPriority = 0x03;//子优先级
    NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;//使能中断
    NVIC_Init(&NVIC_Struct);//初始化中断
}

//定时器3中断函数
void TIM3_IRQHandler(void){
    //判断是否中断溢出
    if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET){
        LED1 = !LED1;
    }
    //清除中断标志位
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}   
