#include "PWM.h"
#include "key.h"



u8 Dimmer_Cool = Dimmer_Min;  //���������ƣ���ʼ��λ������Сֵ
u8 Dimmer_Warm = Dimmer_Min;  //ů��������ƣ���ʼ��λ������Сֵ

/******************************************************************************
 ����: void PCA_PWM_Init(void)
 ����: ����PCA����PWM���ƺ����ĳ�ʼ��
 ����: ��
 ����: ��
******************************************************************************/
void PCA_PWM_Init(void)
{	
	AUXR1 &= 0xCF;	     //#xx00 xxxx    [CCP_S1:CCP_S0]=[0:0],�л�PWM�����IO������CCP0--P1.1 CCP1--P1.0 CCP2--P3.7
	
	P1M1 &= ~(0x03);	          //P1^0,P1^1 ����Ϊ�������
  P1M0 |=  (0x03);	
	
	CMOD = 0x8c;        //#1000 1100 ����ģʽ�£�ֹͣPCA������������
	                    //           ѡ��PCAʱ��ԴλSYSclk/6;
	                    //           ��ֹPCA����������ж������־λ
	
	CCON = 0;           // ����PCA����������ж������־λCF
	
	CL = 0;             //����PCA������
	CH = 0;

	CCAPM0 = 0x42;	    //#0100 0010  ��ֹPCAģ��0ʹ�ñȽ�������
	                    //            CCP0���������������PWM���
	                    //            ��ֹ�ж�
	
	CCAPM1 = 0x42;      //#0100 0010   ͬ�ϣ��������õ���PCAģ��1
	
	CCAP0H = Dimmer_Cool;  //PCAģ��0��PWM�������
	CCAP1H = Dimmer_Warm;	 //PCAģ��1��PWM�������
	
	CR = 1;             //����PCA������
}


/******************************************************************************
 ����: void PCA_PWM_Updata(void)
 ����: ����PCAģ��0��ģ��1���������ֵ����ʹ���������ֵ����Сֵ֮��
 ����: ��
 ����: ��
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
