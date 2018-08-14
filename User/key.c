#include "key.h"
#include "pwm.h"
#include "led.h"
#include "rs485.h"
#include "delay.h"



u8 PWM_adjust_drect0=0;  //PWM0�����ķ���0:����1����С
u8 PWM_adjust_drect1=0;  //PWM1�����ķ���0:����1����С
u8 Key_busy = 0;         //0�������¼����������1���а����¼����ڴ���
//========================================================================
// ����: void Key_scan(void)
// ����: ����ɨ�躯�������⵽�а��������£��������Ӧ�ĵ��⴦��
// ����: ��
// ����: ��
// ��ע: pwm�ĵ���ֵΪ299-499���͵�ƽ��Ч(�͵�ƽ�µƲŻ���)�����pwmֵԽ��
//      ��Խ������֮��pwmֵԽС����Խ��(pwmֵ��Ҳ��ͨ����˵��pwm��ռ�ձ�);  
//      ���������10���ȼ�����ÿ���ȼ�pwmֵ���ӻ����20(20=(499-299)/10)
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
			 if(!PWM_adjust_drect1)//����PWMֵ
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

