#ifndef __LINKED_LIST_H
#define	__LINKED_LIST_H

/******************************** ͷ�ļ� *************************************/
#include <stdio.h>
#include <stdlib.h>
#include "stm32f1xx_hal.h"

/******************************** �궨�� *************************************/
#define MARK_INSTORAGE 0xFCFC      //���׶α��
#define MARK_OUTSTORAGE 0x0000  //����׶α��

#define ITEM_NAME_SIZE  13
#define ITEM_INFO_SIZE   (sizeof(item_info_t) - sizeof(item_info_t *))

/***************************** ȫ�ֱ������� **********************************/
typedef struct item_info_node{
    uint16_t mark;
    uint8_t TID[12]; 
    uint8_t name[ITEM_NAME_SIZE];
    uint8_t shelf;   
    uint8_t layer;
    uint32_t instorage_time;
    uint32_t outstorage_time;
    struct item_info_node *next;
}item_info_t;

extern item_info_t *item_info_head;
/******************************* �������� ************************************/
void append(item_info_t** head_ref, item_info_t* new_node);
void deleteList(item_info_t** head_ref);

#endif /* __LINKED_LIST_H */
