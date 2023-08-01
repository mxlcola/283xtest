/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_rtc.c
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ÔëÉù´«¸ÐÆ÷
*******************************************************************************/
#include "cry_rtc.h"

#include <math.h>

static const float EPSINON = 0.000001f;

void Xtal32_ClockConfig(void)
{
    stc_clk_xtal32_cfg_t stcXtal32Cfg;

    MEM_ZERO_STRUCT(stcXtal32Cfg);

    CLK_Xtal32Cmd(Disable);
    Ddl_Delay1ms(100u);
    stcXtal32Cfg.enDrv = ClkXtal32HighDrv;
    stcXtal32Cfg.enFilterMode = ClkXtal32FilterModeFull;
    CLK_Xtal32Config(&stcXtal32Cfg);
    CLK_Xtal32Cmd(Enable);
    Ddl_Delay1ms(100u);
}

void Clock_MeasureConfig(void)
{
    stc_clk_fcm_cfg_t stcClkFcmCfg;
    stc_clk_fcm_window_cfg_t stcClkFcmWinCfg;
    stc_clk_fcm_measure_cfg_t stcClkFcmMeasureCfg;
    stc_clk_fcm_reference_cfg_t stcClkFcmReferCfg;
    stc_clk_fcm_interrupt_cfg_t stcClkFcmIntCfg;

    MEM_ZERO_STRUCT(stcClkFcmCfg);
    MEM_ZERO_STRUCT(stcClkFcmWinCfg);
    MEM_ZERO_STRUCT(stcClkFcmMeasureCfg);
    MEM_ZERO_STRUCT(stcClkFcmReferCfg);
    MEM_ZERO_STRUCT(stcClkFcmIntCfg);

    PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_FCM, Enable);

    stcClkFcmWinCfg.windowLower = XTAL32_MEASURE_LOWER;
    stcClkFcmWinCfg.windowUpper = XTAL32_MEASURE_UPPER;

    stcClkFcmMeasureCfg.enSrc = ClkFcmSrcXtal;
    stcClkFcmMeasureCfg.enSrcDiv = ClkFcmMeaDiv1;

    stcClkFcmReferCfg.enRefSel = ClkFcmInterRef;
    stcClkFcmReferCfg.enExtRef = Disable;
    stcClkFcmReferCfg.enIntRefSrc = ClkFcmSrcXtal32;
    stcClkFcmReferCfg.enIntRefDiv = ClkFcmIntrefDiv128;
    stcClkFcmReferCfg.enEdge = ClkFcmEdgeRising;
    stcClkFcmReferCfg.enFilterClk = ClkFcmFilterClkNone;

    stcClkFcmCfg.pstcFcmIntCfg = &stcClkFcmIntCfg;
    stcClkFcmCfg.pstcFcmMeaCfg = &stcClkFcmMeasureCfg;
    stcClkFcmCfg.pstcFcmRefCfg = &stcClkFcmReferCfg;
    stcClkFcmCfg.pstcFcmWindowCfg = &stcClkFcmWinCfg;

    CLK_FcmConfig(&stcClkFcmCfg);
}

uint16_t Rtc_GetCompenValue(void)
{
    uint32_t u32Tmp = 0ul;
    en_flag_status_t enStaTmp;
    float clkMeasureVal;
    uint16_t integerVal = 0u, decimalsVal = 0u;
    uint16_t clkCompenVal = 0u;
    stc_clk_freq_t stcClkFreq;

    MEM_ZERO_STRUCT(stcClkFreq);

	Clock_MeasureConfig();
	
    CLK_FcmCmd(Enable);
    do
    {
        enStaTmp = CLK_GetFcmFlag(ClkFcmFlagErrf);

        if ((Set == CLK_GetFcmFlag(ClkFcmFlagOvf)) || (Set == enStaTmp))
        {
            CLK_FcmCmd(Disable);
            CLK_ClearFcmFlag(ClkFcmFlagOvf);
            CLK_ClearFcmFlag(ClkFcmFlagErrf);
            u32Tmp = 0xffu;
			
        }
    } while (Reset == CLK_GetFcmFlag(ClkFcmFlagMendf));

    if (0xffu != u32Tmp)
    {
        CLK_GetClockFreq(&stcClkFreq);
        u32Tmp = CLK_GetFcmCounter();
        clkMeasureVal = ((float)stcClkFreq.sysclkFreq * 128.0f) / (float)u32Tmp;

        CLK_FcmCmd(Disable);
        CLK_ClearFcmFlag(ClkFcmFlagMendf);


       // if (!((clkMeasureVal >= -EPSINON) && (clkMeasureVal <= EPSINON)))
        {
            clkMeasureVal = (clkMeasureVal - (float)XTAL32_VALUE) / (float)XTAL32_VALUE * (float)1000000.0f;
            clkMeasureVal = clkMeasureVal * (float)XTAL32_VALUE / 1000000.0f;

            if (clkMeasureVal < -EPSINON)    
            {
                clkMeasureVal = (float)fabs((double)clkMeasureVal);
                integerVal = (uint16_t)(((~((uint32_t)clkMeasureVal)) + 1u) & 0x0Fu);
                /* Magnify one thousand times */
                u32Tmp = (uint32_t)clkMeasureVal;
                clkMeasureVal = (clkMeasureVal - (float)u32Tmp) * 1000.0f;
                decimalsVal = (uint16_t)((((~((uint32_t)clkMeasureVal)) & 0x3E0u) >> 5u) + 1u);
            }
            else                           
            {
                clkMeasureVal += 1.0f;
                integerVal = (uint16_t)(((uint32_t)clkMeasureVal) & 0x0Fu);
                u32Tmp = (uint32_t)clkMeasureVal;
                clkMeasureVal = (float)((clkMeasureVal - (float)u32Tmp) * 1000.0f);
                decimalsVal = (uint16_t)(((uint32_t)clkMeasureVal & 0x3E0u) >> 5u);
            }
        }
        clkCompenVal = ((uint16_t)(integerVal << 5u) | decimalsVal) & 0x1FFu;
    }
	printf(" %d %f %d\r\n", clkCompenVal, clkMeasureVal, u32Tmp);
    return clkCompenVal;
}

void rtc_CalendarConfig(void)
{
    stc_rtc_date_time_t stcRtcDateTimeCfg;

    MEM_ZERO_STRUCT(stcRtcDateTimeCfg);

    stcRtcDateTimeCfg.u8Year = 00u;
    stcRtcDateTimeCfg.u8Month = 1u;
    stcRtcDateTimeCfg.u8Day = 1u;
    stcRtcDateTimeCfg.u8Weekday = RtcWeekdayWednesday;
    stcRtcDateTimeCfg.u8Hour = 0u;
    stcRtcDateTimeCfg.u8Minute = 0u;
    stcRtcDateTimeCfg.u8Second = 0u;
	
    if (RTC_SetDateTime(RtcDataFormatDec, &stcRtcDateTimeCfg, Enable, Enable) != Ok)
    {
        printf("write calendar failed!\r\n");
    }
}

void rtc_init(void)
{
	stc_rtc_init_t stcRtcInit;
	
	MEM_ZERO_STRUCT(stcRtcInit);

	
	//if (0u == M4_RTC->CR1_f.START)
	{
		if (RTC_DeInit() == ErrorTimeout)
        {
            printf("Reset RTC failed!\r\n");
        }
        else
        {
			Xtal32_ClockConfig();
			
            stcRtcInit.enClkSource = RtcClkXtal32;
            stcRtcInit.enPeriodInt = RtcPeriodIntOneSec;
            stcRtcInit.enTimeFormat = RtcTimeFormat24Hour;
            stcRtcInit.enCompenWay = RtcOutputCompenUniform;
            stcRtcInit.enCompenEn = Enable;
            stcRtcInit.u16CompenVal = Rtc_GetCompenValue();
            RTC_Init(&stcRtcInit);

            RTC_IrqCmd(RtcIrqPeriod, Enable);
            RTC_Cmd(Enable);
			RTC_OneHzOutputCmd(Enable);
			
            Ddl_Delay1ms(1u);
			
            rtc_CalendarConfig();
        }
	}
	

	stc_i2c_init_t stcI2cInit;
	
	
	MEM_ZERO_STRUCT(stcI2cInit);
	
}

