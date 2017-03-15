#include "stm32f4xx.h"
#include "uart.h"

//串口中断函数
void USART1_IRQHandler(){
    u8 res;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)){		
		res=USART_ReceiveData(USART1);
		USART_SendData(USART1,res);
	}
}

int main(){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    Usart_Init();
    while(1);
}
