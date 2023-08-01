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


#define I2S_BUFFER_SIZE (512)		// 倍频程运算缓存区大小（不可修改）


extern uint32_t SLM_Overload; 		// 过载标识
extern uint32_t I2s_DSPFlag;  		// DSP算法运行标识

/*
 *		频率计权	:		A:A计权		C:C计权		Z:Z计权
 *		时间计权	:		F:F计权		S:S计权
 *		单位	:		dBV:伏特分贝			SPL:声压级
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

extern int32_t SLM_dBV_APeak;		// LA dBV 峰峰值
extern int32_t SLM_dBV_CPeak;		// LC dBV 峰峰值
extern int32_t SLM_dBV_ZPeak;		// LZ dBV 峰峰值

extern int32_t SLM_dBV_LAeq;		// LA dBV 等效声压级
extern int32_t SLM_dBV_LCeq;		// LC dBV 等效声压级
extern int32_t SLM_dBV_LZeq;		// LZ dBV 等效声压级

extern int32_t SLM_SPL_AF;			// LAF 声压级
extern int32_t SLM_SPL_AS;			// LAS 声压级
extern int32_t SLM_SPL_CF;			// LCF 声压级
extern int32_t SLM_SPL_CS;			// LCS 声压级
extern int32_t SLM_SPL_ZF;			// LZF 声压级
extern int32_t SLM_SPL_ZS;			// LZS 声压级	

extern int32_t SLM_SPL_AF_Max;		// LAF 声压级 MAX
extern int32_t SLM_SPL_AS_Max;		// LAS 声压级 MAX
extern int32_t SLM_SPL_CF_Max;		// LCF 声压级 MAX
extern int32_t SLM_SPL_CS_Max;		// LCS 声压级 MAX
extern int32_t SLM_SPL_ZF_Max;		// LZF 声压级 MAX
extern int32_t SLM_SPL_ZS_Max;		// LZS 声压级 MAX

extern int32_t SLM_SPL_AF_Min;		// LAF 声压级 MIN
extern int32_t SLM_SPL_AS_Min;		// LAS 声压级 MIN
extern int32_t SLM_SPL_CF_Min;		// LCF 声压级 MIN
extern int32_t SLM_SPL_CS_Min;		// LCS 声压级 MIN
extern int32_t SLM_SPL_ZF_Min;		// LZF 声压级 MIN
extern int32_t SLM_SPL_ZS_Min;		// LZS 声压级 MIN

extern int32_t SLM_SPL_APeak;		// LA 声压级 峰峰值
extern int32_t SLM_SPL_CPeak;		// LC 声压级 峰峰值
extern int32_t SLM_SPL_ZPeak;		// LS 声压级 峰峰值

extern int32_t SLM_SPL_LAeq;		// LA 声压级 等效声压级
extern int32_t SLM_SPL_LCeq;		// LC 声压级 等效声压级
extern int32_t SLM_SPL_LZeq;		// LZ 声压级 等效声压级

extern int32_t Oct3L_dBV[];			// 1/3 倍频程 dBV L
extern int32_t Oct3M_dBV[];			// 1/3 倍频程 dBV M (1/3 倍频程)
extern int32_t Oct3H_dBV[];			// 1/3 倍频程 dBV H

extern int32_t Oct3L_SPL[];			// 1/3 声压级 dBV L
extern int32_t Oct3M_SPL[];			// 1/3 声压级 dBV M (1/3 倍频程)
extern int32_t Oct3H_SPL[];			// 1/3 声压级 dBV H


// FLASH 参数
extern uint32_t FlashVal_SPLCalCoff;  	
extern uint32_t FlashVal_SPLCalValue;	
extern uint32_t FlashVal_Sens;	   
extern uint32_t FlashVal_CurrCal;	  
extern uint32_t FlashVal_LeqTime;	 
extern uint32_t FlashVal_LpmaxTime;   
extern uint32_t FlashVal_CurrOutType;	

//extern uint8_t Cmd_FreqWeigt; 				/* 频率计权   0：A计权  			1：C计权  2：Z计权  */
//extern uint8_t Cmd_TimeWeigt; 				/* 时间计权   0：F档    			1：S档  */
/***************当前计算 A/C/Z F/SQ全部计算*****************/
extern uint8_t Cmd_Range; 					/* 量程切换   0：自动切换    	1：0dB  2:35.25dB （默认为0 UI不需要设置）*/



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

