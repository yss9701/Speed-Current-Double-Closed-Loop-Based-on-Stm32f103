#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

void KEY_Init(void);//IO初始化
u8 KEY_Scan(void);   //按键扫描函数
u8 ReScan(GPIO_TypeDef* GPIOX,uint16_t colPin,u8 colIndex);//二次扫描
#endif
