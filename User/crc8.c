#include "crc8.h"


/**********************************************************************************
  * @brief  对数据进行CRC值运算
  * @param  len--数据长度；
	          buf--数据内容
  * @retval CRC值
 *********************************************************************************/
uint8_t CRC8_Check(uint8_t len,const uint8_t *buf)
{
    uint8_t num_i=0,num_j=0,crc =0,middle_byte =0;
    for(num_i = 0 ; num_i < len; num_i++)
    {
        middle_byte = buf[num_i]; 
        for( num_j = 0 ; num_j < 8; num_j++){
            if(((crc^middle_byte)&0x01) == 0 ){
                crc >>=1;
            }
            else{
                crc^= 0x18;
                crc >>=1;
                crc |=0x80;
            }
            middle_byte >>=1;
        }
    }
    return crc ;
}





/*******************************END*********************************/
