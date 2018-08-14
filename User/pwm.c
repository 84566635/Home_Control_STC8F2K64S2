#include "PWM.h"
#include "key.h"



u8 Dimmer_Cool = Dimmer_Min;  //冷光脉宽控制，初始化位调光最小值
u8 Dimmer_Warm = Dimmer_Min;  //暖光脉宽控制，初始化位调光最小值

/******************************************************************************
 函数: void PCA_PWM_Init(void)
 描述: 利用PCA进行PWM调制函数的初始化
 参数: 无
 返回: 无
******************************************************************************/
void PCA_PWM_Init(void)
{	
	AUXR1 &= 0xCF;	     //#xx00 xxxx    [CCP_S1:CCP_S0]=[0:0],切换PWM输出的IO引脚至CCP0--P1.1 CCP1--P1.0 CCP2--P3.7
	
	P1M1 &= ~(0x03);	          //P1^0,P1^1 设置为推挽输出
  P1M0 |=  (0x03);	
	
	CMOD = 0x8c;        //#1000 1100 空闲模式下，停止PCA计数器工作；
	                    //           选择PCA时钟源位SYSclk/6;
	                    //           禁止PCA计数器溢出中断请求标志位
	
	CCON = 0;           // 清零PCA计数器溢出中断请求标志位CF
	
	CL = 0;             //清零PCA计数器
	CH = 0;

	CCAPM0 = 0x42;	    //#0100 0010  禁止PCA模块0使用比较器功能
	                    //            CCP0引脚用作脉宽调制PWM输出
	                    //            禁止中断
	
	CCAPM1 = 0x42;      //#0100 0010   同上，不过设置的是PCA模块1
	
	CCAP0H = Dimmer_Cool;  //PCA模块0的PWM脉宽控制
	CCAP1H = Dimmer_Warm;	 //PCA模块1的PWM脉宽控制
	
	CR = 1;             //启动PCA计数器
}


/******************************************************************************
 函数: void PCA_PWM_Updata(void)
 描述: 更新PCA模块0和模块1的脉宽控制值，并使脉宽在最大值与最小值之间
 参数: 无
 返回: 无
******************************************************************************/
void PCA_PWM_Updata(void)
{
	if(Dimmer_Cool >= Dimmer_Max)
	{
		Dimmer_Cool = Dimmer_Max;	
	}

	if(Dimmer_Cool <= Dimmer_Min)
	{
		Dimmer_Cool = Dimmer_Min;		
	}
	
	if(Dimmer_Warm >= Dimmer_Max)
	{
		Dimmer_Warm = Dimmer_Max;	
	}

	if(Dimmer_Warm <= Dimmer_Min)
	{
		Dimmer_Warm = Dimmer_Min;		
	}	
		
 	CCAP0H = Dimmer_Cool;
	CCAP1H = Dimmer_Warm;	 
}

/**********************************THE END**********************************/ 
