#include "key.h"
#include "delay.h"

typedef struct 
{
	GPIO_TypeDef* GPIOX;
	uint16_t GPIO_Pin;
}KGPT[2][4]; //4*4
KGPT KPIN={ //只需要在下面填写横竖行的IO口和管脚   IO口和管脚都可以随意定义 无需在一个IO口 管脚也无需按顺序
						{{GPIOE,GPIO_Pin_0} ,{GPIOE,GPIO_Pin_1}, {GPIOE,GPIO_Pin_2},{GPIOE,GPIO_Pin_3}},//row 横行
						{{GPIOE,GPIO_Pin_4},{GPIOE,GPIO_Pin_5}, {GPIOE,GPIO_Pin_6},{GPIOE,GPIO_Pin_7}}  //col 竖行
					};
#define row 0
#define col 1					
#define RCC_APB2_GPIOX  RCC_APB2Periph_GPIOE//|RCC_APB2Periph_GPIOC//IO时钟					
#define MAX_Time   36000000 // 松手检测最大延时   
//按键初始化函数 
void KEY_Init(void)
{
  u8 i;
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2_GPIOX,ENABLE);//使能PORTC时钟
  for(i=0;i<4;i++)
	
	{
	 GPIO_InitStructure.GPIO_Pin = KPIN[row][i].GPIO_Pin;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置成推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
	 GPIO_Init(KPIN[row][i].GPIOX, &GPIO_InitStructure);//初始化row
	 GPIO_ResetBits(KPIN[row][i].GPIOX,KPIN[row][i].GPIO_Pin);
	}
 for(i=0;i<4;i++)
	{
		 GPIO_InitStructure.GPIO_Pin  = KPIN[col][i].GPIO_Pin;//PC5
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
		 GPIO_Init(KPIN[col][i].GPIOX, &GPIO_InitStructure);//初始化col
		GPIO_SetBits(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin);
	}
} 

u8 ReScan(GPIO_TypeDef* GPIOX,uint16_t colPin,u8 colIndex)
{
 u8 a;
 delay_ms(1);//消抖延时
 for(a = 0;a<4;a++){
  GPIO_SetBits(KPIN[row][a].GPIOX,KPIN[row][a].GPIO_Pin);//每个行置1
  if((GPIO_ReadInputDataBit(GPIOX, colPin) == 1)) //如果列也变了 行的值就知道了 为 i
		{
			GPIO_ResetBits(KPIN[row][a].GPIOX,KPIN[row][a].GPIO_Pin); //行恢复 置0 
		  return colIndex+a*4+1;//返回的数据 为1-16 对应4x4键盘的16个键
		}
	GPIO_ResetBits(KPIN[row][a].GPIOX,KPIN[row][a].GPIO_Pin);     //行恢复 置0
 }
 return 0;
}
u8 KEY_Scan(void)
{	 
 u8 i,keyvalue,j;
	//u32 j=0;
	for(i = 0;i<4;i++)
	{
	 if(GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0)//检测列 列值为 i
	 {
			keyvalue = ReScan(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin,i);//检测行 取键值
		 for(j=0;j<4;j++)
		 {
			 GPIO_SetBits(KPIN[col][j].GPIOX,KPIN[col][j].GPIO_Pin);
		 }
		 for(j=0;j<4;j++)
		 {
			 GPIO_ResetBits(KPIN[row][j].GPIOX,KPIN[row][j].GPIO_Pin);
		 }
		  while(GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0);
		//	while((GPIO_ReadInputDataBit(KPIN[col][i].GPIOX,KPIN[col][i].GPIO_Pin) == 0)&&(j<MAX_Time))j++;//松手检测
			return keyvalue;//返回键值
		}
	}
return 0;
}


					


