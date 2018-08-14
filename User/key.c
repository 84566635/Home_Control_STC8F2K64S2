#include "key.h"
#include "pwm.h"
#include "led.h"
#include "rs485.h"
#include "delay.h"



u8 PWM_adjust_drect0=0;  //PWM0调整的方向：0:增大；1：减小
u8 PWM_adjust_drect1=0;  //PWM1调整的方向：0:增大；1：减小
u8 Key_busy = 0;         //0，按键事件处理结束；1，有按键事件正在处理
//========================================================================
// 函数: void Key_scan(void)
// 描述: 按键扫描函数，如检测到有按键被按下，则进行相应的调光处理
// 参数: 无
// 返回: 无
// 备注: pwm的调光值为299-499，低电平有效(低电平下灯才会亮)，因而pwm值越大
//      灯越暗；反之，pwm值越小，灯越亮(pwm值，也即通常所说的pwm的占空比);  
//      按键调光分10个等级，即每个等级pwm值增加或减少20(20=(499-299)/10)
//========================================================================
void Key_scan(void)
{
  if(Key1==0 || Key2==0)
	{
     Delay_10ms(10);  
		 if(Key1==0 && Key_busy == 0)
		 {
			 Key_busy =1;      
			 if(!PWM_adjust_drect0)            
			 {
         Dimmer_Cool += 0x01;                      //(PWM_HIGH_MAX-pwm0)*100/PWM_BASE;
       }
			 else
			 {
         Dimmer_Cool -= 0x01;
       }
	     
			 if(Dimmer_Cool >= Dimmer_Max)
			 {
         Dimmer_Cool = Dimmer_Max;
				 PWM_adjust_drect0 = 1;
       }
			 if(Dimmer_Cool <= Dimmer_Min)
			 {
         Dimmer_Cool = Dimmer_Min;
				 PWM_adjust_drect0 = 0;
        } 
		    PCA_PWM_Updata();				
		 }
		 
     if(Key2==0 && Key_busy == 0)
		  {
			 Key_busy =1;      
			 if(!PWM_adjust_drect1)//增大PWM值
			 {
         Dimmer_Warm += 0x01;
       }
			 else
			 {
         Dimmer_Warm -= 0x01;
       }
	     
			 if(Dimmer_Warm >= Dimmer_Max)
			 {
         Dimmer_Warm = Dimmer_Max;
				 PWM_adjust_drect1 = 1;
       }
			 if(Dimmer_Warm <= Dimmer_Min)
			 {
         Dimmer_Warm = Dimmer_Min;
				 PWM_adjust_drect1 = 0;
        } 
        PCA_PWM_Updata();				
		  }
    Key_busy = 0;			
  }

}

/**********************************THE END**********************************/ 

