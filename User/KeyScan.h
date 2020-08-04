#ifndef __KEYSCAN_H
#define __KEYSCAN_H


#include "stm32f10x.h"


void Scan_GPIO_Config(void);
u8 Read_KeyValue(void);
void Delay_ms1(int xms);

#endif


