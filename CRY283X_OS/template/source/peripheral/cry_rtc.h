/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_rtc.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ����������
*******************************************************************************/
#ifndef __CRY_RTC_H__
#define __CRY_RTC_H__

#include "hc32_ddl.h"

#define XTAL32_MEASURE_LOWER            20000u
#define XTAL32_MEASURE_UPPER            40000u

void rtc_init(void);

void rtc_CalendarConfig(void); // ����RTCʱ�� ��2000��1��1������0��0��0��

#endif
