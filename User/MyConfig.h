#ifndef _MYCONFIG_H
#define _MYCONFIG_H

#include "STC8F2K16S2.H"


/*前置模块类型定义*/
#define Device_Mode 0x02

/*IO输入扫描类型定义*/
#define Input_IO 1
#define Enable_IO 2
#define Addr_IO 3

/*设备状态*/
#define  Online    1
#define  Offline   0

/*继电器开关状态*/
#define  OFF    0
#define  ON     1
#define  Keep   2
#define  Toggle 3


/*输入IO引脚定义*/
sbit IO1 = P5^4;//IO1 = P1^4;
sbit IO2 = P1^6;//IO2 = P1^2;
sbit IO3 = P1^3;//IO3 = P0^3;
sbit IO4 = P1^1;//IO4 = P0^1;
sbit IO5 = P3^2;//IO5 = P5^4;
sbit IO6 = P3^4;//IO6 = P2^3;
sbit IO7 = P2^4;//IO7 = P2^1;
sbit IO8 = P0^3;//IO8 = P2^5;

/*IO使能拨码开关引脚定义*/
sbit IO1_En = P0^2;//IO1_En = P2^4;
sbit IO2_En = P0^0;//IO2_En = P3^1;
sbit IO3_En = P2^7;//IO3_En = P3^0;
sbit IO4_En = P0^1;//IO4_En = P3^5;
sbit IO5_En = P2^6;//IO5_En = P3^4;
sbit IO6_En = P2^5;//IO6_En = P2^0;
sbit IO7_En = P3^5;//IO7_En = P2^2;
sbit IO8_En = P3^3;//IO8_En = P5^5;

/*RS485地址拨码开关引脚定义*/
sbit Addr1 = P5^5;//Addr1 = P1^5;
sbit Addr2 = P1^7;//Addr2 = P1^3;
sbit Addr3 = P1^5;//Addr3 = P1^1;
sbit Addr4 = P1^4;//Addr4 = P1^0;
sbit Addr5 = P1^2;//Addr5 = P0^2;
sbit Addr6 = P1^0;//Addr6 = P0^0;



/*继电器引脚定义*/
sbit Relay1 = P2^1;//Relay1 = P3^3;
sbit Relay2 = P2^0;//Relay2 = P3^2;



typedef 	unsigned char	uint8_t;
typedef 	unsigned int	uint16_t;
typedef 	unsigned long	uint32_t;




#endif