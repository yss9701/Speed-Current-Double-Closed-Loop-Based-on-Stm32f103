#include "balance.h"


/*************************************************
                 速度部分参数
*************************************************/
#define XIANSHU (11*34)
#define CAR_SPEED_CONSTANT  (7500/XIANSHU)
float SpeedSet=0;
float Speed_control_p=4;
float SPEED_CONTROL_I=5;
float Integral_MAX=230;
float JIFEN_YUZHI=100;
float gf_SpeedControlOutMax=4.5;
const float MOTOR_OUT_MAX=450;
const float PWM_BIANHUALV=10;
/*************************************************
                 电流部分参数
*************************************************/
float Current_control_p=25;
float Current_CONTROL_I=1;
float JIFEN_YUZHI_I=150;
float Integral_MAX_I=300;
float gf_CurrentControlOutMax=450;
/*************************************************
                  速度部分变量
*************************************************/
float gf_nowspeed,
	    gf_GPT,
      gf_SpeedControlIntegral,
      gf_SpeedControlOut,
      fmoter_old;
/*************************************************
                  电流部分变量
*************************************************/
float gf_current;
float gf_CurrentControlIntegral;
float gf_CurrentControlOut;
////////////////采集脉冲//////////////////////////////////////
void GetMoterpluse(void)
{
	gf_GPT=(short)TIM4->CNT;
	TIM4->CNT = 0;
}
/////////////////转速pid/////////////////////////////////////
void speedpid(void)
{
	float fDelta=0, f_p=0,fI_0=0;
	int i_index=0;
	gf_nowspeed=gf_GPT*CAR_SPEED_CONSTANT;  //当前转速换算  r/min
	fDelta=SpeedSet-gf_nowspeed;         //偏差
	if(fDelta>JIFEN_YUZHI|fDelta<-JIFEN_YUZHI) //积分分离阈值
		i_index=0;
	else
		i_index=1;
	f_p=fDelta*(float)(TEXT_Buffer[0])/100;            //比例控制
	fI_0= fDelta*((float)(TEXT_Buffer[1])/1000)*i_index;  //积分控制
	gf_SpeedControlIntegral += fI_0;            //累加
	if(gf_SpeedControlIntegral>Integral_MAX)    //积分限幅
		gf_SpeedControlIntegral=Integral_MAX;
	else if(gf_SpeedControlIntegral<-Integral_MAX)
		gf_SpeedControlIntegral=-Integral_MAX;
	gf_SpeedControlOut=f_p+gf_SpeedControlIntegral;
	
	if(gf_SpeedControlOut>gf_SpeedControlOutMax)   //外环输出（转速）限幅
		gf_SpeedControlOut=gf_SpeedControlOutMax;
	else if(gf_SpeedControlOut<-gf_SpeedControlOutMax)
		gf_SpeedControlOut=-gf_SpeedControlOutMax;
}
//////////////////////电流pid///////////////////////////////
void currentpid(void)
{
	float fDelta=0, f_p=0,fI_0=0;
	int i_index=0;
	
	gf_current=gf_current*0.9+((Get_Adc_Average(ADC_Channel_1,10)-2106.05)*1.6)*0.1; //当前电流扩大100倍  A
	
	
	fDelta=gf_SpeedControlOut*100-gf_current;     //偏差
	if(fDelta>JIFEN_YUZHI_I|fDelta<-JIFEN_YUZHI_I)  //积分分离阈值
		i_index=0;
	else
		i_index=1;
	f_p=fDelta*(float)(TEXT_Buffer[2]);               //比例控制
	fI_0= fDelta*((float)(TEXT_Buffer[3])/100)*i_index; //积分控制
	gf_CurrentControlIntegral += fI_0;         //累加
	if(gf_CurrentControlIntegral>Integral_MAX_I)  //积分限幅
		gf_CurrentControlIntegral=Integral_MAX_I;
	else if(gf_CurrentControlIntegral<-Integral_MAX_I)
		gf_CurrentControlIntegral=-Integral_MAX_I;
	gf_CurrentControlOut=f_p+gf_CurrentControlIntegral;
	
	if(gf_CurrentControlOut>gf_CurrentControlOutMax)  //内环输出限幅
		gf_CurrentControlOut=gf_CurrentControlOutMax;
	else if(gf_CurrentControlOut<-gf_CurrentControlOutMax)
		gf_CurrentControlOut=-gf_CurrentControlOutMax;
}
/////////////////////////////////////////////////////
void Moteroutput(void)
{
	float fmoter;
	fmoter=gf_CurrentControlOut;  //给控制量
	
	
	
	if(fmoter-fmoter_old>PWM_BIANHUALV)  //限pwm变化率，防止反电动势太高拉低单片机供电电压，使单片机复位
	{
		fmoter=fmoter_old+PWM_BIANHUALV;
	}
	else if(fmoter-fmoter_old<-PWM_BIANHUALV)
	{
		fmoter=fmoter_old-PWM_BIANHUALV;
	}
		if(fmoter>MOTOR_OUT_MAX)    //pwm占空比限幅，防止程序跑飞以及烧驱动板
		fmoter=MOTOR_OUT_MAX;
	  else if(fmoter<-MOTOR_OUT_MAX)
		fmoter=-MOTOR_OUT_MAX;
		
		fmoter_old=fmoter;
	if(fmoter>=0)                 //正转
	{
		LED1_ON;
	  LED2_OFF; 
		vMotoXPwmOutleft((uint16_t)(fmoter+500));
	}
	else if(fmoter<0)            //反转
	{
		LED1_OFF;
	  LED2_ON; 
		vMotoXPwmOutleft((uint16_t)(500-fmoter));
	}
}


