/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_i2s_adc.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ÔëÉù´«¸ÐÆ÷
*******************************************************************************/
#ifndef __CRY_I2S_ADC_H__
#define __CRY_I2S_ADC_H__


#include "hc32_ddl.h"


//#define EXCK_ON

#define I2S_CH                      (M4_I2S2)
#define I2S_RXI_IRQ                 (INT_I2S2_RXIRQOUT)

#define I2S2_WS_PORT                (PortC)
#define I2S2_WS_PIN                 (Pin04)
#define I2S2_SD_PORT                (PortC)
#define I2S2_SD_PIN                 (Pin02)
#define I2S2_CK_PORT                (PortC)
#define I2S2_CK_PIN                 (Pin05)
#define I2S2_SDIN_PORT              (PortC)
#define I2S2_SDIN_PIN               (Pin03)


//#ifdef EXCK_ON
//#define I2S2_EXCK_PORT              (PortC)
//#define I2S2_EXCK_PIN               (Pin02)
//#else
//#define I2S2_MCK_PORT               (PortC)
//#define I2S2_MCK_PIN                (Pin08)
//#endif

/* DMA for Play */
//#define DMA_UNIT_PLAY               (M4_DMA2)
//#define DMA_CH_PLAY                 (DmaCh0)
//#define DMA_PLAY_FCG                (PWC_FCG0_PERIPH_DMA2)
//#define DMA_PLAY_IRQ_SRC            (INT_DMA2_TC0)

/* DMA for Record */
#define I2S2_DMA_UNIT_RECORD             (M4_DMA1)
#define I2S2_DMA_CH_RECORD               (DmaCh2)
#define I2S2_DMA_RECORD_FCG              (PWC_FCG0_PERIPH_DMA1)
#define I2S2_DMA_RX_TRIG_SOURCE  		 (EVT_I2S2_RXIRQOUT)
#define I2S2_DMA_RECORD_IRQ_SRC          (INT_DMA1_TC2)

#define I2S2_BUFFER_SIZE			(1024)

#define PACKET                      (96U)
#define NUM                         (8U)
#define TOTAL_IN_BUF_SIZE           (PACKET*NUM)

#define IDLE_STATE                  (0U)
#define RECORD_STATE                (1U)
#define ALL_RUN_STATE               (2U)
#define STOP_STATE                  (3U)


void i2s_init(void);
void i2s_ReadBuffer(int32_t *buffer);
void i2s_start(void);
void i2s_stop(void);

#endif

