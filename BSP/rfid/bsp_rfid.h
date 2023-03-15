#ifndef __RFID_H
#define	__RFID_H

#include "stm32f1xx_hal.h"
#include ".\rfid\bsp_rf200_rfid.h"

#define MAX_TAG_FILTER_NUM  50

extern unsigned int flag1s;         //  1000ms溢出标志位
extern uint8_t tag_cnt;    // 标签计数

typedef struct {// 筛选出的标签
    uint8_t len;
    TAG_STA_t tag[MAX_TAG_FILTER_NUM];
}tag_filter_t;

extern tag_filter_t tag_filter;

void contain_test(void);
void contain_test1(void);
void contain_test2(void);

void RFID_GetSingleTID();
void RFID_FilterTag(uint8_t *cnt);

#endif /* __RFID_H */

