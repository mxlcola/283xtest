/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_adc.c
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ÔëÉù´«¸ÐÆ÷
*******************************************************************************/
#include "cry_adc.h"


void adc_init(void)
{
	stc_port_init_t stcPortInit;
    stc_adc_init_t stcAdcInit;
	stc_adc_ch_cfg_t stcChCfg;
	uint8_t au8Adc1SaSampTime[ADC_REMAP_CH_COUNT] = {0x30};

	MEM_ZERO_STRUCT(stcPortInit);
    MEM_ZERO_STRUCT(stcAdcInit);
	MEM_ZERO_STRUCT(stcChCfg);
	
	PWC_Fcg3PeriphClockCmd(PWC_FCG3_PERIPH_ADC1, Enable);
	
#if (ADC_RESOLUTION == ADC_RESOLUTION_8BIT)
    stcAdcInit.enResolution = AdcResolution_8Bit;
#elif (ADC_RESOLUTION == ADC_RESOLUTION_10BIT)
    stcAdcInit.enResolution = AdcResolution_10Bit;
#else
    stcAdcInit.enResolution = AdcResolution_12Bit;
#endif
    stcAdcInit.enDataAlign  = AdcDataAlign_Right;
    stcAdcInit.enAutoClear  = AdcClren_Disable;
    stcAdcInit.enScanMode   = AdcMode_SAOnce;


    ADC_Init(M4_ADC1, &stcAdcInit);	
	ADC_ChannelRemap(M4_ADC1, ADC_REMAP_CH, ADC_REMAP_PIN);
	
	stcPortInit.enPinMode = Pin_Mode_Ana;
    stcPortInit.enPullUp  = Disable;
	PORT_Init(BAT_DETECT_ADC_PORT, BAT_DETECT_ADC_PIN, &stcPortInit);
	
	
	stcChCfg.u32Channel  = ADC_REMAP_CH;
    stcChCfg.u8Sequence  = ADC_SEQ_A;
    stcChCfg.pu8SampTime = au8Adc1SaSampTime;
	

	ADC_AddAdcChannel(M4_ADC1, &stcChCfg);


//	PWC_Fcg3PeriphClockCmd(PWC_FCG3_PERIPH_CMP, Enable);
//	CMP_ADC_SetRefVoltPath(CmpAdcRefVoltPathVref);
//	PWC_PwrMonitorCmd(Enable);

//	ADC_ConfigAvg(M4_ADC1, AdcAvcnt_32);
//	ADC_AddAvgChannel(M4_ADC1, ADC_REMAP_CH);
}


float getAdcValue(void)
{
//	uint16_t m_au16Adc1Value[ADC_REMAP_CH_COUNT] = {0};
//	
//	ADC_PollingSa(M4_ADC1, m_au16Adc1Value, ADC_REMAP_CH_COUNT, 10u);
//	
//	return ((float)m_au16Adc1Value[0u] * ADC_VREF) / (float)ADC_ACCURACY;
	
	
	ADC_StartConvert(M4_ADC1);
	while (Reset == ADC_GetEocFlag(M4_ADC1, ADC_SEQ_A))
	{
		;
	}
	uint16_t m_u16AdcValue = ADC_GetValue(M4_ADC1, ADC_INTERNAL_INDEX);
	
	ADC_ClrEocFlag(M4_ADC1, ADC_SEQ_A);
	
	return ((float)(m_u16AdcValue - 2192) * ADC_VREF * ADC_PGA) / (float)ADC_ACCURACY;
}

float getVoltagePercent(void)
{
	float voltage = getAdcValue();
	
	if(voltage < VOLTAGE_CHARGE_EMPT)
	{
		return 0;
	}
	if(voltage > VOLTAGE_CHARGE_FULL)
	{
		return 100;
	}
	return (voltage - VOLTAGE_CHARGE_EMPT) * 100.0f / (VOLTAGE_CHARGE_FULL - VOLTAGE_CHARGE_EMPT);
}



