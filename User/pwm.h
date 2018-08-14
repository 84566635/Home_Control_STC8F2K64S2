#ifndef __pwm_H
#define  __pwm_H

#include "STC15W408AS.H"
#include "MyConfig.h"

//#define  Dimmer_Max 0x76
//#define  Dimmer_Min 0x12

#define  Dimmer_Max 0x70
#define  Dimmer_Min 0x06

#define	PCA0			0
#define	PCA1			1
#define	PCA2			2
#define	PCA_Counter		3
#define	PCA_P12_P11_P10_P37	(0<<4)
#define	PCA_P34_P35_P36_P37	(1<<4)
#define	PCA_P24_P25_P26_P27	(2<<4)
#define	PCA_Mode_PWM				0x42
#define	PCA_Mode_Capture			0
#define	PCA_Mode_SoftTimer			0x48
#define	PCA_Mode_HighPulseOutput	0x4c
#define	PCA_Clock_1T	(4<<1)
#define	PCA_Clock_2T	(1<<1)
#define	PCA_Clock_4T	(5<<1)
#define	PCA_Clock_6T	(6<<1)
#define	PCA_Clock_8T	(7<<1)
#define	PCA_Clock_12T	(0<<1)
#define	PCA_Clock_Timer0_OF	(2<<1)
#define	PCA_Clock_ECI	(3<<1)
#define	PCA_Rise_Active	(1<<5)
#define	PCA_Fall_Active	(1<<4)
#define	PCA_PWM_8bit	(0<<6)
#define	PCA_PWM_7bit	(1<<6)
#define	PCA_PWM_6bit	(2<<6)


extern u8 Dimmer_Cool;
extern u8 Dimmer_Warm;

void  PCA_PWM_Updata(void);
void  PCA_PWM_Init(void);

#endif


