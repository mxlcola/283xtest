/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_i2c_adc_dac_rtc_power.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ����������
*******************************************************************************/
#ifndef __CRY_I2C_ADC_DAC_RTC_POWER_H__
#define __CRY_I2C_ADC_DAC_RTC_POWER_H__

#include "hc32_ddl.h"


// I2C ���ò���
#define I2C_UNIT                        (M4_I2C3)
#define E2_ADDRESS                      (0x43u)
#define E2_PAGE_LEN                     (0x39u)
#define E2_MEM_ADR_LEN                  (1u)
#define DATA_TEST_ADDR                  (0x08u)
#define I2C_SCL_PORT                    (PortB)
#define I2C_SCL_PIN                     (Pin08)
#define I2C_SDA_PORT                    (PortB)
#define I2C_SDA_PIN                     (Pin09)
#define I2C_GPIO_SCL_FUNC               (Func_I2c3_Scl)
#define I2C_GPIO_SDA_FUNC               (Func_I2c3_Sda)

#define I2C_FCG_USE                     (PWC_FCG1_PERIPH_I2C3)

#define I2C_TIMEOUT                   	(0x40000ul)


// I2C �豸���ò���
/***************ADC���ò���***********************************/
#define ES7243E					0x7A			//Read 0xFD == 0x7A�ж�IC�ͺ��Ƿ�Ϊ?ES7243E
#define ES7243L					0x72			//Read 0xFD == 0x72�ж�IC�ͺ��Ƿ�Ϊ?ES7243L
#define STATEconfirm			0xFC			//״̬��ȷ�� �ض�STATEconfirm�ļĴ���ֵȷ��IC��������

#define NORMAL_I2S			0x00
#define NORMAL_LJ			0x01
#define NORMAL_DSPA			0x03
#define NORMAL_DSPB			0x23
#define Format_Len24		0x00
#define Format_Len20		0x01
#define Format_Len18		0x02
#define Format_Len16		0x03
#define Format_Len32		0x04

#define VDDA_3V3			0x00
#define VDDA_1V8			0x01
#define MCLK_PIN			0x00
#define SCLK_PIN			0x01



#define Device 				ES7243E			//��Ʒѡ��7243E��EΪ3.3V���磬LΪ1.8V����
#define CHANNELS_MAX		4				//TDM_NFSģʽ�����ã�����CH��������Ϊż��
#define MSMode_MasterSelOn	1				//����ģʽѡ��0ѡ��SlaveMode,1ѡ��MasterMode
#define Ratio 				256				//Ratio=MCLK/LRCK
#define Format 				NORMAL_I2S
#define Format_Len			Format_Len32	//���ݳ���
#define SCLK_DIV			4				//SCLK��Ƶѡ��,SCLK=MCLK/SCLK_DIV
#define SCLK_INV			0				//���뷽ʽ��0Ϊ�½��أ�1Ϊ������
#define MCLK_SOURCE			MCLK_PIN		//ʹ��MCLK

#define VDDA_VOLTAGE		VDDA_3V3		//ģ���ѹ3.3V
#define ADC_PGA_GAIN		0				//ADCģ������(0~14)
#define ADC_Volume			191				//ADC��������(0~255),191:0DB,0.5dB/Step
#define Dmic_Selon 			0				//DMICѡ��Ĭ��0�رգ�1��
#define Dmic_GAIN 			0				//DMIC���棺(0~7),6dB/Step

/***************DAC���ò���***********************************/
#define STATEconfirm_DAC		0x0C		//״̬��ȷ�� �ض�STATEconfirm��ֵȷ��IC��������״̬
#define NORMAL_I2S_DAC			0x00
#define NORMAL_LJ_DAC			0x01
#define NORMAL_DSPA_DAC			0x03
#define NORMAL_DSPB_DAC			0x07
#define Format_Len24_DAC		0x00
#define Format_Len20_DAC		0x01
#define Format_Len18_DAC		0x02
#define Format_Len16_DAC		0x03
#define Format_Len32_DAC		0x04


#define VDDA_3V3			0x00
#define VDDA_1V8			0x01
#define MCLK_PIN			0x00
#define SCLK_PIN			0x01
/***************��������***************/
/**************************************************/
#define MSMode_MasterSelOn_DAC	0				//��Ʒ����ģʽѡ��:Ĭ��ѡ��0ΪSlaveMode,��Ϊ1ѡ��MasterMode
#define Ratio_DAC 				256				//ʵ��Ratio=MCLK/LRCK����,��Ҫ��ʵ��ʱ�ӱ���ƥ��
#define Format_DAC 				NORMAL_I2S_DAC		//���ݸ�ʽѡ��,��Ҫ��ʵ��ʱ��ƥ��
#define Format_Len_DAC			Format_Len24_DAC	//���ݳ���ѡ��,��Ҫ��ʵ��ʱ��ƥ��
#define SCLK_DIV_DAC			4				//SCLK��Ƶѡ��:(ѡ��Χ1~128),SCLK=MCLK/SCLK_DIV
#define SCLK_INV_DAC			0				//Ĭ�϶��뷽ʽΪ�½���,1Ϊ�����ض���,��Ҫ��ʵ��ʱ��ƥ��
#define MCLK_SOURCE_DAC			MCLK_PIN		//�Ƿ�Ӳ��û��MCLK��Ҫ��SCLK����MCLK

#define VDDA_VOLTAGE_DAC		VDDA_3V3		//ģ���ѹѡ��Ϊ3V3����1V8,��Ҫ��ʵ��Ӳ��ƥ��
#define DAC_Volume_DAC			191				//DAC��������:(ѡ��Χ0~255),191:0DB,��0.5dB/Step
#define DACHPModeOn_DAC			0				//������ؿ���HP����:Ĭ��ѡ��ر�0,��Ϊ1


/***************SGM4154x��Դ����оƬ��ز���������Ҫ�������ã����������оƬ�����ֲ�***********************************/
#define SGM4154x_CHRG_CTRL_0	0x00
#define SGM4154x_CHRG_CTRL_1	0x01
#define SGM4154x_CHRG_CTRL_2	0x02
#define SGM4154x_CHRG_CTRL_3	0x03
#define SGM4154x_CHRG_CTRL_4	0x04
#define SGM4154x_CHRG_CTRL_5	0x05
#define SGM4154x_CHRG_CTRL_6	0x06
#define SGM4154x_CHRG_CTRL_7	0x07
#define SGM4154x_CHRG_STAT	    0x08
#define SGM4154x_CHRG_FAULT	    0x09
#define SGM4154x_CHRG_CTRL_a	0x0a
#define SGM4154x_CHRG_CTRL_b	0x0b
#define SGM4154x_CHRG_CTRL_c	0x0c
#define SGM4154x_CHRG_CTRL_d	0x0d
#define SGM4154x_INPUT_DET   	0x0e
#define SGM4154x_CHRG_CTRL_f	0x0f


/* charge voltage  */
#define SGM4154x_VREG_V_MASK		0xF8
#define SGM4154x_VREG_V_MAX_uV	    4624000
#define SGM4154x_VREG_V_MIN_uV	    3856000
#define SGM4154x_VREG_V_DEF_uV	    4208000
#define SGM4154x_VREG_V_STEP_uV	    32000

/* vindpm voltage  */
#define SGM4154x_VINDPM_V_MASK      0x0f
#define SGM4154x_VINDPM_V_MIN_uV    3900000
#define SGM4154x_VINDPM_V_MAX_uV    12000000
#define SGM4154x_VINDPM_STEP_uV     100000
#define SGM4154x_VINDPM_DEF_uV	    3600000
#define SGM4154x_VINDPM_OS_MASK     0x03


#define SGM4154x_IINDPM_I_MASK		0x1f
#define SGM4154x_IINDPM_I_MIN_uA	100000
#define SGM4154x_IINDPM_I_MAX_uA	3200000
#define SGM4154x_IINDPM_STEP_uA	    100000
#define SGM4154x_IINDPM_DEF_uA	    2400000


#define SGM4154x_TERMCHRG_CUR_MASK			0x0f
#define SGM4154x_TERMCHRG_CURRENT_STEP_uA	60000
#define SGM4154x_TERMCHRG_I_MIN_uA			60000
#define SGM4154x_TERMCHRG_I_MAX_uA			960000
#define SGM4154x_TERMCHRG_I_DEF_uA			180000


#define SGM4154x_PRECHRG_CUR_MASK			0xf0
#define SGM4154x_PRECHRG_CURRENT_STEP_uA	60000
#define SGM4154x_PRECHRG_I_MIN_uA			60000
#define SGM4154x_PRECHRG_I_MAX_uA			780000
#define SGM4154x_PRECHRG_I_DEF_uA			180000

#define SGM4154x_ICHRG_I_MASK			0x3f
#define SGM4154x_ICHRG_I_MIN_uA			0
#define SGM4154x_ICHRG_I_MAX_uA			3000000
#define SGM4154x_ICHRG_I_DEF_uA			1980000

#define SGM4154x_VRECHARGE              0x01
#define SGM4154x_VRECHRG_STEP_mV		100
#define SGM4154x_VRECHRG_OFFSET_mV		100



typedef struct _rtc_date_t {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} rtc_date_t;



void i2c_init(void);
void I2CWRNBYTE_CODEC_ADC(uint16_t WriteAddr,uint8_t DataToWrite);
void I2CWRNBYTE_CODEC_DAC(uint16_t WriteAddr,uint8_t DataToWrite);
void I2CWRNBYTE_CODEC_RTC(uint16_t WriteAddr,uint8_t DataToWrite);
void I2CWRNBUFFER_CODEC_RTC(uint16_t WriteAddr,uint8_t *DataToWrite, uint32_t u32Size);
void I2CWRDBUFFER_CODEC_RTC(uint16_t ReadAddr,uint8_t *DataToRecv, uint32_t u32Size);
void I2CWRNBYTE_CODEC_POWER(uint16_t WriteAddr,uint8_t DataToWrite);
void I2CWRDBYTE_CODEC_POWER(uint16_t WriteAddr,uint8_t *DataToRecv);
void I2CUPBYTE_CODEC_POWER(uint16_t UpdateAddr,uint8_t Mask, uint8_t DataToWrite);
void I2CWRNBYTE_CODEC_RTC(uint16_t WriteAddr,uint8_t DataToWrite);
void I2CWRNBUFFER_CODEC_RTC(uint16_t ReadAddr,uint8_t *DataToRecv, uint32_t u32Size);


void ES7243E_adc_init(void);
void ES7243L_adc_Standby(void);
void ES7243L_adc_Resume(void);

void ES8156_dac_init(void);
void ES8156_dac_Standby_0uA(void);
void ES8156_dac_Standby_NoPop(void);

void BMP8563_init(void);
void BMP8563_setTime(rtc_date_t *date);
void BMP8563_getTime(rtc_date_t *date);

void SGM415_power_init(void);
void SGM415_power_SetChrgVolt(uint32_t chrg_volt); 
uint32_t SGM415_power_GetChrgVolt(void);
void SGM415_power_SetChrgCurrLim(uint32_t chrg_uA);
uint32_t SGM415_power_GetCurrLim(void);
void SGM415_power_SetChrgTermCurr(uint32_t chrg_uA);
void SGM415_power_SetPreChrgCurr(uint32_t chrg_uA);
void SGM415_power_SetiChrgCurr(uint32_t chrg_uA);
uint8_t SGM415_power_GetVindpmOffsetOs(void);
void SGM415_power_SetVindpmOffsetOs(uint8_t offset_os);
void SGM415_power_SetReChargeCurr(uint32_t chrg_mV);


#endif
