#include <stdint.h>
#include <rtthread.h>

#include "hc32_ddl.h"
#include "lvgl/lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"

#include "cry_usart.h"

#define CPU_USAGE_CALC_TICK    1000  // 计算周期
#define CPU_USAGE_LOOP        10

static rt_uint8_t cpu_usage_major = 0, cpu_usage_minor= 0;
static rt_uint32_t total_count = 0;

void RT_SysTick_Init(void);
void rt_cpu_usage_hook(void);

void SysClkConfig(void)
{
    stc_clk_xtal_cfg_t   stcXtalCfg;
    stc_clk_mpll_cfg_t   stcMpllCfg;
    en_clk_sys_source_t  enSysClkSrc;
    stc_clk_sysclk_cfg_t stcSysClkCfg;


    MEM_ZERO_STRUCT(enSysClkSrc);
    MEM_ZERO_STRUCT(stcSysClkCfg);
    MEM_ZERO_STRUCT(stcXtalCfg);
    MEM_ZERO_STRUCT(stcMpllCfg);


    stcSysClkCfg.enHclkDiv  = ClkSysclkDiv1;
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv2;
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;
    CLK_SysClkConfig(&stcSysClkCfg);


    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);


    stcMpllCfg.pllmDiv = 1u;
    stcMpllCfg.plln = 50u; 	// 42 168M;  48 192M   60  240M
    stcMpllCfg.PllpDiv = 2u;
    stcMpllCfg.PllqDiv = 2u;
    stcMpllCfg.PllrDiv = 2u;


    CLK_SetPllSource(ClkPllSrcXTAL);
    CLK_MpllConfig(&stcMpllCfg);

    EFM_Unlock();
    EFM_SetLatency(EFM_LATENCY_4);
    EFM_Lock();

    CLK_MpllCmd(Enable);

    while (Set != CLK_GetFlagStatus(ClkFlagMPLLRdy))
    {
    }


    CLK_SetSysClkSource(CLKSysSrcMPLL);
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
// heap default size: 64K(1024 * 16 * 4), 芯片RAM 192K 包含32K RAM 与 4K RET RAM（断电保护），（每块大小64K）(page 55)
// RAM1 	64K 0x20000000~0x2000FFFF Even-parity check
// RAM2 	64K 0x20010000~0x2001FFFF Even-parity check
// RAM3 	28K 0x20020000~0x20026FFF ECC check
// RET_RAM 	 4K 0x200F0000~0x200F0FFF Even-parity check
// RET_H 	32K 0x1FFF8000~0x1FFFFFFF Even-parity check
#define RT_HEAP_SIZE (16 * 1024)
static uint32_t rt_heap[RT_HEAP_SIZE] __attribute__((at(0x20000000)));    
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif



void rt_hw_board_init()
{	
	SysClkConfig();
	RT_SysTick_Init();

	DDL_PrintfInit(DEBUG_USART_CH, DEBUG_USART_BAUDRATE, uart_init);
	
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif


#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
	
	rt_thread_idle_sethook(rt_cpu_usage_hook);
}

void SysTick_Handler(void)
{
    rt_interrupt_enter();

    rt_tick_increase();

    rt_interrupt_leave();

    lv_tick_inc(1);
}

void RT_SysTick_Init(void)
{
    stc_clk_freq_t stcClkFreq;

    MEM_ZERO_STRUCT(stcClkFreq);

    CLK_GetClockFreq(&stcClkFreq);
	
    SysTick_Config(stcClkFreq.sysclkFreq / RT_TICK_PER_SECOND);
}

void rt_hw_console_output(const char *str)
{
	rt_size_t i = 0, size = 0;
	char a = '\r';

	size = rt_strlen(str);
	for (i = 0; i < size; i++)
	{
		if (*(str + i) == '\n')
		{
			DEBUG_USART_CH->DR = a;
		    while (0ul == DEBUG_USART_CH->SR_f.TC)
		    {
		        ;
		    }
		}
		DEBUG_USART_CH->DR = *(str + i);
	    while (0ul == DEBUG_USART_CH->SR_f.TC)
	    {
	        ;
	    }
	}
}

char rt_hw_console_getchar(void)
{
    int ch = -1;

	if (Set == USART_GetStatus(DEBUG_USART_CH, UsartRxNoEmpty))
	{
		ch = DEBUG_USART_CH->DR_f.RDR;
    }
    else
    {
		if(Set == USART_GetStatus(DEBUG_USART_CH, UsartOverrunErr))
		{
            USART_ClearStatus(DEBUG_USART_CH, UsartOverrunErr);
        }
        rt_thread_mdelay(10);
    }
    return ch;
}

void rt_cpu_usage_hook(void)
{
	rt_tick_t tick;
    rt_uint32_t count;
	volatile rt_uint32_t loop;
	
	if (total_count == 0) 
	{                            
        rt_enter_critical();                                
        tick = rt_tick_get();                               
		while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK) 
		{
            total_count ++;                         
            loop = 0;
			while (loop < CPU_USAGE_LOOP) loop ++;
        }
        rt_exit_critical();
    }
	
	count = 0;

    tick = rt_tick_get();                           
	while (rt_tick_get() - tick < CPU_USAGE_CALC_TICK) 
	{
        count ++;                                   
        loop  = 0;
		while (loop < CPU_USAGE_LOOP) loop ++;
    }

	if (count < total_count) 
	{                          
        count = total_count - count;
        cpu_usage_major = (count * 100) / total_count;
        cpu_usage_minor = ((count * 100) % total_count) * 100 / total_count;
    } 
	else 	
	{
        total_count = count;                               
    	cpu_usage_major = 0;
    	cpu_usage_minor = 0;
    }
}

void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor)
{
    RT_ASSERT(major != RT_NULL);
    RT_ASSERT(minor != RT_NULL);

    *major = cpu_usage_major;                               
    *minor = cpu_usage_minor;
}
