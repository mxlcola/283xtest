/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_pwm.c
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ÔëÉù´«¸ÐÆ÷
*******************************************************************************/
#include "cry_pwm.h"



void pwm_init(void)
{
	stc_timer6_basecnt_cfg_t         stcTIM6BaseCntCfg;
	stc_timer6_port_output_cfg_t     stcTIM6PWMxCfg;
	
	PWC_Fcg2PeriphClockCmd(PWC_FCG2_PERIPH_TIM61, Enable);
	
	PORT_SetFunc(PWM_PORT, PWM_PIN, Func_Tim6, Disable); 
	
	
	stcTIM6BaseCntCfg.enCntMode   = Timer6CntTriangularModeA;           //Triangular wave mode
    stcTIM6BaseCntCfg.enCntDir    = Timer6CntDirUp;                     //Counter counting up
    stcTIM6BaseCntCfg.enCntClkDiv = Timer6PclkDiv1;                     //Count clock: pclk0
    Timer6_Init(M4_TMR61, &stcTIM6BaseCntCfg);   
	
	
	Timer6_SetPeriod(M4_TMR61, Timer6PeriodA, PWM_PERIOD);
	Timer6_SetGeneralCmpValue(M4_TMR61, Timer6GenCompareA, PWM_PERIOD / 2); //Ä¬ÈÏ50% 
	
	
    stcTIM6PWMxCfg.bOutEn     = Enable;                     //Output enable
    stcTIM6PWMxCfg.enPerc     = Timer6PWMxCompareLow;       //PWMA port output Low level when CNTER value match PERAR
    stcTIM6PWMxCfg.enCmpc     = Timer6PWMxCompareHigh;      //PWMA port output High level when CNTER value match with GCMAR
    stcTIM6PWMxCfg.enStaStp   = Timer6PWMxStateSelSS;       //PWMA output status is decide by STACA STPCA when CNTER start and stop
    stcTIM6PWMxCfg.enStaOut   = Timer6PWMxPortOutLow;       //PWMA port output set low level when CNTER start
    stcTIM6PWMxCfg.enStpOut   = Timer6PWMxPortOutLow;       //PWMA port output set low level when CNTER stop
    stcTIM6PWMxCfg.enDisVal   = Timer6PWMxDisValLow;
    Timer6_PortOutputConfig(M4_TMR61, Timer6PWMA, &stcTIM6PWMxCfg);
	
	
	Timer6_SetFunc(M4_TMR61, Timer6PWMA, Timer6ModeCompareOutput);
	Timer6_StartCount(M4_TMR61);
}


void pwm_SetCmpValue(uint16_t compValue)
{
	Timer6_SetGeneralCmpValue(M4_TMR61, Timer6GenCompareA, compValue); 
}




