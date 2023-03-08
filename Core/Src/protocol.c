/**
  ******************************************************************************
  * @file    protocol.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2021-09-05
  * @brief   通信协议处理
  *	    修改记录 :
  *		版本号  日期              作者             说明
  *         V1.0    2021-09-05  SwaveYu     正式发布
  *         V1.1    2021-09-14  SwaveYu     增加了串口调参协议  
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  *
  ******************************************************************************
  */

/******************************** 头文件 *************************************/
#include "bsp.h"
#include "stdlib.h"

/****************************** 私有宏定义 ***********************************/

/******************************* 全局变量 ************************************/
int *adj_parm[PARM_NUM]; /* 串口调参用指针数组 */
int parma, parmb;

/***************************** 私有全局变量 **********************************/


/******************************* 私有函数 ************************************/


/**
 * @brief  通信协议解析处理函数
 * @param  PROTOCOL_MODETypeDef mode 解析模式, uint8_t *buf 处理的数据, uint16_t len 处理的数据长度
 * @retval   无
 */
int Protocol_Decode(PROTOCOL_MODETypeDef mode, uint8_t *buf, uint16_t len)
{
    if(mode == PROTOCOL_MODE_DEFAULT) { 
        /* 解析来自串口的时间设置指令，指令的格式为SET:N年月日时分秒，例：SET:2023-3-8-15:36:27 */
        if (memcmp(buf, FORMAT_SET, FORMAT_SET_LEN) == 0) {
            uint32_t date_time[6];
            uint32_t index = FORMAT_SET_LEN;
            uint32_t sn = 0;
            for (uint32_t i = FORMAT_SET_LEN; i < len; i++) {
                if (buf[i] == ',' || buf[i] == ';' || buf[i] == '-' || buf[i] == ':' || buf[i] == ' ') {
                    //buf[i] = '\0';
                    date_time[sn++] = atoi(buf + index);
                    index = i + 1;
                }
                else if (i == len - 1) {
                    date_time[sn] = atoi(buf + index);
                }
            }
            //RTC_Set(date_time[0], date_time[1], date_time[2], date_time[3], date_time[4], date_time[5]);
            RTC_Set_UnixTime(date_time[0], date_time[1], date_time[2], date_time[3], date_time[4], date_time[5]);
            return 0;
        }
        printf("解析模式%d匹配失败\n", mode);
        return -1;
    }
    else {
        return 0;
    }
}

/**
 * @brief  串口调参协议解析处理函数
* @param  uint8_t Buf[]    串口接收到的数据，例："ADJ:0,123;1,-1;4,321"
 * @retval   无
 */
void Protocol_AdjParm_Decode(uint8_t Buf[])
{
    uint32_t i,len = 0;
    int index = 3;
    int SN;
    
    len = strlen(Buf);    
    if(memcmp(Buf, "ADJ:", 4) == 0) {
        for(i = 4; i < len; i++) {
            if(Buf[i] == ',') {
                Buf[i] = '\0';
                SN = atoi(&Buf[index + 1]);
                if(SN < 0 || SN >= PARM_NUM) SN = 0; /* 防止指针数组下标越界 */
                index = i;
            }
            else if(Buf[i] == ';') {
                Buf[i] = '\0';
                *adj_parm[SN] = atoi(&Buf[index + 1]);
                index = i;
            }
            else if(i == len - 1) {
                *adj_parm[SN] = atoi(&Buf[index + 1]);
            }
        }
    }
    else return;
}

/**
 * @brief     将指针数组中的指针指向对应参数
 * @param  uint8_t Buf[]    串口接收到的数据
 * @retval   无
 */
void Protocol_AdjParm_SetPointer(void)
{
//    例
//    int a;
//    adj_parm[0] = &a;
    adj_parm[0] = &parma;
    adj_parm[1] = &parmb;
}
 /*********************************END OF FILE*********************************/
