#include "pwm.h"
#include "pwm.h"
#include "delay.h"

/**
1.使能定时器和相关IO口时钟。
  使能定时器时钟：RCC_APB1PeriphClockCmd();
  使能GPIO时钟：RCC_AHB1PeriphClockCmd ();
2.初始化IO口为复用功能输出。函数：GPIO_Init();
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
3.GPIOF9复用映射到定时器14
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); 	
4.初始化定时器：ARR,PSC等：TIM_TimeBaseInit();
5.初始化输出比较参数:TIM_OC1Init();
6.使能预装载寄存器： TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable); 
7.使能自动重装载的预装载寄存器允许位TIM_ARRPreloadConfig(TIM14,ENABLE);
8.使能定时器。
9.不断改变比较值CCRx，达到不同的占空比效果:TIM_SetCompare1();
*/

//CH1 PWM输出初始化设置
void Tim1PwmInit(u16 arr,u16 psc){
    //声明定时器1 PWM 输出结构体变量
    TIM_OCInitTypeDef TIM1_OCInitStruct;
    
    //声明定时器1结构体变量
    TIM_TimeBaseInitTypeDef TIM1_TimeBaseInitStruct;
    
    //声明GPIO结构体变量
    GPIO_InitTypeDef GPIOA_InitStruct;
    
    //使能定时器1
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    
    //使能IO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1,ENABLE);
    
    //初始化IO配置
    GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIOA_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   
    GPIO_Init(GPIOA,&GPIOA_InitStruct);
    
    //定时器配置   
    TIM1_TimeBaseInitStruct.TIM_Period = arr;//设置用来作为 TIMx 时钟频率除数的预分频值 不分频
    TIM1_TimeBaseInitStruct.TIM_Prescaler = psc;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值  80K
    TIM1_TimeBaseInitStruct.TIM_ClockDivision = 0;//时钟分割:TDTS = Tck_tim
    TIM1_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
    TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseInitStruct);
    
    //设置定时器外设PWM输出配置
    TIM1_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; //脉宽调制模式 2
    TIM1_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM1_OCInitStruct.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
    TIM1_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性高
    TIM_OC1Init(TIM1,&TIM1_OCInitStruct);
    
    //CH1 预装载使能
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
    
    //使能 TIMx 在 ARR 上的预装载寄存器
    TIM_ARRPreloadConfig(TIM1,ENABLE);
    
    //使能定时器
    TIM_Cmd(TIM1,ENABLE);
    
    //定时器输出PWM使能
    TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

int main(){
    u16 led0pwmval=0;
    u8 dir=1;
    delay_init();  //延时函数初始化 
    Tim1PwmInit(899,0);//不分频。PWM 频率=72000/(899+1)=80Khz
    while(1){
        delay_ms(10);
        if(dir)led0pwmval++;
        else led0pwmval--;
        if(led0pwmval>300)dir=0;
        if(led0pwmval==0)dir=1; 
        TIM_SetCompare1(TIM1,led0pwmval); 
    }
}
