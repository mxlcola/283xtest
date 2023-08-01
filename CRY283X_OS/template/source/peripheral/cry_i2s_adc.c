/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_i2s_adc.c
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x 噪声传感器
*******************************************************************************/
#include "cry_i2s_adc.h"

#ifdef USE_RT_THREAD
#include "rtthread.h"

static rt_mq_t i2s2_mq = RT_NULL;
#endif

static int32_t i2s_data_in = 0;
static int32_t i2s_data_in_buffer[I2S2_BUFFER_SIZE];
static uint32_t i2s_data_count = 0;
static uint8_t i2s_data_full_flag = 0;

//static int32_t au32RecorderSoundI2s[1024];

void I2SRx_CallBack(void);
void I2STx_CallBack(void);

uint32_t GetI2SClkFreq(M4_I2S_TypeDef* pstcI2sReg)
{
    en_clk_peri_source_t enSrc = ClkPeriSrcPclk;
    uint32_t u32Freq = 0ul;
    stc_clk_freq_t stcClkFreq;
    stc_pll_clk_freq_t stcPllClkFreq;

    if(NULL != pstcI2sReg)
    {
        enSrc = CLK_GetI2sClkSource(pstcI2sReg);
        CLK_GetClockFreq(&stcClkFreq);
        CLK_GetPllClockFreq(&stcPllClkFreq);
        switch(enSrc)
        {
            case ClkPeriSrcPclk:
                u32Freq = stcClkFreq.pclk1Freq;
                break;
            case ClkPeriSrcMpllp:
                u32Freq = stcPllClkFreq.mpllp;
                break;
            case ClkPeriSrcMpllq:
                u32Freq = stcPllClkFreq.mpllq;
                break;
            case ClkPeriSrcMpllr:
                u32Freq = stcPllClkFreq.mpllr;
                break;
            case ClkPeriSrcUpllp:
                u32Freq = stcPllClkFreq.upllp;
                break;
            case ClkPeriSrcUpllq:
                u32Freq = stcPllClkFreq.upllq;
                break;
            case ClkPeriSrcUpllr:
                u32Freq = stcPllClkFreq.upllr;
                break;
            default:
                break;
        }
    }
    return u32Freq;
}



//void i2s_tx_dma_tc2_irq_cb(void)
//{
//    DMA_ClearIrqFlag(I2S2_DMA_UNIT_RECORD, I2S2_DMA_CH_RECORD, TrnCpltIrq);
//	DMA_SetDesAddress (I2S2_DMA_UNIT_RECORD, I2S2_DMA_CH_RECORD, (uint32_t)&au32RecorderSoundI2s[0]);
//	DMA_SetTransferCnt(I2S2_DMA_UNIT_RECORD, I2S2_DMA_CH_RECORD, 1024);
//	DMA_ChannelCmd(I2S2_DMA_UNIT_RECORD, I2S2_DMA_CH_RECORD, Enable);
//}

//void i2s_dma_config(void)
//{
//    stc_dma_config_t stcDmaCfg;
//    stc_irq_regi_conf_t stcIrqRegiConf;

//    MEM_ZERO_STRUCT(stcDmaCfg);
//	MEM_ZERO_STRUCT(stcIrqRegiConf);

//	PWC_Fcg0PeriphClockCmd(PWC_FCG0_PERIPH_PTDIS, Enable);
//    PWC_Fcg0PeriphClockCmd(I2S2_DMA_RECORD_FCG, Enable);
//    
//    stcDmaCfg.u16BlockSize           	= 1;
//    stcDmaCfg.u16TransferCnt         	= 1024;
//    stcDmaCfg.u32SrcAddr             	= (uint32_t)(&I2S_CH->RXBUF);
//    stcDmaCfg.u32DesAddr             	= (uint32_t)(&au32RecorderSoundI2s[0]);
//	
//	stcDmaCfg.stcDmaChCfg.enSrcRptEn  	= Disable;
//    stcDmaCfg.stcDmaChCfg.enDesRptEn  	= Enable;
//    stcDmaCfg.stcDmaChCfg.enSrcNseqEn 	= Disable;
//    stcDmaCfg.stcDmaChCfg.enDesNseqEn 	= Disable;
//	
//	stcDmaCfg.u32DmaLlp      			= 0u;
//    stcDmaCfg.stcSrcNseqCfg.u16Cnt    	= 0u;
//    stcDmaCfg.stcSrcNseqCfg.u32Offset 	= 0u;
//    stcDmaCfg.stcDesNseqCfg.u16Cnt    	= 0u;
//    stcDmaCfg.stcDesNseqCfg.u32Offset 	= 0u;
//	
//	stcDmaCfg.stcDmaChCfg.enLlpEn 	 	= Disable;
//    stcDmaCfg.stcDmaChCfg.enSrcInc   	= AddressFix;
//    stcDmaCfg.stcDmaChCfg.enDesInc   	= AddressIncrease ;
//    stcDmaCfg.stcDmaChCfg.enTrnWidth 	= Dma32Bit;
//    stcDmaCfg.stcDmaChCfg.enIntEn    	= Enable;
//    DMA_InitChannel(I2S2_DMA_UNIT_RECORD, I2S2_DMA_CH_RECORD, &stcDmaCfg);
//    DMA_SetTriggerSrc(I2S2_DMA_UNIT_RECORD, I2S2_DMA_CH_RECORD, I2S2_DMA_RX_TRIG_SOURCE);


//    DMA_Cmd(I2S2_DMA_UNIT_RECORD, Enable);
//	DMA_ClearIrqFlag(I2S2_DMA_UNIT_RECORD, I2S2_DMA_CH_RECORD, TrnCpltIrq);
//	
//	DMA_ChannelCmd(I2S2_DMA_UNIT_RECORD, I2S2_DMA_CH_RECORD, Enable);
//	
//    stcIrqRegiConf.enIRQn      = Int010_IRQn;
//    stcIrqRegiConf.pfnCallback = &i2s_tx_dma_tc2_irq_cb;
//    stcIrqRegiConf.enIntSrc    = I2S2_DMA_RECORD_IRQ_SRC;
//    enIrqRegistration(&stcIrqRegiConf);
//    
//    NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
//    NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);	
//    NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
//}
	
void i2s_init(void)
{
	stc_port_init_t stcPortIni;
	stc_i2s_config_t stcI2sCfg;
	stc_irq_regi_conf_t stcIrqRegiConf;
	
    MEM_ZERO_STRUCT(stcI2sCfg);
	MEM_ZERO_STRUCT(stcPortIni);
	MEM_ZERO_STRUCT(stcIrqRegiConf);
	

    PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_I2S2, Enable);

	// I2S 引脚初始化
    stcPortIni.enPullUp = Disable;
    stcPortIni.enPinDrv = Pin_Drv_L;
    PORT_Init(I2S2_CK_PORT, I2S2_CK_PIN, &stcPortIni);
    PORT_Init(I2S2_WS_PORT, I2S2_WS_PIN, &stcPortIni);
    PORT_Init(I2S2_SD_PORT, I2S2_SD_PIN, &stcPortIni);
	PORT_Init(I2S2_SDIN_PORT, I2S2_SDIN_PIN, &stcPortIni);
    PORT_SetFunc(I2S2_CK_PORT, I2S2_CK_PIN, Func_I2s2_Ck, Disable);
    PORT_SetFunc(I2S2_WS_PORT, I2S2_WS_PIN, Func_I2s2_Ws, Disable);
    PORT_SetFunc(I2S2_SD_PORT, I2S2_SD_PIN, Func_I2s2_Sd, Disable);
    PORT_SetFunc(I2S2_SDIN_PORT, I2S2_SDIN_PIN, Func_I2s2_Sdin, Disable);
	
//#ifdef EXCK_ON
////    stcPortIni.enPullUp = Enable;
////    stcPortIni.enPinDrv = Pin_Drv_H;
//    PORT_Init(I2S2_EXCK_PORT, I2S2_EXCK_PIN, &stcPortIni);
//    PORT_SetFunc(I2S2_EXCK_PORT, I2S2_EXCK_PIN, Func_I2s2_Sdin, Disable);
//#else
////    PORT_Init(I2S2_MCK_PORT, I2S2_MCK_PIN, &stcPortIni); //当前从机未使用
////    PORT_SetFunc(I2S2_MCK_PORT, I2S2_MCK_PIN, Func_I2s2_Sdin, Disable);
//#endif

	// I2S 初始化
	CLK_SetI2sClkSource(I2S_CH, ClkPeriSrcMpllp);
    I2s_DeInit(I2S_CH);
    //stcI2sCfg.u32I2sInterClkFreq = GetI2SClkFreq(I2S_CH);
    stcI2sCfg.enStandrad = Std_Philips;
    stcI2sCfg.enMode = I2sSlave;
    stcI2sCfg.enFullDuplexMode = I2s_FullDuplex;
    stcI2sCfg.enChanelLen = I2s_ChLen_32Bit;
    stcI2sCfg.enDataBits = I2s_DataLen_24Bit;
    stcI2sCfg.u32AudioFreq = I2S_AudioFreq_48k;
	
//#ifdef EXCK_ON
////    stcI2sCfg.enMcoOutEn = Disable;
////    stcI2sCfg.enExckEn = Enable;
//#else	
//    stcI2sCfg.enMcoOutEn = Enable;
//    stcI2sCfg.enExckEn = Disable;
//#endif

    I2s_Init(I2S_CH, &stcI2sCfg);
	
	
	stcIrqRegiConf.enIRQn = Int006_IRQn;
    stcIrqRegiConf.enIntSrc = I2S_RXI_IRQ;
    stcIrqRegiConf.pfnCallback = &I2SRx_CallBack;
	
	enIrqRegistration(&stcIrqRegiConf);
	
	NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
	NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_05);
	NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
	
	
	stcIrqRegiConf.enIRQn = Int007_IRQn;
    stcIrqRegiConf.enIntSrc = INT_I2S2_TXIRQOUT;
    stcIrqRegiConf.pfnCallback = &I2STx_CallBack;
	
	enIrqRegistration(&stcIrqRegiConf);
	
	NVIC_ClearPendingIRQ(stcIrqRegiConf.enIRQn);
	NVIC_SetPriority(stcIrqRegiConf.enIRQn, DDL_IRQ_PRIORITY_05);
	NVIC_EnableIRQ(stcIrqRegiConf.enIRQn);
	
	//i2s_dma_config();
	
#ifdef USE_RT_THREAD
	i2s2_mq = rt_mq_create("i2s2_mq", 40, 20, RT_IPC_FLAG_FIFO);	
#endif
		
}

void i2s_start(void)
{
	
	while(PORT_GetBit(I2S2_WS_PORT, I2S2_WS_PIN) == Reset)
	{
		;
	}
	I2S_FuncCmd(I2S_CH, RxEn, Enable);
	I2S_FuncCmd(I2S_CH, RxIntEn, Enable);
	I2S_FuncCmd(I2S_CH, TxEn, Enable);
	I2S_FuncCmd(I2S_CH, TxIntEn, Enable);
}

void i2s_stop(void)
{
	I2S_FuncCmd(I2S_CH, RxEn, Disable);
	I2S_FuncCmd(I2S_CH, RxIntEn, Disable);
	I2S_FuncCmd(I2S_CH, TxEn, Disable);
	I2S_FuncCmd(I2S_CH, TxIntEn, Disable);
}


void i2s_ReadBuffer(int32_t *buffer)
{
#ifdef USE_RT_THREAD
	int32_t i2s2_mq_flag = 0;
	rt_mq_recv(i2s2_mq, &i2s2_mq_flag, sizeof(int32_t), RT_WAITING_FOREVER);
#else
	while(i2s_data_full_flag == 0)
	{
		//rt_thread_mdelay(1);
	}
#endif
	i2s_data_full_flag = 0;
	memcpy(buffer, i2s_data_in_buffer, sizeof(int32_t) * I2S2_BUFFER_SIZE);
}

void I2SRx_CallBack(void)
{	
	i2s_data_in = I2S_RevData(I2S_CH);
	
	i2s_data_in_buffer[i2s_data_count++] = (i2s_data_in << 8u);

	if(i2s_data_count >= I2S2_BUFFER_SIZE)
	{
		i2s_data_count = 0;
		i2s_data_full_flag = 1;
#ifdef USE_RT_THREAD
		rt_mq_send(i2s2_mq, &i2s_data_full_flag, sizeof(int32_t));
#endif		
	}
}

void I2STx_CallBack(void)
{
	I2S_SendData(I2S_CH, (uint32_t)(i2s_data_in));
}	
	
	





