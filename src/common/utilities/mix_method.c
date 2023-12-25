/***********************************************************
*  File: mix_method.c
*  Author: nzy
*  Date: 20160826
***********************************************************/
#define __MIX_METHOD_GLOBALS
#include "mix_method.h"
#include "tal_memory.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
#define __tolower(c) ((('A' <= (c))&&((c) <= 'Z')) ? ((c) - 'A' + 'a') : (c))

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
int uni_strcmp   (  const char *src,const char *dst)
{
    int ret = 0;

    while(!(ret  =  *(unsigned char *)src - *(unsigned char *)dst) && *dst) {
        ++src,++dst;
    }

    if(ret < 0) {
        ret = -1;
    } else if(ret > 0) {
        ret = 1;
    }

    return (ret);
}


unsigned char asc2hex(char asccode)
{
    unsigned char ret;

    if('0'<=asccode && asccode<='9')
        ret=asccode-'0';
    else if('a'<=asccode && asccode<='f')
        ret=asccode-'a'+10;
    else if('A'<=asccode && asccode<='F')
        ret=asccode-'A'+10;
    else
        ret=0;

    return ret;
}

void ascs2hex(unsigned char *hex,unsigned char *ascs,int srclen)
{
    unsigned char l4,h4;
    int i,lenstr;
    lenstr = srclen;

    if(lenstr%2) {
        lenstr -= (lenstr%2);
    }

    if(lenstr==0){
        return;
    }

    for(i=0; i < lenstr; i+=2) {
        h4=asc2hex(ascs[i]);
        l4=asc2hex(ascs[i+1]);
        hex[i/2]=(h4<<4)+l4;
    }
}

void hex2str(unsigned char *pbDest, unsigned char *pbSrc, int nLen)
{
    char    ddl,ddh;
    int i;

    for (i=0; i<nLen; i++) {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if (ddh > 57) ddh = ddh + 7;
        if (ddl > 57) ddl = ddl + 7;
        pbDest[i*2] = ddh;
        pbDest[i*2+1] = ddl;
    }

    pbDest[nLen*2] = '\0';
}

void byte2str(unsigned char *pbDest, unsigned char *pbSrc, int nLen, bool_t upper)
{
    char to_lower[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    char to_upper[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    char *tmp = upper?to_upper:to_lower;

    int i;
    for (i=0; i<nLen; i++) {
        pbDest[i*2] = tmp[((pbSrc[i]&0xF0)>>4)];
        pbDest[i*2+1] = tmp[(pbSrc[i]&0x0F)];
    }

    pbDest[nLen*2] = '\0';
    return;
}

// input: str->string
//        revr_index->reverse index,start from 0
//        ch->find char
// return: find position
int str_revr_find_ch(const char *str,const int revr_index,const char ch)
{
    if(NULL == str) {
        return -1;
    }

    int len = strlen(str);
    if(revr_index >= len) {
        return -1;
    }

    int i = 0;
    for(i = (len-1-revr_index);i >= 0;i--) {
        if(str[i] == ch) {
            return i;
        }
    }

    return -2;
}

void byte_sort(unsigned char is_ascend,unsigned char *buf,int buf_cnt)
{
    int i,j;
    unsigned char tmp = 0;
    for(j = 1;j < buf_cnt;j++) {
        for(i = 0;i < buf_cnt-j;i++) {
            if(is_ascend) {
                if(buf[i] > buf[i+1]) {
                    tmp = buf[i];
                    buf[i] = buf[i+1];
                    buf[i+1] = tmp;
                }
            }else {
                if(buf[i] < buf[i+1]) {
                    tmp = buf[i];
                    buf[i] = buf[i+1];
                    buf[i+1] = tmp;
                }
            }
        }
    }
}

char *mm_strdup(const char *str)
{
    if((void *)0 == str) {
        return (void *)0;
    }

    char *tmp = tal_malloc(strlen(str)+1);
    if((void *)0 == tmp) {
        return (void *)0;
    }
    memcpy(tmp, str, strlen(str));
    tmp[strlen(str)] = '\0';

    return tmp;
}

// ver is xx.xx.xx / xx.xx / xxxx
// Return 1: valid  0: invalid
int is_valid_version(char *ver)
{
    int section_num_count = 0;  //每一节数字个数
    int dot = 0;       //几个点分隔符
    char *last = NULL;
	
    if(ver == NULL){
        return 0;
    }
    if(strlen(ver) > 10 || strlen(ver) == 0) {  //服务端接受的版本最大长度10个字符
        return 0;
    }

    while(*ver) {
        if(*ver == '.') {
            if(last == NULL || (*last) == '.') {
                return 0;
            }

            dot++;
            if(dot > 2) {
                return 0;
            }

            section_num_count = 0;
        }else if(*ver >= '0' && *ver <= '9') {
            section_num_count++;
            if(section_num_count > 8) { //一种可能是客户以时间命名版本20190121，把单个section的长度加到最长8位
                return 0;
            }
        }else {
            return 0;
        }

        last = ver;
        ver++;
    }

    if(last != NULL && *last == '.') {
        return 0;
    }

    return 1;
}

int string2mac(const char *str, char mac[6])
{
    int len = strlen(str);
    int i = 0;
    int count = 0;

    if (12 != len && 17 != len) {
        return -1;
    }

    while (i < 6) {
        mac[i]=((asc2hex(str[count])<<4)&0xF0) + (asc2hex(str[count+1])&0x0F);
        count = count+2;
        if (len == 17) {
            count++;
        }
        i++;
    }

    return 0;
}

/*
 "1.0.1" to 0x010001
 "40.00" t0 0x2800
*/
int versionString2int(const char* versionString){
    int len = strlen(versionString);
    if(len <= 0){
        return OPRT_INVALID_PARM;
    }
    static unsigned char v = 0;
    int i = 0;
    int intOfString = 0;
    while(i < len){
        if(versionString[i] == '.'){
            i++;
            intOfString <<= 8;
            v = 0;
            continue;
        }else{
             unsigned char d = asc2hex(versionString[i]);             
             v = v*10 + d;
             if(versionString[i+1] == '.' || versionString[i+1] == '\0'){
                 intOfString |= v;
             }
        }        
        i++;
    }
    
    v = 0;
    return intOfString;
}


int tuya_isalpha(int c)
{
    if(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))) {
        return c;
    } else {
        return 0;
    }
}

int tuya_tolower(int c)
{
    return ((c >= 'A') && (c <= 'Z')) ? (c - 'A' + 'a') : c;
}

int tuya_toupper(int c)
{
    return ((c >= 'a') && (c <= 'z')) ? (c - 'a' + 'A') : c;
}

#include "mbedtls/base64.h"

#define TY_BASE64_BUF_LEN_CALC(slen)   (((slen) / 3 + ((slen) % 3 != 0)) * 4 + 1)   // 1 for '\0'

char * tuya_base64_encode(const unsigned char * bindata, char * base64, int binlength)
{
    size_t dlen, olen;
    dlen = TY_BASE64_BUF_LEN_CALC(binlength);
    mbedtls_base64_encode((unsigned char *)base64, dlen, &olen, bindata, binlength);
    return base64;
}

int tuya_base64_decode(const char * base64, unsigned char * bindata)
{

    size_t olen = 0;

    mbedtls_base64_decode(bindata, strlen(base64), &olen, (unsigned char *)base64, strlen(base64));

    return olen;
}

