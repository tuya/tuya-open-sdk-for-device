/***********************************************************
*  File: crc_8.c 
*  Author: nzy
*  Date: 20170222
***********************************************************/
#define __CRC_16_GLOBALS
#include "crc_16.h"

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

unsigned short get_crc_16(unsigned char *data,unsigned short size)
{
    unsigned short poly[2] = {0, 0xa001}; //0x8005 <==> 0xa001
    unsigned short crc = 0xffff;
    int i, j;
    
    for(j = size; j > 0; j--) {
        unsigned char ds = *data++;
        
        for(i = 0; i < 8; i++) {
            crc = (crc >> 1) ^ poly[(crc ^ ds) & 1];
            ds = ds >> 1;
        }
    }
    
    return crc;
}

