#ifndef __Balance_H__
#define __Balance_H__
#include "stdint.h"
#include "bsp_usart1.h"
#include "bsp_usart1.h"
#include "mpu6050.h"
#include "bsp_i2c.h"
#include "bsp_pwm_output.h" 
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "STM32F1.h"
void speedpid(void);
void GetMoterpluse(void);
void currentpid(void);
void Moteroutput(void);

#endif
