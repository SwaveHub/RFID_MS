#ifndef __LINKED_LIST_H
#define	__LINKED_LIST_H

/******************************** 头文件 *************************************/
#include <stdio.h>
#include <stdlib.h>
#include "stm32f1xx_hal.h"

/******************************** 宏定义 *************************************/
#define MARK_INSTORAGE 0xFC      //入库阶段标记
#define MARK_OUTSTORAGE 0xF0  //出库阶段标记
#define MARK_DIRTY 0xC0              //出库且上传后阶段标记

#define ITEM_INFO_SIZE   (sizeof(item_info_t) - sizeof(item_info_t *))

/***************************** 全局变量声明 **********************************/
typedef struct item_info_node{
    uint8_t mark;
    uint8_t TID[12]; 
    uint8_t name[9];
    uint8_t shelf;   
    uint8_t layer;
    uint32_t instorage_time;
    uint32_t outstorage_time;
    struct item_info_node *next;
}item_info_t;

extern item_info_t *item_info_head;
/******************************* 函数声明 ************************************/
void append(item_info_t** head_ref, item_info_t* new_node);
void deleteList(item_info_t** head_ref);

#endif /* __LINKED_LIST_H */
