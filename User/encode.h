#ifndef __ENCODE_H__
#define __ENCODE_H__
#include "sys.h" 
#include "stdint.h"
void TIM4_Init(void);
uint16_t getpluse(void);
void TIM4_IRQHandler(void);
#endif
