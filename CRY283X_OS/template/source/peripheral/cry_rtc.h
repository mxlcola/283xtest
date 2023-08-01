/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_rtc.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x 噪声传感器
*******************************************************************************/
#ifndef __CRY_RTC_H__
#define __CRY_RTC_H__

#include "hc32_ddl.h"

#define XTAL32_MEASURE_LOWER            20000u
#define XTAL32_MEASURE_UPPER            40000u

void rtc_init(void);

void rtc_CalendarConfig(void); // 重置RTC时间 到2000年1月1日下午0点0分0秒

#endif
