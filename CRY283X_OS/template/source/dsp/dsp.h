/******************** (C) COPYRIGHT 2008 CRY ********************
* File Name          : dsp.h
* Author             : Jason Cao
* Version            : V1.0.0
* Date               : 04/27/2013
* Description        : Header for dsps.c module
*/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __DSP_H
#define __DSP_H

/* Includes ------------------------------------------------------------------*/

#include "math.h"
#include "stdint.h"


#define I2S_BUFFER_SIZE (512)		// ��Ƶ�����㻺������С�������޸ģ�


extern uint32_t SLM_Overload; 		// ���ر�ʶ
extern uint32_t I2s_DSPFlag;  		// DSP�㷨���б�ʶ

/*
 *		Ƶ�ʼ�Ȩ	:		A:A��Ȩ		C:C��Ȩ		Z:Z��Ȩ
 *		ʱ���Ȩ	:		F:F��Ȩ		S:S��Ȩ
 *		��λ	:		dBV:���طֱ�			SPL:��ѹ��
 */
	
extern int32_t SLM_dBV_AF;	 		// LAF dBV
extern int32_t SLM_dBV_AS;			// LAS dBV
extern int32_t SLM_dBV_CF;			// LCF dBV
extern int32_t SLM_dBV_CS;			// LCS dBV
extern int32_t SLM_dBV_ZF;			// LZF dBV
extern int32_t SLM_dBV_ZS;			// LZS dBV

extern int32_t SLM_dBV_AF_Max;		// LAF dBV MAX
extern int32_t SLM_dBV_AS_Max;		// LAS dBV MAX
extern int32_t SLM_dBV_CF_Max;		// LCF dBV MAX
extern int32_t SLM_dBV_CS_Max;		// LCS dBV MAX
extern int32_t SLM_dBV_ZF_Max;		// LZF dBV MAX
extern int32_t SLM_dBV_ZS_Max;		// LZS dBV MAX

extern int32_t SLM_dBV_AF_Min;		// LAF dBV MIN
extern int32_t SLM_dBV_AS_Min;		// LAS dBV MIN
extern int32_t SLM_dBV_CF_Min;		// LCF dBV MIN
extern int32_t SLM_dBV_CS_Min;		// LCS dBV MIN
extern int32_t SLM_dBV_ZF_Min;		// LZF dBV MIN
extern int32_t SLM_dBV_ZS_Min;		// LZS dBV MIN

extern int32_t SLM_dBV_APeak;		// LA dBV ���ֵ
extern int32_t SLM_dBV_CPeak;		// LC dBV ���ֵ
extern int32_t SLM_dBV_ZPeak;		// LZ dBV ���ֵ

extern int32_t SLM_dBV_LAeq;		// LA dBV ��Ч��ѹ��
extern int32_t SLM_dBV_LCeq;		// LC dBV ��Ч��ѹ��
extern int32_t SLM_dBV_LZeq;		// LZ dBV ��Ч��ѹ��

extern int32_t SLM_SPL_AF;			// LAF ��ѹ��
extern int32_t SLM_SPL_AS;			// LAS ��ѹ��
extern int32_t SLM_SPL_CF;			// LCF ��ѹ��
extern int32_t SLM_SPL_CS;			// LCS ��ѹ��
extern int32_t SLM_SPL_ZF;			// LZF ��ѹ��
extern int32_t SLM_SPL_ZS;			// LZS ��ѹ��	

extern int32_t SLM_SPL_AF_Max;		// LAF ��ѹ�� MAX
extern int32_t SLM_SPL_AS_Max;		// LAS ��ѹ�� MAX
extern int32_t SLM_SPL_CF_Max;		// LCF ��ѹ�� MAX
extern int32_t SLM_SPL_CS_Max;		// LCS ��ѹ�� MAX
extern int32_t SLM_SPL_ZF_Max;		// LZF ��ѹ�� MAX
extern int32_t SLM_SPL_ZS_Max;		// LZS ��ѹ�� MAX

extern int32_t SLM_SPL_AF_Min;		// LAF ��ѹ�� MIN
extern int32_t SLM_SPL_AS_Min;		// LAS ��ѹ�� MIN
extern int32_t SLM_SPL_CF_Min;		// LCF ��ѹ�� MIN
extern int32_t SLM_SPL_CS_Min;		// LCS ��ѹ�� MIN
extern int32_t SLM_SPL_ZF_Min;		// LZF ��ѹ�� MIN
extern int32_t SLM_SPL_ZS_Min;		// LZS ��ѹ�� MIN

extern int32_t SLM_SPL_APeak;		// LA ��ѹ�� ���ֵ
extern int32_t SLM_SPL_CPeak;		// LC ��ѹ�� ���ֵ
extern int32_t SLM_SPL_ZPeak;		// LS ��ѹ�� ���ֵ

extern int32_t SLM_SPL_LAeq;		// LA ��ѹ�� ��Ч��ѹ��
extern int32_t SLM_SPL_LCeq;		// LC ��ѹ�� ��Ч��ѹ��
extern int32_t SLM_SPL_LZeq;		// LZ ��ѹ�� ��Ч��ѹ��

extern int32_t Oct3L_dBV[];			// 1/3 ��Ƶ�� dBV L
extern int32_t Oct3M_dBV[];			// 1/3 ��Ƶ�� dBV M (1/3 ��Ƶ��)
extern int32_t Oct3H_dBV[];			// 1/3 ��Ƶ�� dBV H

extern int32_t Oct3L_SPL[];			// 1/3 ��ѹ�� dBV L
extern int32_t Oct3M_SPL[];			// 1/3 ��ѹ�� dBV M (1/3 ��Ƶ��)
extern int32_t Oct3H_SPL[];			// 1/3 ��ѹ�� dBV H


// FLASH ����
extern uint32_t FlashVal_SPLCalCoff;  	
extern uint32_t FlashVal_SPLCalValue;	
extern uint32_t FlashVal_Sens;	   
extern uint32_t FlashVal_CurrCal;	  
extern uint32_t FlashVal_LeqTime;	 
extern uint32_t FlashVal_LpmaxTime;   
extern uint32_t FlashVal_CurrOutType;	

//extern uint8_t Cmd_FreqWeigt; 				/* Ƶ�ʼ�Ȩ   0��A��Ȩ  			1��C��Ȩ  2��Z��Ȩ  */
//extern uint8_t Cmd_TimeWeigt; 				/* ʱ���Ȩ   0��F��    			1��S��  */
/***************��ǰ���� A/C/Z F/SQȫ������*****************/
extern uint8_t Cmd_Range; 					/* �����л�   0���Զ��л�    	1��0dB  2:35.25dB ��Ĭ��Ϊ0 UI����Ҫ���ã�*/



void SLM_Basic_Cal(void);
void SLM_Statistics(void);
void SLM_Data_In16(uint16_t usData, uint8_t channel);
void SLM_Data_In32(int32_t usData, uint8_t channel);
int32_t SLM_OCT_Process(void);
int32_t SLM_Calibration(void);


void SLM_StartTest(void);
void SLM_StopTest(void);

int32_t Log_Lookup(uint64_t Value);


void SLM_DataBuf_LR_In32(int32_t *usDataL, int32_t *usDataR, uint32_t len);




#endif 

