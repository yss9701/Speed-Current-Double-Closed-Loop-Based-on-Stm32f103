/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：usart2.c
 * 描述    ：将printf函数重定向到USART2。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手。         
 * 实验平台：秉火STM32开发板
 * 硬件连接：---------------------
 *          | 									  |
 *          | PA2  - USART2(Tx)   |
 *          | PA3  - USART2(Rx)   |
 *          |                     |
 *           ---------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "bsp_usart2.h"
#include <stdarg.h>
uint8_t  Uart_Rx[UART_RX_LEN ]={0};

/// 配置USART2接收中断
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = macUSART_IRQ_2;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：USARTx_Config
 * 描述  ：USART2 GPIO 配置,工作模式配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USARTx_Config_2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
	/* config USART2 clock */
	macUSART_APBxClock_FUN_2(macUSART_CLK_2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel5);
	macUSART_GPIO_APBxClock_FUN_2(macUSART_GPIO_CLK_2, ENABLE);

	/* USART2 GPIO config */
	 /* Configure USART2 Tx (PA.02) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = macUSART_TX_PIN_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(macUSART_TX_PORT_2, &GPIO_InitStructure);
	    
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = macUSART_RX_PIN_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(macUSART_RX_PORT_2, &GPIO_InitStructure);
	 ///////////////////// 
		DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( macUSARTx_2->DR ) );	
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Uart_Rx;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
		/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
		DMA_InitStructure.DMA_BufferSize = UART_RX_LEN   ;

		/*外设地址不增*/	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 

		/*内存地址自增*/
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	

		/*外设数据单位*/	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

		/*内存数据单位 8bit*/
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 

		/*DMA模式：不断循环*/
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 

		/*优先级：中*/	
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  

		/*禁止内存到内存的传输	*/
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

		/*配置DMA1的4通道*/		   
		DMA_Init(DMA1_Channel5, &DMA_InitStructure); 	   
		
		/*使能DMA*/
		DMA_Cmd (DMA1_Channel5,ENABLE);					
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = macUSART_BAUD_RATE_2;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(macUSARTx_2, &USART_InitStructure); 
	
	/*	配置中断优先级 */
	NVIC_Configuration();
	/* 使能串口2接收中断 */
	USART_ITConfig(macUSARTx_2, USART_IT_RXNE, DISABLE);
	USART_ITConfig(macUSARTx_2, USART_IT_TC, DISABLE);
USART_ITConfig(macUSARTx_2, USART_IT_IDLE, ENABLE);
USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(macUSARTx_2, ENABLE);
	//USART_ClearFlag(macUSARTx_2, USART_FLAG_TC);
}

