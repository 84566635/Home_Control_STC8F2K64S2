#include "eeprom.h"
#include "intrins.h"

/******************************************************************************
 函数: void IapIdle(void)
 描述: 关闭IAP
 参数: 无
 返回: 无
******************************************************************************/
void IapIdle(void)
{
    IAP_CONTR = 0;                 
    IAP_CMD = 0;                    
    IAP_TRIG = 0;                  
    IAP_ADDRH = 0x80;               
    IAP_ADDRL = 0;
}


/******************************************************************************
 函数: void EraseSector(u16 addr)
 描述: 擦除目标区域
 参数: addr 需要擦除的地址
 返回: 无
******************************************************************************/
void EraseSector(u16 addr)
{
    IAP_CONTR = ENABLE_IAP;         
    IAP_CMD = CMD_ERASE;            
    IAP_ADDRL = addr;               
    IAP_ADDRH = addr >> 8;          
    IAP_TRIG = 0x5a;                
    IAP_TRIG = 0xa5;                
    _nop_();                       
    IapIdle();
}


/******************************************************************************
 函数: void WriteByte(u16 addr,u8 dat)
 描述: 往目标区域写入数据
 参数: addr 需要写入数据的地址
       dat  需要写入的数据
 返回: 无
******************************************************************************/
void WriteByte(u16 addr,u8 dat)
{
    IAP_CONTR = ENABLE_IAP;         
    IAP_CMD = CMD_PROGRAM;         
    IAP_ADDRL =  addr;              
    IAP_ADDRH = addr >> 8;         
    IAP_DATA = dat;               
    IAP_TRIG = 0x5a;                
    IAP_TRIG = 0xa5;               
    _nop_();                        
    IapIdle();
}



/******************************************************************************
 函数: u8 ReadByte(u16 addr)
 描述: 从目标区域读出数据
 参数: addr 需要读取数据的地址
 返回:  从目标区域读取的数据
******************************************************************************/
u8 ReadByte(u16 addr)
{
    u8 dat;                       

    IAP_CONTR = ENABLE_IAP;         
    IAP_CMD = CMD_READ;             
    IAP_ADDRL = addr;               
    IAP_ADDRH = addr >> 8;          
    IAP_TRIG = 0x5a;                
    IAP_TRIG = 0xa5;               
    _nop_();                       
    dat = IAP_DATA;                 
    IapIdle();                    

    return dat;                     
}


/*******************************END*********************************/
