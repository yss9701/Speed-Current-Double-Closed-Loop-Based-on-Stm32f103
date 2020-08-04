#ifndef __USART2_H
#define	__USART2_H


#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_it.h"

#define             macUSART_BAUD_RATE_2                       115200

#define             macUSARTx_2                                USART1
#define             macUSART_APBxClock_FUN_2                   RCC_APB2PeriphClockCmd
#define             macUSART_CLK_2                            RCC_APB2Periph_USART1
#define             macUSART_GPIO_APBxClock_FUN_2              RCC_APB2PeriphClockCmd
#define             macUSART_GPIO_CLK_2                        (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO)     
#define             macUSART_TX_PORT_2                         GPIOA   
#define             macUSART_TX_PIN_2                          GPIO_Pin_9
#define             macUSART_RX_PORT_2                         GPIOA 
#define             macUSART_RX_PIN_2                          GPIO_Pin_10
#define             macUSART_IRQ_2                             USART1_IRQn
#define             macUSART_INT_FUN_2                         USART1_IRQHandler
#define UART_RX_LEN   128
extern uint8_t Uart_Rx[UART_RX_LEN ];

void USARTx_Config_2(void);



#endif /* __USART2_H */
