#ifndef __PROTOCOL_H
#define	__PROTOCOL_H

/******************************** 头文件 *************************************/
#include "stm32f1xx_hal.h"

/******************************** 宏定义 *************************************/
#define FORMAT_SET  "SET:"
#define FORMAT_SET_LEN  (strlen(FORMAT_SET))

#define PARM_NUM  5

/***************************** 全局变量声明 **********************************/
extern int *adj_parm[PARM_NUM]; /* 串口调参用指针数组 */
extern int parma, parmb;

/* 定义协议处理模式 */
typedef enum {
    PROTOCOL_MODE_DEFAULT,
}PROTOCOL_MODETypeDef ;

/******************************* 函数声明 ************************************/
int Protocol_Decode(PROTOCOL_MODETypeDef mode, uint8_t *buf, uint16_t len);
void Protocol_AdjParm_Decode(uint8_t Buf[]);
void Protocol_AdjParm_SetPointer(void);
    
#endif /* __PROTOCOL_H */
