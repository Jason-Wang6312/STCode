#include "incap.h"

//定时器 5 通道 1 输入捕获配置
//arr：自动重装值(TIM2,TIM5 是 32 位的!!) psc：时钟预分频数
void TIM5InCapInit(u32 arr,u16 psc){
    //IO初始化参数结构体
    GPIO_InitTypeDef GPIOA_Struct;
    //定时器参数结构体
    TIM_TimeBaseInitTypeDef TIM5_Struct;
    //输入捕获参数结构体
    TIM_ICInitTypeDef TIM5_InCap_Struct;
    //中断参数结构体
    NVIC_InitTypeDef NVIC_Struct;
    
    //初始化IO口
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//开启IO的时钟
    
    GPIOA_Struct.GPIO_Mode = GPIO_Mode_AF;//端口复用
    GPIOA_Struct.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIOA_Struct.GPIO_Pin = GPIO_Pin_0;//IO口
    GPIOA_Struct.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
    GPIOA_Struct.GPIO_Speed = GPIO_Speed_50MHz;//时钟上游
    GPIO_Init(GPIOA,&GPIOA_Struct);//初始化写入

    //定时器5初始化
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    
    TIM5_Struct.TIM_Prescaler = psc; //定时器分频
    TIM5_Struct.TIM_Period = arr;//自动重装载值
    TIM5_Struct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
    TIM5_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM5,&TIM5_Struct);
    
    //输入出补获初始化
    TIM5_InCap_Struct.TIM_Channel = TIM_Channel_1;//选择输入端 IC1 映射到 TI1 上
    TIM5_InCap_Struct.TIM_ICFilter = 0x00;//配置输入滤波器 不滤波
    TIM5_InCap_Struct.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
    TIM5_InCap_Struct.TIM_ICSelection = TIM_ICSelection_DirectTI;//映射到 TI1 上
    TIM5_InCap_Struct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//配置输入分频,不分频
    TIM_ICInit(TIM5,&TIM5_InCap_Struct);//初始化 TIM5 输入捕获参数
    
    //允许更新和捕获中断
    TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);
    
    //使能定时器 5
    TIM_Cmd(TIM5,ENABLE ); 
    
    //中断初始化配置
    NVIC_Struct.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_Struct.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级 2
    NVIC_Struct.NVIC_IRQChannelSubPriority =0;//响应优先级 0
    NVIC_Struct.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
    NVIC_Init(&NVIC_Struct);  //根据指定的参数初始化 VIC 寄存器
}


//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于 32 位定时器来说,1us 计数器加 1,溢出时间:4294 秒)
//u8 TIM5CH1_CAPTURE_STA=0;  //输入捕获状态 
//u32 TIM5CH1_CAPTURE_VAL; //输入捕获值(TIM2/TIM5 是 32 位)
//定时器 5 中断服务程序
void TIM5_IRQHandler(){
    if(TIM5InCapStatus & 0x80){ //成功捕获
        if(TIM5InCapStatus & 0x40){//
            
        }
    }
}

