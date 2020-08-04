#ifndef __PWM_OUTPUT_H
#define	__PWM_OUTPUT_H


#include "stm32f10x.h"


/********************TIM参数定义，只限TIM2、3、4或5**************************/
#define             macTIMx                                TIM3
#define             macTIM_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macTIM_CLK                             RCC_APB1Periph_TIM3
#define             macTIM_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macTIM_GPIO_CLK                        (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define             macTIM_CH1_PORT                        GPIOA
#define             macTIM_CH1_PIN                         GPIO_Pin_6
#define             macTIM_CH2_PORT                        GPIOB
#define             macTIM_CH2_PIN                         GPIO_Pin_5
#define             macTIM_CH3_PORT                        GPIOB
#define             macTIM_CH3_PIN                         GPIO_Pin_0
#define             macTIM_CH4_PORT                        GPIOB
#define             macTIM_CH4_PIN                         GPIO_Pin_1


/************************** TIM 函数声明********************************/
void TIMx_PWM_Init(void);
void vMotoXPwmOutleft(u16 lChannel1);
void vMotoXPwmOutright(u16 lChannel2);
void vMotoXPwmOutleftright(u16 lChannel3);
static void TIMx_Mode_Config(void);
static void TIMx_GPIO_Config(void) ;
void TIM2_NVIC_Config(void);
void TIM2_Config(void);
#endif /* __PWM_OUTPUT_H */


