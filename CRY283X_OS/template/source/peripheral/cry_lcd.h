/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_lcd.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x 噪声传感器
*******************************************************************************/
#ifndef __CRY_LCD_H__
#define __CRY_LCD_H__

#include "hc32_ddl.h"

#define LCD_W 									240
#define LCD_H 									320

#define LCD_RES_PORT           					(PortA)
#define LCD_RES_PIN            					(Pin06)

#define LCD_DC_PORT            					(PortA)
#define LCD_DC_PIN             					(Pin08)

#define LCD_BLK_PORT           					(PortE)
#define LCD_BLK_PIN            					(Pin02)

#define LCD_NSS_PORT            				(PortA)
#define LCD_NSS_PIN             				(Pin04)

#define LCD_NSS_HIGH()     						PORT_SetBits(LCD_NSS_PORT, LCD_NSS_PIN)
#define LCD_NSS_LOW()      						PORT_ResetBits(LCD_NSS_PORT, LCD_NSS_PIN)

#define LCD_RES_Clr()  							M4_PORT->PORRA |= LCD_RES_PIN        //RES
#define LCD_RES_Set()  							M4_PORT->POSRA |= LCD_RES_PIN

#define LCD_DC_Clr()   							M4_PORT->PORRA |= LCD_DC_PIN         //DC
#define LCD_DC_Set()   							M4_PORT->POSRA |= LCD_DC_PIN

#define LCD_BLK_Clr()  							PORT_ResetBits(LCD_BLK_PORT, LCD_BLK_PIN)    //BLK
#define LCD_BLK_Set() 							PORT_SetBits(LCD_BLK_PORT, LCD_BLK_PIN)


#define LCD_SPI_SCK_PORT                    	(PortA)
#define LCD_SPI_SCK_PIN                     	(Pin05)
#define LCD_SPI_SCK_FUNC                    	(Func_Spi2_Sck)


#define LCD_SPI_MOSI_PORT                   	(PortA)
#define LCD_SPI_MOSI_PIN                    	(Pin07)
#define LCD_SPI_MOSI_FUNC                   	(Func_Spi2_Mosi)


#define LCD_SPI_UNIT                        	(M4_SPI2)
#define LCD_SPI_UNIT_CLOCK                  	(PWC_FCG1_PERIPH_SPI2)


#define LCD_SPI_DMA_UNIT                    	(M4_DMA1)
#define LCD_SPI_DMA_CLOCK_UNIT              	(PWC_FCG0_PERIPH_DMA1)
#define LCD_SPI_DMA_TX_CHANNEL              	(DmaCh1)
#define LCD_SPI_DMA_RX_CHANNEL              	(DmaCh0)
#define LCD_SPI_DMA_TX_TRIG_SOURCE          	(EVT_SPI2_SPTI)
#define LCD_SPI_DMA_RX_TRIG_SOURCE          	(EVT_SPI2_SPRI)



#define WHITE               0xFFFF
#define BLACK         	    0x0000	  
#define BLUE           	    0x001F  
#define BRED                0xF81F
#define GRED 			    0xFFE0
#define GBLUE			    0x07FF
#define RED           	    0xF800
#define MAGENTA       	    0xF81F
#define GREEN         	    0x07E0
#define CYAN          	    0x7FFF
#define YELLOW        	    0xFFE0
#define BROWN 		        0XBC40 	// 棕色
#define BRRED 			    0XFC07 	// 棕红色
#define GRAY  			    0X8430 	// 灰色
#define DARKBLUE      	    0X01CF	// 深蓝色
#define LIGHTBLUE      	    0X7D7C	// 浅蓝色  
#define GRAYBLUE       	    0X5458 	// 灰蓝色
#define LIGHTGREEN     	    0X841F 	// 浅绿色
#define LGRAY 			    0XC618 	// 浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE           0XA651 	// 浅灰蓝色(中间层颜色)
#define LBBLUE              0X2B12 	// 浅棕蓝色(选择条目的反色)






void lcd_gpio_Init(void);
void lcd_spi_init(void);
void lcd_spi_trans(uint8_t *dat, uint16_t len);
void LCD_SPI_Write(uint8_t Data);
void LCD_Write_Bus(uint8_t dat);
void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA(uint16_t dat);
void LCD_WR_REG(uint8_t dat);
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

void lcd_init(void);
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);
void LCD_BackLight(bool on_off);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode);
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);

void LCD_Printf(uint16_t x1,uint16_t y1, uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode, const char *p, ...);




#endif
