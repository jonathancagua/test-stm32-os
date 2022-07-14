/**
 * @file main.c
 * @author Jonathan Cagua(jonathan.cagua@gmail.com)
 * @brief testing de componentes
 * @version 0.1
 * @date 2022-07-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdint.h>
#include <stdio.h>
#include "led.h"
#include "uart.h"
//#include "timebase.h"
#include "os_kernel.h"
#define QUANTA	10
typedef uint32_t task_profile;

task_profile task0_profiler,task1_profiler,task2_profiler;

void task1_main(void);
void task2_main(void);
void task0_main(void);

int main(void)
{
	uint32_t volatile start  = 0U;
	led_init();
	uart_tx_init();
	// init del kernel
	os_kernel_init();
	// agrego el thread
	// seteo el robin time quanta
	os_kernel_add_threads(&task0_main, &task1_main, &task2_main);
	os_kernel_launch(QUANTA);
	while(1);
}


void task0_main(void)
{
	while(1)
	{
		task0_profiler++;
		os_thread_yield();
	}
}

void task1_main(void)
{
	while(1)
	{
		task1_profiler++;
	}
}

void task2_main(void)
{
	while(1)
	{
		task2_profiler++;
	}
}


void task1_run(void)
{
	printf("task1 is starting...\n\r");
}


void task1_stop(void)
{
	printf("task1 is stopping...\n\r");
}

void task2_run(void)
{
	printf("task2 is opening...\n\r");
}


void task2_stop(void)
{
	printf("task2 is closing...\n\r");
}
