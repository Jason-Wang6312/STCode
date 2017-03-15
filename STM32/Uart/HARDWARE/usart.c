#include "uart.h"

void Usart_Init(void){
    USART_InitTypeDef USART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    //使能时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    USART_Cmd(USART1,ENABLE);//串口使能
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//串口中断使能
    
    //串品初始化
    USART_InitStruct.USART_BaudRate     = 115200; //波特率
    USART_InitStruct.USART_Mode         = USART_Mode_Rx|USART_Mode_Tx; //模式
    USART_InitStruct.USART_Parity       = USART_Parity_No;//奇偶校验位
    USART_InitStruct.USART_StopBits     = USART_StopBits_1;//结束位
    USART_InitStruct.USART_WordLength   = USART_WordLength_8b;//数据位长
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1,&USART_InitStruct);

    //串口IO初始化
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF; //复用映射
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP; //推挽
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_9|GPIO_Pin_10;//uart IO
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉
    GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_50MHz; //速度 
    GPIO_Init(GPIOA,&GPIO_InitStruct);   
    
    //中断配置
    NVIC_InitStruct.NVIC_IRQChannel                     = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority          = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd                  = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
    
    //端口复用映射
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
}

