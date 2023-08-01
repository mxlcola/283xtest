/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_adc.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ����������
*******************************************************************************/
#ifndef __CRY_ADC_H__
#define __CRY_ADC_H__

#include "hc32_ddl.h"

#define BAT_DETECT_ADC_PORT			(PortA)
#define BAT_DETECT_ADC_PIN			(Pin00)

#define ADC_REMAP_PIN            	(ADC1_IN0)
#define ADC_REMAP_CH             	(ADC1_CH0)
#define ADC_REMAP_CH_COUNT			(1u)
#define ADC_INTERNAL_INDEX			(ADC_CH_IDX0)

#define ADC_RESOLUTION_8BIT         (8u)
#define ADC_RESOLUTION_10BIT        (10u)
#define ADC_RESOLUTION_12BIT        (12u)

#define ADC_RESOLUTION             	(ADC_RESOLUTION_12BIT)

#define ADC_VREF                    (5.0f)	// ADC�ο���ѹ
#define ADC_PGA                    	(3.0f)  // ADC�Ŵ�������
#define ADC_ACCURACY               	(1ul << ADC_RESOLUTION)

#define VOLTAGE_CHARGE_FULL			(6.0f)		// ����
#define VOLTAGE_CHARGE_EMPT			(3.6f)		// �յ�



void adc_init(void);
float getAdcValue(void); 			// ��ȡADC��ѹ
float getVoltagePercent(void);		// ��ȡ��ص�ѹ�ٷֱ� ��VOLTAGE_CHARGE_EMPT ~ VOLTAGE_CHARGE_FULL��


#endif

