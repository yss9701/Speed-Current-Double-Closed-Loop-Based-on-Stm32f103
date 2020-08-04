/**
  ******************************************************************************
  * @file    main.c
  * @author  LYC
  * @version V1.0
  * @date    2014-04-22
  * @brief   移植野火的软件I2C
  ******************************************************************************
  * @attention
  * 实验平台:野火 iSO STM32 开发板 
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "balance.h"
#include "STM32F1.h"
#include "bsp_usart1.h"
#include "mpu6050.h"
#include "bsp_i2c.h"
#include "bsp_pwm_output.h" 
void TIM2_NVIC_Config(void);
void TIM2_Config(void);
void TIM2_IRQHandler(void);
static int count=0;
int canshuOK_p=0;
int canshuOK_i=0;
int qidongOK=0;
int canshuOK_ip=0;
int canshuOK_iI=0;
uint8_t color=0;
extern float SpeedSet;
extern float Speed_control_p;
extern float SPEED_CONTROL_I;
extern float Current_control_p;
extern float Current_CONTROL_I;
//extern short Accel[3];
//extern	short Gyro[3];
extern float gf_nowspeed,
	           gf_GPT,
             gf_SpeedControlIntegral,
             gf_SpeedControlOut,
             gf_current;
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
/* LED亮度等级 PWM表 */
uint16_t indexWave[] = {998,998,997,996,995,993,990,986,981,973,961,944,921,886,837,766,663,516,305,0,0,305,516,663,
766,837,886,921,944,961,973,981,986,990,993,995,996,997,998,998};
float var[2];
int i;
int speed_count=0;
int tiaoshi=0;
int tiaoshi_ok=0;
int main(void)
{
	//uint8_t datatemp[ SIZE ];
	delay_init();//滴答延时初始化
	delay_ms(5);
	KEY_Init();  //矩阵键盘初始化
	GPIO_Configuration();//lcd端口初始化
	LCD1602_Init();      //lcd使能
	//I2C初始化
	//i2c_GPIO_Config();
  //MPU6050初始化
	//MPU6050_Init();
	
	STMFLASH_Read ( FLASH_SAVE_ADDR, ( uint16_t * ) TEXT_Buffer, SIZE );
	delay_ms(5);
	LCD1602_Show_Str(0,0,"Debug");
	while(tiaoshi_ok==0)
	{
		switch (KEY_Scan())
		{
			case 1:
				tiaoshi=0;
			  LCD1602_ClearScreen();
			  LCD1602_Show_Str(0,0,"Debug");
			break;
			case 2:
				tiaoshi=1;
			  LCD1602_ClearScreen();
			  LCD1602_Show_Str(0,0,"Release");
			break;
			case 3:
				tiaoshi_ok=1;
			break;
		}
	}
	LCD1602_ClearScreen();
	if(tiaoshi==1)
	{
		LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);
		LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
		while(KEY_Scan()==0);
		LCD1602_ClearScreen();
		LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
		LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		while(KEY_Scan()==0); 
		//USART_Cmd(macUSARTx, DISABLE);
		USARTx_Config_2();//初始化串口2，远程控制
	}
	else if(tiaoshi==0)
{
	USARTx_Config();//串口1通信初始化
	//USART_Cmd(macUSARTx_2, DISABLE);
	LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);//显示速度环比例系数
	//按键修改比例系数
	while(canshuOK_p==0)
	{
		
		i=KEY_Scan();
		switch(i)
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[0]+=1;
			LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[0]-=1;
			LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);
		  
		break;
		case 3:
			canshuOK_p=1;
		  
		break;
		
		
	}
		i=0;
	}
	canshuOK_p=0;
	LCD1602_ClearScreen();
	//按键修改转速环积分系数
	LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
	while(canshuOK_i==0)
	{
		
		i=KEY_Scan();
		switch(i)
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[1]+=1;
			LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[1]-=1;
			LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
		  
		break;
		case 3:
			canshuOK_i=1;
		  
		break;
		
		
	}
	i=0;
	}
	canshuOK_i=0;
	LCD1602_ClearScreen();
	//按键修改电流环比例系数
	LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
	while(canshuOK_ip==0)
	{
		
		i=KEY_Scan();
		switch(i)
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[2]+=1;
		LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[2]-=1;
		LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
		  
		break;
		case 3:
			canshuOK_ip=1;
		  
		break;
		
		
	}
	i=0;
	}
	canshuOK_ip=0;
	LCD1602_ClearScreen();
	//按键修改电流环积分系数
	LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		while(canshuOK_iI==0)
	{
		
		i=KEY_Scan();
		switch(i)
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[3]+=1;
		LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[3]-=1;
		LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		  
		break;
		case 3:
			canshuOK_iI=1;
		  
		break;
		
		
	}
	i=0;
	}
}
  canshuOK_iI=0;
	LCD1602_ClearScreen();
//按键控制启动
	LCD1602_Show_Str(0,0,"Start now?");
	while(qidongOK==0)
	{
		i=KEY_Scan();
		if(i==3)
			qidongOK=1;
		i=0;
	}		
	qidongOK=0;
	LCD1602_ClearScreen();

	
	LED_GPIO_Config();//电机正反转控制
	
  TIMx_PWM_Init();//pwm初始化TIM3
	Adc_Init();//ad采集初始化，电流
	TIM4_Init();//编码器初始化TIM4
	TIM2_NVIC_Config();//定时中断初始化，定时控制
	TIM2_Config();

while(1)
{
	if(tiaoshi==0)
	{
//串口显示转速电流
	var[0]=gf_nowspeed;
	var[1]=gf_current;
	vcan_sendware(var, sizeof(var));
	//delay_us(500);
	delay_ms(1);
	}
	//按键调整给定转速
	i=KEY_Scan();
	switch(i)
	{
		case 1:
			SpeedSet+=50;
		break;
		case 2:
			SpeedSet-=50;
		break;
		case 3:
			SpeedSet=0;
		break;
		case 4:
			SpeedSet=-SpeedSet;
		break;
		case 5:
			TIM_Cmd(TIM2,DISABLE);
		  LED1_OFF;
	    LED2_OFF; 
		  tiaoshi_ok=0;
		  STMFLASH_Write ( FLASH_SAVE_ADDR, ( uint16_t * ) TEXT_Buffer, SIZE );
		  LCD1602_ClearScreen();
		  LCD1602_Show_Str(0,0,"Parameter saved");
		break;
		case 6:
		{
			TIM_Cmd(TIM2,DISABLE);
		  LED1_OFF;
	    LED2_OFF; 
		  LCD1602_ClearScreen();
		 	LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);//显示速度环比例系数
	//按键修改比例系数
	while(canshuOK_p==0)
	{
		
		
		switch(KEY_Scan())
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[0]+=1;
			LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[0]-=1;
			LCDshowNum(0,0,"P_S:",(int)TEXT_Buffer[0]);
		  
		break;
		case 3:
			canshuOK_p=1;
		  
		break;
		
		
	}
		
	}
	canshuOK_p=0;
	LCD1602_ClearScreen();
	//按键修改转速环积分系数
	LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
	while(canshuOK_i==0)
	{
		
		
		switch(KEY_Scan())
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[1]+=1;
			LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[1]-=1;
			LCDshowNum(0,1,"I_S:",(int)TEXT_Buffer[1]);
		  
		break;
		case 3:
			canshuOK_i=1;
		  
		break;
		
		
	}

	}
	canshuOK_i=0;
	LCD1602_ClearScreen();
	//按键修改电流环比例系数
	LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
	while(canshuOK_ip==0)
	{
		
		
		switch(KEY_Scan())
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[2]+=1;
		LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[2]-=1;
		LCDshowNum(0,0,"P_I:",(int)TEXT_Buffer[2]);
		  
		break;
		case 3:
			canshuOK_ip=1;
		  
		break;
		
		
	}

	}
	canshuOK_ip=0;
	LCD1602_ClearScreen();
	//按键修改电流环积分系数
	LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		while(canshuOK_iI==0)
	{
		
		
		switch(KEY_Scan())
	{
		case 1:
			LCD1602_ClearScreen();
			TEXT_Buffer[3]+=1;
		LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		  
		break;
		case 2:
			LCD1602_ClearScreen();
			TEXT_Buffer[3]-=1;
		LCDshowNum(0,1,"I_I:",(int)TEXT_Buffer[3]);
		  
		break;
		case 3:
			canshuOK_iI=1;
		  
		break;
		
		
	}

	}
  canshuOK_iI=0;
	LCD1602_ClearScreen();
	LCD1602_Show_Str(0,0,"Start now?");
	while(qidongOK==0)
	{
		
		if(KEY_Scan()==3)
			qidongOK=1;
	}		
	qidongOK=0;
	LCD1602_ClearScreen();
	TIM_Cmd(TIM2,ENABLE);
  }
		break;
	}
	i=0;
	if(tiaoshi_ok==1)
	{
	//LCD显示设定与当前速度
	LCDshowNum(0,0,"SpeedSet:",SpeedSet);
	LCDshowNum(0,1,"Now speed:",gf_nowspeed);
		
	}
}

}
void TIM2_NVIC_Config(void)//定时中断优先级配置
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void TIM2_Config(void)//定时中断时间配置
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler=71;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=2000;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}
void TIM2_IRQHandler(void)
{
	static uint8_t pwm_index = 0;			//用于PWM查表
	static uint8_t period_cnt = 0;		//用于计算周期数
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		 count++;
		 period_cnt++;
		if(period_cnt >= 50)										//若输出的周期数大于10，输出下一种脉冲宽的PWM波
			{
				switch (color)
				{
				case 0:
				vMotoXPwmOutright(indexWave[pwm_index]);
				vMotoXPwmOutleftright(indexWave[pwm_index]);
				break;
				case 1:
					vMotoXPwmOutright(indexWave[pwm_index]);
					vMotoXPwmOutleftright(998);
				break;
				case 2:
					vMotoXPwmOutright(998);
					vMotoXPwmOutleftright(indexWave[pwm_index]);
				break;
				}
				pwm_index++;												//标志PWM表的下一个元素
			
				if( pwm_index >=  40)								//若PWM脉冲表已经输出完成一遍，重置PWM查表标志
				{
					pwm_index=0;	
          color++;
          if(color>=3)
						color=0;
				}
				
				period_cnt=0;												//重置周期计数标志
			}
  switch (count)
  {
  case 1:
    
	  speed_count++;
    GetMoterpluse();//4ms采集一次脉冲
	if(speed_count==10)//40ms进行一次速度pid
	{
	  speedpid();
		speed_count=0;
	}
    break;
 
    
    case 2:
		currentpid();//4ms采集一次电流值并进行电流pid
		Moteroutput();//控制电机
    count=0;
    break;
  }
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		
	}
}


/*********************************************END OF FILE**********************/
