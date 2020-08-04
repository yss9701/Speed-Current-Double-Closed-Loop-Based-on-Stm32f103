#include "KeyScan.h"


void Delay_ms1(int xms)	
{
  u32 i,j;
	for(i=xms;i>0;i--)
	  for(j=72000;j>0;j--);
}
void Scan_GPIO_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_3);							//设置行为高电平
	GPIO_ResetBits(GPIOE, GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_7);						//设置列为低电平
}




 
u8 Read_KeyValue(void)
{
	u8 KeyValue=0;
	if((GPIO_ReadInputData(GPIOE)&0xFF)!=0x0F)								//取GPIOC低六位电平值，判断按键是否按下
	{
		Delay_ms1(8);																					//延时消抖，该延时函数是软件延时，非准确延时
		if((GPIO_ReadInputData(GPIOE)&0xFF)!=0x0F)						//再次判断按键是否按下
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_0);										//设置第一行为 高电平，其它行为低电平，通过扫描，可以判断那一列出现高电平
			GPIO_ResetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_3);				
			switch(GPIO_ReadInputData(GPIOE)&0xFF)							//取GPIOC低六位电平值，判断那一按键按下
			{
				case 0x11: KeyValue = 1; break;				// 0001 0001 
				case 0x21: KeyValue = 5; break;				// 0010 0001
				case 0x41: KeyValue = 9; break;				// 0100 0001
				case 0x81: KeyValue = 13; break;				// 1000 0001
			}
			GPIO_SetBits(GPIOE, GPIO_Pin_1);
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_2|GPIO_Pin_3);
			
			switch(GPIO_ReadInputData(GPIOE)&0xFF)
			{
				case 0x12: KeyValue = 2; break;   //0001 0010
				case 0x22: KeyValue = 6; break;   //0010 0010
				case 0x42: KeyValue = 10;break;    //0100 0010
				case 0x82: KeyValue = 14;break;    //1000 0010
			}
			GPIO_SetBits(GPIOE, GPIO_Pin_2);
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_3);
			
			switch(GPIO_ReadInputData(GPIOE)&0xFF)
			{
				case 0x14: KeyValue = 3; break;   //0001 0100
				case 0x24: KeyValue = 7; break;   //0010 0100
				case 0x44: KeyValue = 11; break;   //0100 0100
				case 0x84: KeyValue = 15; break;   //1000 0100
			}
			GPIO_SetBits(GPIOE, GPIO_Pin_3);
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2);
			
			switch(GPIO_ReadInputData(GPIOE)&0xFF)
			{
				case 0x18: KeyValue = 4; break;   //0001 1000
				case 0x28: KeyValue = 8; break;   //0010 1000
				case 0x48: KeyValue = 12; break;   //0100 1000
				case 0x88: KeyValue = 16; break;   //1000 1000
			}
			GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_3);
			GPIO_ResetBits(GPIOE, GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_7);
			
			while((GPIO_ReadInputData(GPIOE)&0xFF)!=0x0F);				//等待按键松开
			return KeyValue;
			//printf("按下的键值:%d\n",KeyValue);
			}
	}
	return 0;
}

