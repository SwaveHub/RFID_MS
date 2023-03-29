  /**
  ******************************************************************************
  * @file    bsp_internal_flash.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   internal flash �ײ�Ӧ�ú���bsp 
  ******************************************************************************
  * @attention
  *
  * 
  * 
  * 
  *
  ******************************************************************************
  */
#include "./flash/bsp_internal_flash.h"

void InternalFlash_UpdateRecord(item_info_t* item_info, uint32_t dataLen);

/**
 @brief �ڲ�Flash��ȡ
 @param address -[in] ��ȡ�ĵ�ַ
 @param pData -[out] ָ����Ҫ����������
 @param dataLen -[in] ���ݳ���
 @return �����ɹ����ֽ���
*/
uint32_t Internal_ReadFlash(uint32_t addrStart, void *pData, uint32_t dataLen)
{
    uint32_t nread = dataLen;
    uint8_t *pBuffer = (uint8_t *)pData;
    const uint8_t *pAddr = (const uint8_t *)addrStart;

    if(!pData || addrStart < STM32FLASH_BASE || addrStart > STM32FLASH_END)
    {
        return 0;
    }

    while(nread >= sizeof(uint32_t) && (((uint32_t)pAddr) <= (STM32FLASH_END - 4)))
    {
        *(uint32_t *)pBuffer = *(uint32_t *)pAddr;
        pBuffer += sizeof(uint32_t);
        pAddr += sizeof(uint32_t);
        nread -= sizeof(uint32_t);
    }

    while(nread && (((uint32_t)pAddr) < STM32FLASH_END))
    {
        *pBuffer++ = *pAddr++;
        nread--;
    }

    return dataLen - nread;
}

/**
 @brief �ڲ�Flash�޼��д��
 @param address -[in] д��ĵ�ַ
 @param pData -[in] ָ����Ҫ����������
 @param dataLen -[in] ���ݳ���
 @return ʵ��д�������������λ������
*/
uint32_t Internal_WriteFlashNoCheck(uint32_t addrStart, const uint16_t *pData, uint32_t dataLen)
{
    uint32_t nwrite = dataLen;
    uint32_t addrmax = STM32FLASH_END - 2;
    /* ����FLASH */
    HAL_FLASH_Unlock();
    while(nwrite)
    {
        if(addrStart > addrmax)
        {
            break;
        }

        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addrStart, *pData);
        if((*(__IO uint16_t*) addrStart) != *pData)
        {
            break;
        }

        nwrite--;
        pData++;
        addrStart += 2;
    }
    /* ����FLASH */
    HAL_FLASH_Lock();        
    return (dataLen - nwrite);
}

/**
 @brief �ڲ�Flashд��
 @param address -[in] д��ĵ�ַ
 @param pData -[in] ָ����Ҫ����������
 @param dataLen -[in] ���ݳ���
 @return ʵ��д�������������λ������
*/
uint32_t Internal_WriteFlash(uint32_t addrStart, const uint16_t *pData, uint32_t dataLen)
{   
    uint32_t i = 0;
    uint32_t pagepos = 0;         // ҳλ��
    uint32_t pageoff = 0;         // ҳ��ƫ�Ƶ�ַ
    uint32_t pagefre = 0;         // ҳ�ڿ���ռ�
    uint32_t offset = 0;          // Address��FLASH�е�ƫ��
    uint32_t nwrite = dataLen;    // ��¼ʣ��Ҫд���������
    const uint16_t *pBuffer = (const uint16_t *)pData;
    uint16_t *FlashBuffer = (uint16_t *)malloc(sizeof(uint16_t) * (STM32FLASH_PAGE_SIZE >> 1));
    
    /* �Ƿ���ַ */
    if(addrStart < STM32FLASH_BASE || addrStart > (STM32FLASH_END - 2) || dataLen == 0 || pData == NULL)
    {
        return 0;
    }

    /* ����FLASH */
    HAL_FLASH_Unlock();

    /* ����ƫ�Ƶ�ַ */
    offset = addrStart - STM32FLASH_BASE;
    /* ���㵱ǰҳλ�� */
    pagepos = offset / STM32FLASH_PAGE_SIZE;
    /* ����Ҫд���ݵ���ʼ��ַ�ڵ�ǰҳ�ڵ�ƫ�Ƶ�ַ */
    pageoff = ((offset % STM32FLASH_PAGE_SIZE) >> 1);
    /* ���㵱ǰҳ�ڿ���ռ� */
    pagefre = ((STM32FLASH_PAGE_SIZE >> 1) - pageoff);
    /* Ҫд������������ڵ�ǰҳ������ */
    if(nwrite <= pagefre)
    {
        pagefre = nwrite;
    }
    
    while(nwrite != 0)
    {
        /* ����Ƿ�ҳ */
        if(pagepos >= STM32FLASH_PAGE_NUM)
        {
            break;
        }

        /* ��ȡһҳ */
        Internal_ReadFlash(STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE, FlashBuffer, STM32FLASH_PAGE_SIZE);

        /* ����Ƿ���Ҫ���� */
        for(i = 0; i < pagefre; i++)
        {
            if(*(FlashBuffer + pageoff + i) != 0xFFFF) /* FLASH������Ĭ������ȫΪ0xFF */
            {
                break;
            }
        }

        if(i < pagefre)
        {
            uint32_t count = 0;
            uint32_t index = 0;
            uint32_t PageError = 0;
            FLASH_EraseInitTypeDef pEraseInit;

            /* ����һҳ */
            pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
            pEraseInit.PageAddress = STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE;
            pEraseInit.Banks = FLASH_BANK_1;
            pEraseInit.NbPages = 1;
            if(HAL_FLASHEx_Erase(&pEraseInit, &PageError) != HAL_OK)
            {
                break;
            }

            /* ���Ƶ����� */
            for(index = 0; index < pagefre; index++)
            {
                *(FlashBuffer + pageoff + index) = *(pBuffer + index);
            }

            /* д��FLASH */
            count = Internal_WriteFlashNoCheck(STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE, FlashBuffer, STM32FLASH_PAGE_SIZE >> 1);
            if(count != (STM32FLASH_PAGE_SIZE >> 1))
            {
                nwrite -= count;
                break;
            }
        }
        else
        {
            /* ���������ֱ��д */
            uint32_t count = Internal_WriteFlashNoCheck(addrStart, pBuffer, pagefre);
            if(count != pagefre)
            {
                nwrite -= count;
                break;
            }
        }

        pBuffer += pagefre;         /* ��ȡ��ַ����         */
        addrStart += (pagefre << 1);  /* д���ַ����         */
        nwrite -= pagefre;          /* ����ʣ��δд�������� */

        pagepos++;     /* ��һҳ           */
        pageoff = 0;   /* ҳ��ƫ�Ƶ�ַ����  */

        /* ����ʣ���������´�д�������� */
        pagefre = nwrite >= (STM32FLASH_PAGE_SIZE >> 1) ? (STM32FLASH_PAGE_SIZE >> 1) : nwrite;
    }

    /* ����FLASH */
    HAL_FLASH_Lock();
    free(FlashBuffer);

    return ((dataLen - nwrite) << 1);
}

/**
 @brief �ڲ�Flashҳ����
 @param pageAddress -[in] ��������ʼ��ַ
 @param nbPages -[in] ����ҳ��
 @return 0 - �ɹ���-1 - ʧ��
*/
int Internal_ErasePage(uint32_t pageAddress, uint32_t nbPages)
{
	uint32_t pageError = 0;
	FLASH_EraseInitTypeDef eraseInit;
    /* ����FLASH */
    HAL_FLASH_Unlock();
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	eraseInit.PageAddress = pageAddress;
	eraseInit.Banks = FLASH_BANK_1;
	eraseInit.NbPages = nbPages;
	if(HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK)
	{
        /* ����FLASH */
        HAL_FLASH_Lock();    
		return -1;
	}
    /* ����FLASH */
    HAL_FLASH_Lock();    
	return 0;
}

/**
 @brief �ڲ�Flash���ҿ�д���ǩ��¼����ʼ��ַ
 @param addrStart -[in] ����������ʼ��ַ
 @param dataLen -[in] ���ݳ���
 @return ��д���ǩ��¼����ʼ��ַ
*/
uint32_t InternalFlash_FindStartAddr(uint32_t addrStart, uint32_t dataLen)
{
    uint16_t mark = 0;
    uint32_t cnt = 0;
    
    do {
        if (Internal_ReadFlash(addrStart + cnt * dataLen, &mark, 2) == 0) return 0; //�ڲ�flash��ȡʧ�ܷ���0
         cnt++;
    } while (mark != 0xFFFF);
    
    return addrStart + --cnt * dataLen;
}

/**
 @brief �ڲ�Flash����ָ����ǩTID��¼����ʼ��ַ
 @param addrStart -[in] ����������ʼ��ַ
 @param dataLen -[in] ���ݳ���
 @return ָ����ǩTID��¼����ʼ��ַ
*/
uint32_t InternalFlash_FindTIDAddr(uint8_t *tid, uint32_t addrStart, uint32_t dataLen)
{
    uint32_t cnt = 0;
    item_info_t *item_info_new = (item_info_t *)malloc(sizeof(item_info_t));
    
    do {
        Internal_ReadFlash(WRITE_START_ADDR + cnt * dataLen, (uint16_t *)item_info_new, dataLen);
        if (memcmp(tid, item_info_new->TID, 12) == 0) {
            free(item_info_new);
            return WRITE_START_ADDR + cnt * dataLen;
        }
         cnt++;
    } while (item_info_new->mark != 0xFFFF);
    
    return 0;
}

/**
 @brief �ڲ�Flashд���ǩ��¼
 @param head_ref -[in] ָ����Ҫ��������������ͷ
 @param dataLen -[in] �������������ݳ���
 @return ��
*/
void InternalFlash_WriteRecord(item_info_t** head_ref, uint32_t dataLen)
{
    item_info_t* current = *head_ref;
    uint32_t cnt = 0;
    uint32_t addrStart = 0;
    
    while (current != NULL) {
        //���ݲ�ͬ��mark���ò�ͬ�ķ�ʽд��¼��flash
        if (current->mark == MARK_INSTORAGE) {
            addrStart = InternalFlash_FindStartAddr(WRITE_START_ADDR, dataLen); //���ҿ�д���ǩ��¼����ʼ��ַ
            if (addrStart >= WRITE_START_ADDR && addrStart <= WRITE_END_ADDR - dataLen) { //��ַ�Ϸ�
                Internal_WriteFlash(addrStart , (uint16_t *)current, dataLen / 2);
            }
        } else if (current->mark == MARK_OUTSTORAGE) {
            InternalFlash_UpdateRecord(current, dataLen);
        }
        current = current->next;
        cnt++;
    }
}

/**
 @brief �ڲ�Flash��ȡ��ǩ��¼
 @param dataLen -[in] �������������ݳ���
 @return ��
*/
void InternalFlash_ReadRecord(uint32_t dataLen)
{
    item_info_t *item_info_new = (item_info_t *)malloc(sizeof(item_info_t));
    
    uint8_t *p = (uint8_t *)item_info_new;
    uint32_t cnt = 0;
    
    do {
        if (Internal_ReadFlash(WRITE_START_ADDR + cnt * dataLen, (uint16_t *)item_info_new, dataLen) == 0) {
            free(item_info_new);
            return ; //�ڲ�flash��ȡʧ�ܷ���
        }
        if(item_info_new->mark != 0xFFFF) {
           for (int i = 0; i < dataLen; i++) {
                printf("%02X ", p[i]);
            }
            printf("\r\n");
        } else {
            free(item_info_new);
            return;
        }
         cnt++;
    } while (1);    

}

/**
 @brief �ڲ�Flash���ñ�ǩ��¼��
 @param addrStart -[in] ����������ʼ��ַ
 @param addrEnd -[in] �������������ַ
 @return ��
*/
void InternalFlash_ResetRecord(uint32_t addrStart, uint32_t addrEnd)
{
    uint32_t nbPages = (addrEnd - addrStart) / STM32FLASH_PAGE_SIZE;
    
    Internal_ErasePage(addrStart, nbPages);
}

/**
 @brief �ڲ�Flash���±�ǩ��¼
 @param head_ref -[in] ָ����Ҫ��������������ͷ
 @param dataLen -[in] ���ݳ���
 @return ��
*/
void InternalFlash_UpdateRecord(item_info_t* item_info, uint32_t dataLen)
{
    item_info_t* item_info_new = malloc(sizeof(item_info_t));
    uint32_t addrStart = 0; 
    
   addrStart = InternalFlash_FindTIDAddr(item_info->TID, WRITE_START_ADDR, dataLen);
    if (addrStart >= WRITE_START_ADDR && addrStart <= WRITE_END_ADDR - dataLen) { //��ַ�Ϸ�
        Internal_ReadFlash(addrStart, (uint16_t *)item_info_new, dataLen);
        item_info_new->mark = item_info->mark;
        item_info_new->outstorage_time = item_info->outstorage_time;
        Internal_WriteFlashNoCheck(addrStart, (uint16_t *)item_info_new, dataLen / 2);
    }
    free(item_info_new);
}

/*********************************************END OF FILE**********************/

