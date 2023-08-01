/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : main.c
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x 噪声传感器
*******************************************************************************/
#include "hc32_ddl.h"
#include "header.h"


#define FLASH_PROGRAM_CHECK				(FLASH_SECTOR_ADRR + FLASH_SECTOR_LEN - 4u)

void lvgl_task(void *parameter);
void oct_task(void *parameter);
void title_task(void *parameter);
void i2s_task(void *parameter);
void timer_10ms_handler(void *parameter);
void main_task(void *parameter);
void cali_task(void *parameter);

long cmd_cpu_usage(void);


#define CALIBRATION_REQUEST_EVENT		(0x1 << 1)
#define CALIBRATION_START_EVENT			(0x1 << 2)
#define CALIBRATION_SUCCESS_EVENT		(0x1 << 3)
#define CALIBRATION_FAIL_EVENT			(0x1 << 3)

rt_event_t calibration_event;

void paraminit(void)
{
	if((*(uint32_t*)(FLASH_PROGRAM_CHECK)) != (0x12345678u)) //检测校验参数，若检测失败则重置所有参数
	{
		Flash_Write(FLASH_PROGRAM_CHECK, 0x12345678u);
		// TO DO ALL PARAM
	}
	else
	{
		// TO DO IMPORTANT PARAM
	}
	
}

void MainInit(void)
{
	paraminit();
	i2c_init();
	i2s_init();
	adc_init();
	pwm_init();
	lv_init();  //初始化lvgl
    lv_port_disp_init();    //显示初始化	
}

/********************************** TEST UI START ***************************************/
lv_obj_t * label2;
lv_obj_t * label3;
lv_obj_t * label4;
lv_obj_t * label5;
lv_obj_t * label6;
uint8_t index_tmp = 3;

static void video_disp_window()
{
	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(0x00, 0xff, 0xff), 0); 
	label2 = lv_label_create(lv_scr_act());
	label3 = lv_label_create(lv_scr_act());
	label4 = lv_label_create(lv_scr_act());
	label5 = lv_label_create(lv_scr_act());
	label6 = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label2, true);
	lv_label_set_recolor(label3, true);
	lv_label_set_recolor(label4, true);
	lv_label_set_recolor(label5, true);
	lv_label_set_recolor(label6, true);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_WRAP);
	lv_label_set_long_mode(label3, LV_LABEL_LONG_WRAP);
	lv_label_set_long_mode(label4, LV_LABEL_LONG_WRAP);
	lv_label_set_long_mode(label5, LV_LABEL_LONG_WRAP);
	lv_label_set_long_mode(label6, LV_LABEL_LONG_WRAP);	/*Circular scroll*/
	lv_obj_set_width(label4, 20);
	lv_obj_set_width(label5, 20);
	lv_label_set_text(label2, "2000-00-00 00:00:00");
	lv_label_set_text(label3, "3.12V");
	lv_label_set_text(label4, "2");
	lv_label_set_text(label5, "#ff0000 A !#");
	lv_label_set_text(label6, "LAF 96dB");
    lv_obj_align(label2, LV_ALIGN_TOP_LEFT, 0, 0);
	lv_obj_align(label3, LV_ALIGN_TOP_RIGHT, 0,0);
	lv_obj_align(label4, LV_ALIGN_CENTER, 20,0);
	lv_obj_align(label5, LV_ALIGN_CENTER, 30,0);
	lv_obj_align(label6, LV_ALIGN_CENTER, 0,30);
}
/********************************** TEST UI END ***************************************/


int32_t main(void)
{
	MainInit();
		
	video_disp_window();
	
	rt_thread_t lvgl_task_tid = rt_thread_create("lvglTask", lvgl_task, RT_NULL, 2048, 10, 4);
	rt_thread_t oct_task_tid = rt_thread_create("octTask",oct_task, RT_NULL, 2048, 19, 4);
	rt_thread_t title_task_tid = rt_thread_create("titleTask", title_task, RT_NULL, 2048, 18, 4);
	rt_thread_t main_task_tid = rt_thread_create("mainTask", main_task, RT_NULL, 2048, 18, 4);
	rt_thread_t i2s_task_tid = rt_thread_create("i2sTask", i2s_task, RT_NULL, 4 * 1024, 10, 4);
	rt_thread_t cali_task_tid = rt_thread_create("caliTask", cali_task, RT_NULL, 2 * 1024, 10, 4);
	
	rt_timer_t handler_timer_10ms = rt_timer_create("timer10ms", timer_10ms_handler, RT_NULL, 10, RT_TIMER_FLAG_PERIODIC);
	
	calibration_event = rt_event_create("calibration_event", RT_IPC_FLAG_FIFO);
	
    if (lvgl_task_tid != RT_NULL)
	{
		rt_thread_startup(lvgl_task_tid);
	}
	
	if (oct_task_tid != RT_NULL)
	{
		rt_thread_startup(oct_task_tid);
	}
	
	if(title_task_tid != RT_NULL)
	{
		rt_thread_startup(title_task_tid);
	}
	
	if(main_task_tid != RT_NULL)
	{
		rt_thread_startup(main_task_tid);
	}
	
	if(i2s_task_tid != RT_NULL)
	{
		rt_thread_startup(i2s_task_tid);
	}
	
	if(cali_task_tid != RT_NULL)
	{
		rt_thread_startup(cali_task_tid);
	}
	
	if (handler_timer_10ms != RT_NULL)
	{
		rt_timer_start(handler_timer_10ms);
	}
}

/************************************ RT THREAD TIMER *****************************************/
static void timer_10ms_handler(void *parameter)
{
	static uint32_t temp_count = 0;
	
	temp_count++;
		
	SLM_Basic_Cal();
	
	if(SLM_Overload == 1)
	{
		temp_count++;
		if(temp_count > 200) // 2s
		{
			SLM_Overload = 0;
		} 
	}
	else
	{
		temp_count = 0;
	}
}

/************************************ RT THREAD TASK *****************************************/
static void lvgl_task(void *parameter)
{
    while (1)
    {
        lv_task_handler(); 
		rt_thread_mdelay(40);
    }
}

static void title_task(void *parameter)
{
	static uint32_t temp_count = 0;
	
	float voltage = getAdcValue();
	lv_label_set_text_fmt(label3, "%.2fV", voltage);
	
    while (1)
    {		
		rtc_date_t date;
		BMP8563_getTime(&date);
		lv_label_set_text_fmt(label2, "%04d-%02d-%02d %02d:%02d:%02d",  date.year, date.month, date.day, date.hour, date.minute, date.second);

		temp_count++;
		if(temp_count % 5 == 0)
		{
			voltage = getAdcValue();
			lv_label_set_text_fmt(label3, "%.2fV", voltage);
		}
		
		rt_thread_mdelay(1000);
    }
}

static void main_task(void *parameter)
{
	static uint32_t temp_count = 0;
	rt_uint32_t event_recved;
	
    while (1)
    {		
		// TO DO Check Application config and parammeter
		temp_count++;
		if(temp_count % 20 == 0)
		{
			lv_label_set_text_fmt(label6, "LAF %.1fdB", SLM_SPL_AF / 10.0f);
		}
		
		rt_event_recv(calibration_event, CALIBRATION_START_EVENT | CALIBRATION_SUCCESS_EVENT | CALIBRATION_FAIL_EVENT, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, &event_recved);
		if((event_recved & CALIBRATION_START_EVENT) == CALIBRATION_START_EVENT)
		{
			// TO DO 校准开始
		}
		else if((event_recved & CALIBRATION_SUCCESS_EVENT) == CALIBRATION_SUCCESS_EVENT)
		{
			// TO DO 校准完成， 结果：成功
		}
		else if((event_recved & CALIBRATION_FAIL_EVENT) == CALIBRATION_FAIL_EVENT)
		{
			// TO DO 校准完成， 结果：失败
		}
	
		rt_thread_mdelay(50);
    }
}

static void oct_task(void *parameter)
{
    while (1)
    {		
		SLM_OCT_Process();	
		
		rt_thread_mdelay(2);
    }
}

//static void i2s_task(void *parameter)
//{
//	int32_t *i2s_buffer = (int32_t *)rt_malloc(I2S2_BUFFER_SIZE * sizeof(int32_t));
//	
//	OctaveWeightResultType *octaveWeightResult = (OctaveWeightResultType *)rt_malloc(3 * sizeof(OctaveWeightResultType));
//	memset(octaveWeightResult, 0, sizeof(OctaveWeightResultType));
//	
//	octaveInit();
//	
//	rt_thread_mdelay(500);
//	
//	i2s_start();
//	
//	while (1)
//	{
//		i2s_ReadBuffer(i2s_buffer);
//		
//		for(uint32_t i = 0; i < (I2S2_BUFFER_SIZE >> 1u); i ++)
//		{
//			octaveProcess((float)(i2s_buffer[i * 2u] / (float)(0x1FFFFFFF)));
//		}
//	}
//}

static void i2s_task(void *parameter)
{
	int32_t *i2s_buffer = (int32_t *)rt_malloc(I2S2_BUFFER_SIZE * sizeof(int32_t));
		
	rt_thread_mdelay(500);
	
	i2s_start();
	
	while (1)
	{
		i2s_ReadBuffer(i2s_buffer);
		
		for(uint32_t i = 0; i < I2S2_BUFFER_SIZE; i ++)
		{
			SLM_Data_In32(i2s_buffer[i], i % 2 == 0? 1 : 0);
		}
	}
}

static void cali_task(void *parameter)
{
	rt_uint32_t event_recved;
	while (1)
	{
		rt_event_recv(calibration_event, CALIBRATION_REQUEST_EVENT, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &event_recved);
		
		if(event_recved == CALIBRATION_REQUEST_EVENT)
		{
			rt_event_send(calibration_event, CALIBRATION_START_EVENT);
			
			int32_t result = SLM_Calibration();
			
			if(result == 1)
			{
				rt_event_send(calibration_event, CALIBRATION_SUCCESS_EVENT);
			}
			else
			{
				rt_event_send(calibration_event, CALIBRATION_FAIL_EVENT);
			}
		}
		else
		{
			rt_thread_mdelay(500);
		}
	}
}

/********************************** RT THREAD CMD TEST ***************************************/
long cmd_test(void)
{
	rt_kprintf("cmd test hello world\n");
	return 0;
}
FINSH_FUNCTION_EXPORT(cmd_test, test cmd);
MSH_CMD_EXPORT(cmd_test, test cmd);

long cmd_view(void)
{
	char buf[256] = {0};
		
	sprintf(buf, "%.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f | %.1f %.1f", 
		Oct3L_SPL[9] / 10.0, Oct3M_SPL[9] / 10.0, Oct3H_SPL[9] / 10.0,
		Oct3L_SPL[8] / 10.0, Oct3M_SPL[8] / 10.0, Oct3H_SPL[8] / 10.0,
		Oct3L_SPL[7] / 10.0, Oct3M_SPL[7] / 10.0, Oct3H_SPL[7] / 10.0,
		Oct3L_SPL[6] / 10.0, Oct3M_SPL[6] / 10.0, Oct3H_SPL[6] / 10.0,
		Oct3L_SPL[5] / 10.0, Oct3M_SPL[5] / 10.0, Oct3H_SPL[5] / 10.0,
		Oct3L_SPL[4] / 10.0, Oct3M_SPL[4] / 10.0, Oct3H_SPL[4] / 10.0,
		Oct3L_SPL[3] / 10.0, Oct3M_SPL[3] / 10.0, Oct3H_SPL[3] / 10.0,
		Oct3L_SPL[2] / 10.0, Oct3M_SPL[2] / 10.0, Oct3H_SPL[2] / 10.0,
		Oct3L_SPL[1] / 10.0, Oct3M_SPL[1] / 10.0, Oct3H_SPL[1] / 10.0,
		Oct3L_SPL[0] / 10.0, Oct3M_SPL[0] / 10.0, Oct3H_SPL[0] / 10.0,
		SLM_SPL_AF / 10.0f, SLM_dBV_AF / 10.0f);
	
	rt_kprintf("%s\n", buf);
	
	return 0;
}
FINSH_FUNCTION_EXPORT(cmd_view, test view);
MSH_CMD_EXPORT(cmd_view, test view);

long cmd_cpu_usage(void)
{
	extern void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor);
	
	rt_uint8_t major;
	rt_uint8_t minor;
	
	cpu_usage_get(&major, &minor);
	
	rt_kprintf("CPU Usage = %d.%d%\n", major, minor);
	
	return 0;
}
FINSH_FUNCTION_EXPORT(cmd_cpu_usage, cpu usage);
MSH_CMD_EXPORT(cmd_cpu_usage, cpu usage);



