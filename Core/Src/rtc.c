/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */
HAL_StatusTypeDef status;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
   if (HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR1)!=0X5050)//是否第一次配置
	{			  
		HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0X5050);//标记已经初始化过了
	}
    else return;
    printf("FIRST TIME\n");
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 1;
  DateToUpdate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/*
*********************************************************************************************************
*	函 数 名: RTC_Set
*	功能说明: 设置RTC时间
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void RTC_Set(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
{
    RTC_TimeTypeDef TimeToUpdate = {0};
    RTC_DateTypeDef DateToUpdate = {0};
    uint32_t Unix;
    
    /* 设置年月日和时分秒 */
    TimeToUpdate.Hours = hour;
    TimeToUpdate.Minutes = minute;
    TimeToUpdate.Seconds = second;

    if (HAL_RTC_SetTime(&hrtc, &TimeToUpdate, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }

    DateToUpdate.Year = year % 100;
	DateToUpdate.Month = month;
	DateToUpdate.Date = day;
    if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
/* 操作寄存器来获取RTC_CNT中计数的值 */
uint32_t Read_UnixTime()
{
    uint32_t unix;
    unix = RTC->CNTH;
    unix <<= 16;
    unix += RTC->CNTL;
    return unix;
}

void Write_UnixTime(uint32_t unix)
{
    RTC->CRL |= 1 << 4;
    RTC->CNTH = unix >> 16;
    RTC->CNTL = unix & 0xFFFF;
    RTC->CRL &= ~ (1 << 4);
    return ;
}

/* USER CODE END 2 */

/* USER CODE END 3 */
/* 使用纯Unix时间戳方法来计算日期时间 */
void RTC_Set_UnixTime(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
{
    uint32_t Unix;
    
    system_time.year = year;
    system_time.month = month;
    system_time.day = day;
    system_time.hour = hour;
    system_time.minute = minute;
    system_time.second = second;
    
    Unix = YMD_To_Unix_Time(&system_time);
    Write_UnixTime(Unix);
}

uint32_t RTC_Get_UnixTime()
{
    return Read_UnixTime();
}

/* USER CODE END 3 */
