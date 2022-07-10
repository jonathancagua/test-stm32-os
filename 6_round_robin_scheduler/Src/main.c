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
#include "led.h"
#include "uart.h"
//#include "timebase.h"

void task1_run(void);
void task2_run(void);
void task1_stop(void);
void task2_stop(void);

int task1_main(void);
int task2_main(void);

int main(void)
{
	uint32_t volatile start  = 0U;
	led_init();
	uart_tx_init();
	//tick_init();
    /* Loop forever */
	if(start)
	{
		task1_main();

	}
	else{
		task2_main();
	}

	while(1)
	{


	}
}


int task1_main(void)
{
	while(1)
	{
		task1_run();
		tick_delay(1);
		task1_stop();
		tick_delay(1);
	}
}

int task2_main(void)
{
	while(1)
	{
		task2_run();
		tick_delay(1);
		task2_stop();
		tick_delay(1);
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
