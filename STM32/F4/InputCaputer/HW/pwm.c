#include "pwm.h"


void TIM14_PWM_Init(u32 arr,u32 psc){
    //GPIOF结构体定义
    GPIO_InitTypeDef GPIOF_Struct;
    //定时器14结构体定义
    TIM_TimeBaseInitTypeDef TIM14_Struct;
    TIM_OCInitTypeDef TIM_OCStruct;
    
    //LED初始化
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//使能F口        
    GPIOF_Struct.GPIO_Mode = GPIO_Mode_AF;//端口输出模式，复用输出
    GPIOF_Struct.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIOF_Struct.GPIO_Pin = GPIO_Pin_9;//初始化端口
    GPIOF_Struct.GPIO_PuPd = GPIO_PuPd_UP;//输出上拉
    GPIOF_Struct.GPIO_Speed = GPIO_Speed_50MHz;//时钟频率
    GPIO_Init(GPIOF,&GPIOF_Struct);//初始化
    
    //端口映射
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14);//GPIOF9复用映射到定时器14
    
    //定时器初始化
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);//使能定时器14时钟
    
    TIM14_Struct.TIM_Prescaler = psc;//时钟预分频数
    TIM14_Struct.TIM_Period = arr;//自动重装值
    TIM14_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM14_Struct.TIM_CounterMode = TIM_CounterMode_Up;//计数器的模式，向上
    TIM_TimeBaseInit(TIM14,&TIM14_Struct);//定时器14初始化
    
    //初始化TIM14 Channel1 PWM模式
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//输出极性:TIM输出比较极性低,低电平有效
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OC1Init(TIM14,&TIM_OCStruct);//根据T指定的参数初始化外设TIM1 4OC1
    
    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
    
    TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE使能，使能自动重装载的预装载寄存器允许位
    
    //使能定时器14
    TIM_Cmd(TIM14,ENABLE);

}



