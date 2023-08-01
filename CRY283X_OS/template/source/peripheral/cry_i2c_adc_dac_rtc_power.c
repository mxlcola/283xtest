/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_i2c_adc_dac_rtc_power.c
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x 噪声传感器
*******************************************************************************/
#include "cry_i2c_adc_dac_rtc_power.h"


en_result_t I2C_Mem_Write(uint16_t u8DevAddr, uint16_t u8MemAddr, uint8_t *pu8Data, uint32_t u32Size, uint32_t u32TimeOut)
{
    en_result_t enRet;
    uint16_t u16MemAddrTemp;
	u16MemAddrTemp = u8MemAddr & 0xFFul;

    I2C_Cmd(I2C_UNIT, Enable);

    I2C_SoftwareResetCmd(I2C_UNIT, Enable);
    I2C_SoftwareResetCmd(I2C_UNIT, Disable);
    enRet = I2C_Start(I2C_UNIT,u32TimeOut);
    if(Ok == enRet)
    {
        enRet = I2C_TransAddr(I2C_UNIT, u8DevAddr, I2CDirTrans, u32TimeOut);

        if(Ok == enRet)
        {
            enRet = I2C_TransData(I2C_UNIT, (uint8_t *)&u16MemAddrTemp, E2_MEM_ADR_LEN, u32TimeOut);
            if(Ok == enRet)
            {
                enRet = I2C_TransData(I2C_UNIT, pu8Data, u32Size, u32TimeOut);
            }
        }
    }

    I2C_Stop(I2C_UNIT,u32TimeOut);
    I2C_Cmd(I2C_UNIT, Disable);
	
    return enRet;
}

en_result_t I2C_Mem_Read(uint16_t u8DevAddr, uint16_t u8MemAddr, uint8_t *pu8Data, uint32_t u32Size, uint32_t u32TimeOut)
{
    en_result_t enRet;
    uint16_t u16MemAddrTemp;
	u16MemAddrTemp = u8MemAddr & 0xFFul;
	
	
    I2C_Cmd(I2C_UNIT, Enable);

	I2C_SoftwareResetCmd(I2C_UNIT, Enable);
    I2C_SoftwareResetCmd(I2C_UNIT, Disable);
	enRet = I2C_Start(I2C_UNIT,u32TimeOut);
    if(Ok == enRet)
    {
        enRet = I2C_TransAddr(I2C_UNIT, u8DevAddr, I2CDirTrans, u32TimeOut);

        if(Ok == enRet)
        {
            enRet = I2C_TransData(I2C_UNIT, (uint8_t *)&u16MemAddrTemp, E2_MEM_ADR_LEN, u32TimeOut);
        }
    }	
	I2C_Stop(I2C_UNIT,u32TimeOut);
	
	
    enRet = I2C_Start(I2C_UNIT,u32TimeOut);
    if(Ok == enRet)
    {
        enRet = I2C_TransAddr(I2C_UNIT, u8DevAddr, I2CDirReceive, u32TimeOut);

        if(Ok == enRet)
        {
			enRet = I2C_ReceiveData(I2C_UNIT, pu8Data, u32Size, u32TimeOut);
        }
    }

    I2C_Stop(I2C_UNIT,u32TimeOut);
    I2C_Cmd(I2C_UNIT, Disable);
	
    return enRet;
}


uint8_t byte2BCD(uint8_t data) 
{
    return ((data / 10) << 4) + data % 10;
}

uint8_t BCD2Byte(uint8_t data) 
{
    return (data >> 4) * 10 + (data & 0x0f);
}


void i2c_init(void)
{
	stc_i2c_init_t stcI2cInit;
    float32_t fErr;
	
	
	MEM_ZERO_STRUCT(stcI2cInit);
	
	
	// I2C引脚使能 
	PORT_SetFunc(I2C_SCL_PORT, I2C_SCL_PIN, I2C_GPIO_SCL_FUNC, Disable);
	PORT_SetFunc(I2C_SDA_PORT, I2C_SDA_PIN, I2C_GPIO_SDA_FUNC, Disable);
	PWC_Fcg1PeriphClockCmd(I2C_FCG_USE, Enable);
	

	// I2C初始化
    I2C_DeInit(I2C_UNIT);
    stcI2cInit.u32ClockDiv = I2C_CLK_DIV8;
    stcI2cInit.u32Baudrate = 400000ul;
    stcI2cInit.u32SclTime = 0ul;
    I2C_Init(I2C_UNIT, &stcI2cInit, &fErr);

    I2C_BusWaitCmd(I2C_UNIT, Enable);

	
	Ddl_Delay1ms(5ul);
	
	ES7243E_adc_init();
	
	Ddl_Delay1ms(5ul);
	
	ES8156_dac_init();
	
	Ddl_Delay1ms(5ul);
	
	BMP8563_init();
	
	Ddl_Delay1ms(5ul);
	
	SGM415_power_init();
	
	Ddl_Delay1ms(5ul);
	
	// ES8156_dac_Standby_0uA();
}



void I2CWRNBYTE_CODEC_ADC(uint16_t WriteAddr,uint8_t DataToWrite)
{	
	I2C_Mem_Write(0x13, WriteAddr, &DataToWrite, 1u, I2C_TIMEOUT);	
}

void I2CWRNBYTE_CODEC_DAC(uint16_t WriteAddr,uint8_t DataToWrite)
{	
	I2C_Mem_Write(0x08, WriteAddr, &DataToWrite, 1u, I2C_TIMEOUT);	
}

void I2CWRNBYTE_CODEC_RTC(uint16_t WriteAddr,uint8_t DataToWrite)
{	
	I2C_Mem_Write(0x51, WriteAddr, &DataToWrite, 1u, I2C_TIMEOUT);	
}

void I2CWRNBUFFER_CODEC_RTC(uint16_t WriteAddr,uint8_t *DataToWrite, uint32_t u32Size)
{	
	I2C_Mem_Write(0x51, WriteAddr, DataToWrite, u32Size, I2C_TIMEOUT);	
}

void I2CWRDBUFFER_CODEC_RTC(uint16_t ReadAddr,uint8_t *DataToRecv, uint32_t u32Size)
{	
	I2C_Mem_Read(0x51, ReadAddr, DataToRecv, u32Size, I2C_TIMEOUT);	
}

void I2CWRNBYTE_CODEC_POWER(uint16_t WriteAddr,uint8_t DataToWrite)
{	
	I2C_Mem_Write(0x1A, WriteAddr, &DataToWrite, 1u, I2C_TIMEOUT);	
}

void I2CWRDBYTE_CODEC_POWER(uint16_t WriteAddr,uint8_t *DataToRecv)
{	
	I2C_Mem_Read(0x1A, WriteAddr, DataToRecv, 1u, I2C_TIMEOUT);	
}

void I2CUPBYTE_CODEC_POWER(uint16_t UpdateAddr,uint8_t Mask, uint8_t DataToWrite)
{	
	uint8_t tmp;
	I2C_Mem_Read(0x1A, UpdateAddr, &tmp, 1u, I2C_TIMEOUT);	
	
	tmp &= ~Mask;
	tmp |= DataToWrite & Mask;
	
	I2C_Mem_Write(0x1A, UpdateAddr, &tmp, 1u, I2C_TIMEOUT);	
}

void ES7243E_adc_init(void)
{
	I2CWRNBYTE_CODEC_ADC(0x01,0x3A);	  
	I2CWRNBYTE_CODEC_ADC(0x00,0x80);	
	I2CWRNBYTE_CODEC_ADC(0xF9,0x00);  
	I2CWRNBYTE_CODEC_ADC(0x04,0x02);	  
	I2CWRNBYTE_CODEC_ADC(0x04,0x01);	  
	I2CWRNBYTE_CODEC_ADC(0xF9,0x01);	  
	I2CWRNBYTE_CODEC_ADC(0x00,0x1E);	  
	I2CWRNBYTE_CODEC_ADC(0x01,0x00);	 
	if(Ratio == 64)//Ratio=MCLK/LRCK=64;3M072/48K;1M028/16K;512K/8K
	{  
		I2CWRNBYTE_CODEC_ADC(0x03,0x20);
		I2CWRNBYTE_CODEC_ADC(0x04,0x03);				  
		I2CWRNBYTE_CODEC_ADC(0x0D,Dmic_GAIN);	  
		I2CWRNBYTE_CODEC_ADC(0xF9,0x00);	  	   
		I2CWRNBYTE_CODEC_ADC(0x05,0x00);
		I2CWRNBYTE_CODEC_ADC(0x06,SCLK_DIV - 1);   
		I2CWRNBYTE_CODEC_ADC(0x07,0x00);//LRCK=MCLK/64 
		I2CWRNBYTE_CODEC_ADC(0x08,0x3F);//LRCK=MCLK/64 
	}	
	if(Ratio == 256)//Ratio=MCLK/LRCK=256;12M288/48K;4M096/16K; 2M048/8K
	{ 	  
		I2CWRNBYTE_CODEC_ADC(0x03,0x20);	  
		I2CWRNBYTE_CODEC_ADC(0x04,0x01);	
		I2CWRNBYTE_CODEC_ADC(0x0D,Dmic_GAIN);	   
		I2CWRNBYTE_CODEC_ADC(0x05,0x00);	  
		I2CWRNBYTE_CODEC_ADC(0x06,SCLK_DIV - 1);   
		I2CWRNBYTE_CODEC_ADC(0x07,0x00);//LRCK=MCLK/256  
		I2CWRNBYTE_CODEC_ADC(0x08,0xFF);//LRCK=MCLK/256  
	}
	if(Ratio == 384)//Ratio=MCLK/LRCK=384;12M288/32K;6M144/16K;3M072/8K
	{ 	  
		I2CWRNBYTE_CODEC_ADC(0x03,0x20);	  
		I2CWRNBYTE_CODEC_ADC(0x0D,Dmic_GAIN);	  
		I2CWRNBYTE_CODEC_ADC(0xF9,0x00);	  
		I2CWRNBYTE_CODEC_ADC(0x04,0x22);//384/3*4=512	  
		I2CWRNBYTE_CODEC_ADC(0x05,0x00);	  
		I2CWRNBYTE_CODEC_ADC(0x06,SCLK_DIV - 1);    
		I2CWRNBYTE_CODEC_ADC(0x07,0x01);//LRCK=MCLK/384  
		I2CWRNBYTE_CODEC_ADC(0x08,0x7F);//LRCK=MCLK/384  
	}	
	if(Ratio == 400)//Ratio=MCLK/LRCK=400;19M2/96K?
	{  
		I2CWRNBYTE_CODEC_ADC(0x03,0x19);//D25	  
		I2CWRNBYTE_CODEC_ADC(0x0D,Dmic_GAIN);	   
		I2CWRNBYTE_CODEC_ADC(0x04,0x00);//
		I2CWRNBYTE_CODEC_ADC(0x05,0x00);	  
		I2CWRNBYTE_CODEC_ADC(0x06,SCLK_DIV - 1);    
		I2CWRNBYTE_CODEC_ADC(0x07,0x01);//LRCK=MCLK/400  
		I2CWRNBYTE_CODEC_ADC(0x08,0x8F);//LRCK=MCLK/400   
	}
	if(Ratio == 512)//Ratio=MCLK/LRCK=512;12M288/24K;6M144/12K;4M096/8K;
	{  
		I2CWRNBYTE_CODEC_ADC(0x03,0x20);	  
		I2CWRNBYTE_CODEC_ADC(0x0D,Dmic_GAIN);	   
		I2CWRNBYTE_CODEC_ADC(0x04,0x00);//512=512	  
		I2CWRNBYTE_CODEC_ADC(0x05,0x00);	  
		I2CWRNBYTE_CODEC_ADC(0x06,SCLK_DIV - 1);    
		I2CWRNBYTE_CODEC_ADC(0x07,0x01);//LRCK=MCLK/512  
		I2CWRNBYTE_CODEC_ADC(0x08,0xFF);//LRCK=MCLK/512   
	}	
	if(Ratio == 768)//Ratio=MCLK/LRCK=768;12M288/16K;6M144/8K;
	{   
		I2CWRNBYTE_CODEC_ADC(0x03,0x20);	  
		I2CWRNBYTE_CODEC_ADC(0x0D,Dmic_GAIN);	  
		I2CWRNBYTE_CODEC_ADC(0x04,0x21);//768/3*2=512	  
		I2CWRNBYTE_CODEC_ADC(0x05,0x00);	  
		I2CWRNBYTE_CODEC_ADC(0x06,SCLK_DIV - 1);   
		I2CWRNBYTE_CODEC_ADC(0x07,0x02);//LRCK=MCLK/768  
		I2CWRNBYTE_CODEC_ADC(0x08,0xFF);//LRCK=MCLK/768  
	}
	if(Ratio == 800)//Ratio=MCLK/LRCK=800;19M2/48K
	{  
		I2CWRNBYTE_CODEC_ADC(0x03,0x19);	  
		I2CWRNBYTE_CODEC_ADC(0x0D,Dmic_GAIN);	   
		I2CWRNBYTE_CODEC_ADC(0x04,0x10);//
		I2CWRNBYTE_CODEC_ADC(0x05,0x00);	  
		I2CWRNBYTE_CODEC_ADC(0x06,SCLK_DIV - 1);    
		I2CWRNBYTE_CODEC_ADC(0x07,0x03);//LRCK=MCLK/800  
		I2CWRNBYTE_CODEC_ADC(0x08,0x1F);//LRCK=MCLK/800   
	}
	if(Ratio == 1024)//Ratio=MCLK/LRCK=512;12M288/12K;
	{ 	  
		I2CWRNBYTE_CODEC_ADC(0x03,0x20);	  
		I2CWRNBYTE_CODEC_ADC(0x0D,Dmic_GAIN);	    
		I2CWRNBYTE_CODEC_ADC(0x04,0x10);//1024/2=512	  
		I2CWRNBYTE_CODEC_ADC(0x05,0x00);	  
		I2CWRNBYTE_CODEC_ADC(0x06,SCLK_DIV - 1);    
		I2CWRNBYTE_CODEC_ADC(0x07,0x03);//LRCK=MCLK/1024  
		I2CWRNBYTE_CODEC_ADC(0x08,0xFF);//LRCK=MCLK/1024  
	}	
	if(Ratio == 1536)//Ratio=MCLK/LRCK=1536;12M288/8K;
	{  
		I2CWRNBYTE_CODEC_ADC(0x03,0x20);	  
		I2CWRNBYTE_CODEC_ADC(0x0D,Dmic_GAIN);	    
		I2CWRNBYTE_CODEC_ADC(0x04,0x20);//1536/3=512	  
		I2CWRNBYTE_CODEC_ADC(0x05,0x00);	  
		I2CWRNBYTE_CODEC_ADC(0x06,SCLK_DIV - 1);    
		I2CWRNBYTE_CODEC_ADC(0x07,0x05);//LRCK=MCLK/1536  
		I2CWRNBYTE_CODEC_ADC(0x08,0xFF);//LRCK=MCLK/1536   
	}

	I2CWRNBYTE_CODEC_ADC(0x02,(MCLK_SOURCE<<7) + SCLK_INV);

	I2CWRNBYTE_CODEC_ADC(0x09,0xCA);	  
	I2CWRNBYTE_CODEC_ADC(0x0A,0x85);	  
	I2CWRNBYTE_CODEC_ADC(0x0B,0xC0 + Format + (Format_Len<<2));	  
	I2CWRNBYTE_CODEC_ADC(0x0E,ADC_Volume);	  

	I2CWRNBYTE_CODEC_ADC(0x10,0x38);	  
	I2CWRNBYTE_CODEC_ADC(0x11,0x16);	
	I2CWRNBYTE_CODEC_ADC(0x14,0x0C);	  
	I2CWRNBYTE_CODEC_ADC(0x15,0x0C);	  
	I2CWRNBYTE_CODEC_ADC(0x17,0x02);	  
	I2CWRNBYTE_CODEC_ADC(0x18,0x26);
	if(Device == ES7243E)
	{
		I2CWRNBYTE_CODEC_ADC(0x0F,0x80);  
		I2CWRNBYTE_CODEC_ADC(0x19,0x77);
	}
	if(Device == ES7243L)
	{
		I2CWRNBYTE_CODEC_ADC(0x0F,0xA0);  
		I2CWRNBYTE_CODEC_ADC(0x19,0xAA);
	}

	I2CWRNBYTE_CODEC_ADC(0x1A,0xF4);	  
	I2CWRNBYTE_CODEC_ADC(0x1B,0x66);	  
	I2CWRNBYTE_CODEC_ADC(0x1C,0x44);	  
	I2CWRNBYTE_CODEC_ADC(0x1E,0x00); 

	I2CWRNBYTE_CODEC_ADC(0x1F,0x08 + (Dmic_Selon<<5) - VDDA_VOLTAGE);	
	  
	I2CWRNBYTE_CODEC_ADC(0x20,0x10 + ADC_PGA_GAIN);//PGA 模拟增益:	  
	I2CWRNBYTE_CODEC_ADC(0x21,0x10 + ADC_PGA_GAIN);//PGA 模拟增益: 

	I2CWRNBYTE_CODEC_ADC(0x00,0x80 + (MSMode_MasterSelOn<<6));//Slave  Mode	
	
	I2CWRNBYTE_CODEC_ADC(0x01,0x3A);	  
	I2CWRNBYTE_CODEC_ADC(0x16,0x3F);	  
	I2CWRNBYTE_CODEC_ADC(0x16,0x00);	  
	Ddl_Delay1ms(5ul);
	I2CWRNBYTE_CODEC_ADC(0x0B,Format + (Format_Len<<2));  	
}

void ES7243L_adc_Standby(void)//待机配置(0UA)
{
	I2CWRNBYTE_CODEC_ADC(0x04,0x02);	  
	I2CWRNBYTE_CODEC_ADC(0x04,0x01);	  
	I2CWRNBYTE_CODEC_ADC(0xF7,0x30);	  
	I2CWRNBYTE_CODEC_ADC(0xF9,0x01);	  
	I2CWRNBYTE_CODEC_ADC(0x16,0xFF);	  
	I2CWRNBYTE_CODEC_ADC(0x17,0x00);	  
	I2CWRNBYTE_CODEC_ADC(0x01,0x38);	  
	I2CWRNBYTE_CODEC_ADC(0x20,0x00);	  
	I2CWRNBYTE_CODEC_ADC(0x21,0x00);	  
	I2CWRNBYTE_CODEC_ADC(0x00,0x00);	  
	I2CWRNBYTE_CODEC_ADC(0x00,0x1E);	  
	I2CWRNBYTE_CODEC_ADC(0x01,0x30);	  
	I2CWRNBYTE_CODEC_ADC(0x01,0x00);	  
} 

void ES7243L_adc_Resume(void)//恢复配置(未下电)
{
	I2CWRNBYTE_CODEC_ADC(0x0B,0xc0 + Format + (Format_Len<<2));
	I2CWRNBYTE_CODEC_ADC(0xF9,0x00);	
	I2CWRNBYTE_CODEC_ADC(0x04,0x01);		
	I2CWRNBYTE_CODEC_ADC(0x17,0x01);		
	I2CWRNBYTE_CODEC_ADC(0x20,0x10);	  
	I2CWRNBYTE_CODEC_ADC(0x21,0x10);	
	I2CWRNBYTE_CODEC_ADC(0x00,0x80);		
	I2CWRNBYTE_CODEC_ADC(0x01,0x3A);		
	I2CWRNBYTE_CODEC_ADC(0x16,0x3F);
	I2CWRNBYTE_CODEC_ADC(0x16,0x00);	
	Ddl_Delay1ms(5ul);
	I2CWRNBYTE_CODEC_ADC(0x0B,Format + (Format_Len<<2));
}

void ES8156_dac_init(void)
{
	I2CWRNBYTE_CODEC_DAC(0x02,0x04 + (SCLK_INV_DAC<<4) + (MCLK_SOURCE_DAC<<7) + MSMode_MasterSelOn_DAC);	
	
	if(DACHPModeOn_DAC == 0)//接PA或者LOUT,内部不开启负载驱动,省功耗
	{
		I2CWRNBYTE_CODEC_DAC(0x20,0x2A);	
		I2CWRNBYTE_CODEC_DAC(0x21,0x3C);	
		I2CWRNBYTE_CODEC_DAC(0x22,0x00);	
		I2CWRNBYTE_CODEC_DAC(0x24,0x07);	
		I2CWRNBYTE_CODEC_DAC(0x23,0x40 + (0x30*VDDA_VOLTAGE_DAC));	
	}
	if(DACHPModeOn_DAC == 1)//接耳机,内部开启负载驱动,支持耳机输出
	{
		I2CWRNBYTE_CODEC_DAC(0x20,0x16);	
		I2CWRNBYTE_CODEC_DAC(0x21,0x7F);	
		I2CWRNBYTE_CODEC_DAC(0x22,0x08);	
		I2CWRNBYTE_CODEC_DAC(0x24,0x01);	
		I2CWRNBYTE_CODEC_DAC(0x23,0xCA + (0x30*VDDA_VOLTAGE_DAC));	
	}
	I2CWRNBYTE_CODEC_DAC(0x0A,0x01);	
	I2CWRNBYTE_CODEC_DAC(0x0B,0x01);	
	I2CWRNBYTE_CODEC_DAC(0x11,NORMAL_I2S_DAC + (Format_Len_DAC<<4));
	I2CWRNBYTE_CODEC_DAC(0x14,DAC_Volume_DAC);	
	if(Ratio_DAC == 1536)//Ratio=MCLK/LRCK=1536：12M288/8K；24M576/16K
	{
		I2CWRNBYTE_CODEC_DAC(0x01,0x26);//1536 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x09,0x00);//1536 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x03,0x06);//LRCK H		
		I2CWRNBYTE_CODEC_DAC(0x04,0x00);//LRCK=MCLK/1536
		I2CWRNBYTE_CODEC_DAC(0x05,SCLK_DIV_DAC);//BCLK=MCLK/4									
	}
	if(Ratio_DAC == 1024)//Ratio=MCLK/LRCK=1024：12M288/12K；24M576/24K
	{
		I2CWRNBYTE_CODEC_DAC(0x01,0x24);//256 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x09,0x00);//256 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x03,0x04);//LRCK H		
		I2CWRNBYTE_CODEC_DAC(0x04,0x00);//LRCK=MCLK/256
		I2CWRNBYTE_CODEC_DAC(0x05,SCLK_DIV_DAC);//BCLK=MCLK/4									
	}
	if(Ratio_DAC == 768)//Ratio=MCLK/LRCK=768：12M288/16K；24M576/32K；
	{
		I2CWRNBYTE_CODEC_DAC(0x01,0x23);//768 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x09,0x00);//768 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x03,0x03);//LRCK H		
		I2CWRNBYTE_CODEC_DAC(0x04,0x00);//LRCK=MCLK/768
		I2CWRNBYTE_CODEC_DAC(0x05,SCLK_DIV_DAC);//BCLK=MCLK/4									
	}
	if(Ratio_DAC == 512)//Ratio=MCLK/LRCK=512：12M288/24K；24M576/48K；
	{
		I2CWRNBYTE_CODEC_DAC(0x01,0x22);//512 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x09,0x00);//512 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x03,0x02);//LRCK H		
		I2CWRNBYTE_CODEC_DAC(0x04,0x00);//LRCK=MCLK/512
		I2CWRNBYTE_CODEC_DAC(0x05,SCLK_DIV_DAC);//BCLK=MCLK/4									
	}
	if(Ratio_DAC == 384)//Ratio=MCLK/LRCK=384：12M288/32K；6M144/16K
	{
		I2CWRNBYTE_CODEC_DAC(0x01,0x63);//384 Ratio(MCLK/LRCK)
		I2CWRNBYTE_CODEC_DAC(0x09,0x00);//384 Ratio(MCLK/LRCK)					
		I2CWRNBYTE_CODEC_DAC(0x03,0x01);//LRCK H		
		I2CWRNBYTE_CODEC_DAC(0x04,0x80);//LRCK=MCLK/384
		I2CWRNBYTE_CODEC_DAC(0x05,SCLK_DIV_DAC);//BCLK=MCLK/4		
	}
	if(Ratio_DAC == 256)//Ratio=MCLK/LRCK=256：12M288/48K；
	{
		I2CWRNBYTE_CODEC_DAC(0x01,0x21);//256 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x09,0x00);//256 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x03,0x01);//LRCK H		
		I2CWRNBYTE_CODEC_DAC(0x04,0x00);//LRCK=MCLK/256
		I2CWRNBYTE_CODEC_DAC(0x05,SCLK_DIV_DAC);//BCLK=MCLK/4									
	}
	if(Ratio_DAC == 128)//Ratio=MCLK/LRCK=128：6M144/48K；
	{
		I2CWRNBYTE_CODEC_DAC(0x01,0x61);//128 Ratio(MCLK/LRCK)
		I2CWRNBYTE_CODEC_DAC(0x09,0x00);//128 Ratio(MCLK/LRCK)					
		I2CWRNBYTE_CODEC_DAC(0x03,0x00);//LRCK H		
		I2CWRNBYTE_CODEC_DAC(0x04,0x80);//LRCK=MCLK/128
		I2CWRNBYTE_CODEC_DAC(0x05,SCLK_DIV_DAC);//BCLK=MCLK/4		
	}
	if(Ratio_DAC == 64)//Ratio=MCLK/LRCK=64：3M072/48K；
	{
		I2CWRNBYTE_CODEC_DAC(0x01,0xA1);//64 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x09,0x02);//64 Ratio(MCLK/LRCK)	
		I2CWRNBYTE_CODEC_DAC(0x03,0x00);//LRCK H		
		I2CWRNBYTE_CODEC_DAC(0x04,0x40);//LRCK=MCLK/64
		I2CWRNBYTE_CODEC_DAC(0x05,SCLK_DIV_DAC);//BCLK=MCLK/2
	}

	I2CWRNBYTE_CODEC_DAC(0x0D,0x14);	
	I2CWRNBYTE_CODEC_DAC(0x18,0x00);	
	I2CWRNBYTE_CODEC_DAC(0x08,0x3F);	
	I2CWRNBYTE_CODEC_DAC(0x00,0x02);	
	I2CWRNBYTE_CODEC_DAC(0x00,0x03);	
	I2CWRNBYTE_CODEC_DAC(0x25,0x20);	
	
}   

void ES8156_dac_Standby_0uA(void)//待机
{
	I2CWRNBYTE_CODEC_DAC(0x14,0x00);	  
	I2CWRNBYTE_CODEC_DAC(0x19,0x02);	  
	I2CWRNBYTE_CODEC_DAC(0x21,0x1F);	  
	I2CWRNBYTE_CODEC_DAC(0x22,0x02);	  
	I2CWRNBYTE_CODEC_DAC(0x25,0x21);	  
	I2CWRNBYTE_CODEC_DAC(0x25,0x01);	
	I2CWRNBYTE_CODEC_DAC(0x25,0x87);	  
	I2CWRNBYTE_CODEC_DAC(0x18,0x01);	  
	I2CWRNBYTE_CODEC_DAC(0x09,0x02);	  
	I2CWRNBYTE_CODEC_DAC(0x09,0x01);	  
	I2CWRNBYTE_CODEC_DAC(0x08,0x00);	  
}

void ES8156_dac_Standby_NoPop(void)
{
	I2CWRNBYTE_CODEC_DAC(0x14,0x00);	  
	I2CWRNBYTE_CODEC_DAC(0x19,0x02);	  
	I2CWRNBYTE_CODEC_DAC(0x21,0x1F);	  
	I2CWRNBYTE_CODEC_DAC(0x22,0x02);	  
	I2CWRNBYTE_CODEC_DAC(0x25,0x21);	  
	I2CWRNBYTE_CODEC_DAC(0x25,0xA1);	  
	I2CWRNBYTE_CODEC_DAC(0x18,0x01);	  
	I2CWRNBYTE_CODEC_DAC(0x09,0x02);	  
	I2CWRNBYTE_CODEC_DAC(0x09,0x01);	  
	I2CWRNBYTE_CODEC_DAC(0x08,0x00);	  
}

void BMP8563_init(void)
{
	I2CWRNBYTE_CODEC_RTC(0x00, 0x00);
	I2CWRNBYTE_CODEC_RTC(0x01, 0x00);
	I2CWRNBYTE_CODEC_RTC(0x0D, 0x00);
}

void BMP8563_setTime(rtc_date_t *date)
{
	if(date == NULL)
	{
		return;
	}
	uint8_t time_buf[7];
    time_buf[0] = byte2BCD(date->second);
    time_buf[1] = byte2BCD(date->minute);
    time_buf[2] = byte2BCD(date->hour);
    time_buf[3] = byte2BCD(date->day);
    time_buf[5] = byte2BCD(date->month) | (date->year >= 2000 ? 0x00 : 0x80);
    time_buf[6] = byte2BCD(date->year % 100);
    I2CWRNBUFFER_CODEC_RTC(0x02, time_buf, 7);
}

void BMP8563_getTime(rtc_date_t *date)
{
	if(date == NULL)
	{
		return;
	}
	uint8_t time_buf[7];
	I2CWRDBUFFER_CODEC_RTC(0x02, time_buf, 7);
	date->second = BCD2Byte(time_buf[0] & 0x7f);
    date->minute = BCD2Byte(time_buf[1] & 0x7f);
    date->hour = BCD2Byte(time_buf[2] & 0x3f);
    date->day = BCD2Byte(time_buf[3] & 0x3f);
    date->month = BCD2Byte(time_buf[5] & 0x1f);
    date->year = BCD2Byte(time_buf[6]) + (time_buf[5] & 0x80 ? 1900 : 2000);
}

void SGM415_power_init(void)
{
	SGM415_power_SetiChrgCurr(SGM4154x_ICHRG_I_DEF_uA);
	SGM415_power_SetPreChrgCurr(SGM4154x_PRECHRG_I_DEF_uA);
	SGM415_power_SetChrgVolt(SGM4154x_VREG_V_DEF_uV);
	SGM415_power_SetChrgTermCurr(SGM4154x_TERMCHRG_I_DEF_uA);
	SGM415_power_SetChrgCurrLim(SGM4154x_IINDPM_DEF_uA);
	SGM415_power_SetReChargeCurr(200);
}

void SGM415_power_SetChrgVolt(uint32_t chrg_volt)
{
	uint8_t reg_val = (chrg_volt - SGM4154x_VREG_V_MIN_uV) / SGM4154x_VREG_V_STEP_uV;
	reg_val = reg_val << 3;
	I2CUPBYTE_CODEC_POWER(SGM4154x_CHRG_CTRL_4, SGM4154x_VREG_V_MASK, reg_val);
}

uint32_t SGM415_power_GetChrgVolt(void)
{
	uint32_t volt = 0;
	uint8_t vreg_val = 0;
	
	I2CWRDBYTE_CODEC_POWER(SGM4154x_CHRG_CTRL_4, &vreg_val);
	
	vreg_val = (vreg_val & SGM4154x_VREG_V_MASK) >> 3;

	if (15 == vreg_val)
	{
		volt = 4352000; 
	}
	else if (vreg_val < 25)	
	{
		volt = vreg_val * SGM4154x_VREG_V_STEP_uV + SGM4154x_VREG_V_MIN_uV;	
	}
	return volt;
}

void SGM415_power_SetChrgCurrLim(uint32_t chrg_uA)
{
	uint8_t reg_val = (chrg_uA - SGM4154x_IINDPM_I_MIN_uA) / SGM4154x_IINDPM_STEP_uA;

	I2CUPBYTE_CODEC_POWER(SGM4154x_CHRG_CTRL_0, SGM4154x_IINDPM_I_MASK, reg_val);
}

uint32_t SGM415_power_GetCurrLim(void)
{
	uint32_t Curr = 0;
	uint8_t ireg_val = 0;
	
	I2CWRDBYTE_CODEC_POWER(SGM4154x_CHRG_CTRL_0, &ireg_val);
	
	ireg_val = (ireg_val & SGM4154x_IINDPM_I_MASK);

	Curr = ireg_val * SGM4154x_IINDPM_STEP_uA + SGM4154x_IINDPM_I_MIN_uA;	
	
	return Curr;
}

void SGM415_power_SetChrgTermCurr(uint32_t chrg_uA)
{
	const unsigned int ITERM_CURRENT_STABLE[] = {
		5000, 10000, 15000, 20000, 30000, 40000, 50000, 60000, 80000, 100000, 120000, 140000, 160000, 180000, 200000, 240000
	};
	uint8_t ireg_val  = 0;
	
	if (chrg_uA < SGM4154x_TERMCHRG_I_MIN_uA)
	{
		chrg_uA = SGM4154x_TERMCHRG_I_MIN_uA;
	}
	else if (chrg_uA > SGM4154x_TERMCHRG_I_MAX_uA)
	{
		chrg_uA = SGM4154x_TERMCHRG_I_MAX_uA;
	}
	
	for(ireg_val = 1; ireg_val < 16 && chrg_uA >= ITERM_CURRENT_STABLE[ireg_val]; ireg_val++){;}
	ireg_val--;

	I2CUPBYTE_CODEC_POWER(SGM4154x_CHRG_CTRL_3, SGM4154x_TERMCHRG_CUR_MASK, ireg_val);
}

void SGM415_power_SetPreChrgCurr(uint32_t chrg_uA)
{
	const unsigned int IPRECHG_CURRENT_STABLE[] = {
		5000, 10000, 15000, 20000, 30000, 40000, 50000, 60000, 80000, 100000, 120000, 140000, 160000, 180000, 200000, 240000
	};
	uint8_t ireg_val  = 0;
	
	if (chrg_uA < SGM4154x_PRECHRG_I_MIN_uA)
	{
		chrg_uA = SGM4154x_PRECHRG_I_MIN_uA;
	}
	else if (chrg_uA > SGM4154x_PRECHRG_I_MAX_uA)
	{
		chrg_uA = SGM4154x_PRECHRG_I_MAX_uA;
	}
	
	for(ireg_val = 1; ireg_val < 16 && chrg_uA >= IPRECHG_CURRENT_STABLE[ireg_val]; ireg_val++){;}
	ireg_val--;
	
	ireg_val = ireg_val << 4;
	
	I2CUPBYTE_CODEC_POWER(SGM4154x_CHRG_CTRL_3, SGM4154x_PRECHRG_CUR_MASK, ireg_val);
}

void SGM415_power_SetiChrgCurr(uint32_t chrg_uA)
{
	uint8_t ireg_val  = 0;
	
	if (chrg_uA < SGM4154x_ICHRG_I_MIN_uA)
	{
		chrg_uA = SGM4154x_ICHRG_I_MIN_uA;
	}
	else if ( chrg_uA > SGM4154x_ICHRG_I_MAX_uA)
	{
		chrg_uA = SGM4154x_ICHRG_I_MAX_uA;
	}
	
	if (chrg_uA <= 40000)
	{
		ireg_val = chrg_uA / 5000;	
	}
	else if (chrg_uA <= 110000)
	{
		ireg_val = 0x08 + (chrg_uA -40000) / 10000;	
	}
	else if (chrg_uA <= 270000)
	{
		ireg_val = 0x0F + (chrg_uA -110000) / 20000;	
	}
	else if (chrg_uA <= 540000)
	{
		ireg_val = 0x17 + (chrg_uA -270000) / 30000;	
	}
	else if (chrg_uA <= 1500000)
	{
		ireg_val = 0x20 + (chrg_uA -540000) / 60000;
	}		
	else if (chrg_uA <= 2940000)
	{
		ireg_val = 0x30 + (chrg_uA -1500000) / 120000;
	}
	else 
	{
		ireg_val = 0x3d;
	}
	
	I2CUPBYTE_CODEC_POWER(SGM4154x_CHRG_CTRL_2, SGM4154x_ICHRG_I_MASK, ireg_val);
}


uint8_t SGM415_power_GetVindpmOffsetOs(void)
{
	uint8_t reg_val = 0;
	
	I2CWRDBYTE_CODEC_POWER(SGM4154x_CHRG_CTRL_f, &reg_val);
	
	reg_val = reg_val & SGM4154x_VINDPM_OS_MASK;
	
	return reg_val;
}

void SGM415_power_SetVindpmOffsetOs(uint8_t offset_os)
{
	I2CUPBYTE_CODEC_POWER(SGM4154x_CHRG_CTRL_f, SGM4154x_VINDPM_OS_MASK, offset_os);
}

void SGM415_power_SetReChargeCurr(uint32_t chrg_mV)
{
	uint8_t vreg_val = 0;
	
	vreg_val = (chrg_mV - SGM4154x_VRECHRG_OFFSET_mV) / SGM4154x_VRECHRG_STEP_mV;
	
	I2CUPBYTE_CODEC_POWER(SGM4154x_CHRG_CTRL_4, SGM4154x_VRECHARGE, vreg_val);
}







