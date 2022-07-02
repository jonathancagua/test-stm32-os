/**
 * @file timebase.c
 * @author Jonathan Cagua(jonathan.cagua@gmail.com)
 * @brief componente que solo escribe por print usnado weak
 * @version 0.1
 * @date 2022-07-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "timebase.h"
#include "stm32l4xx.h"

#define ONE_SEC_LOAD		4000000//Startup clock ES DE 4mhz LO DICE EL DATA

#define CTRL_ENABLE			(1U<<0)
#define CTRL_TICK_INT		(1U<<1)
#define CTRL_CLCK_SRC		(1U<<2)
#define CTRL_COUNT_FLAG		(1U<<16)

#define DELAY_MAX		0xFFFFFFFFU

volatile uint32_t tick_current;
volatile uint32_t g_curr_tick_p;

volatile uint32_t tick_freq = 1;
void tick_increment(void);


//Esta declaracion lo sacas del startup linea 145
void SysTick_Handler(void)
{
	tick_increment();
}


uint32_t tick_get(void)
{
	__disable_irq();
	g_curr_tick_p = tick_current;
	__enable_irq();


	return g_curr_tick_p;
}

void tick_delay(uint32_t delay)
{
	uint32_t tick_start =  tick_get();
	uint32_t time_out =  delay;

	if(time_out < DELAY_MAX)
	{
		time_out += (uint32_t)(tick_freq);
	}

	while((tick_get() - tick_start) < time_out){}

}

void tick_increment(void)
{
	tick_current += tick_freq;
}

void tick_init(void)
{
	//Cargar el numero de ciclos por segundo
	SysTick->LOAD =  ONE_SEC_LOAD - 1;

	//Limpiar el Systick
	SysTick->VAL  = 0;

	//seleccionar el clock interno
	SysTick->CTRL  = CTRL_CLCK_SRC;

	//activar interrupcion
	SysTick->CTRL  |= CTRL_TICK_INT;

	//activar el systick
	SysTick->CTRL  |= CTRL_ENABLE;

	//activar interrrupcion global
	__enable_irq();
}
