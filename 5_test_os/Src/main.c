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
#include "timebase.h"
#include "os_kernel.h"
uint32_t sp_antes, sp_durante, sp_despues;
uint32_t stackFrame[8];

extern uint32_t sp_tarea1;					//Stack Pointer para la tarea 1
extern uint32_t sp_tarea2;					//Stack Pointer para la tarea 2
extern uint32_t sp_tarea3;					//Stack Pointer para la tarea 3
void task1(void *arg)
{
	int i;
	while (1) {
		i++;
	}
}

void task2(void *arg)
{
	int j;
	while (1) {
		j++;
	}
}

void task3(void *arg)
{
	int j;
	while (1) {
		j++;
	}
}

int main(void)
{
	struct task_block *task_block1;
	struct task_block *task_block2;
	struct task_block *task_block3;

	led_init();
	uart_tx_init();
	tick_init();
	os_init();
	task_block1 = task_create("tarea1", task1, NULL);
	sp_tarea1 = task_block1->sp;
	task_block2 = task_create("tarea2", task2, NULL);
	sp_tarea2 = task_block2->sp;
	task_block3 = task_create("tarea3", task3, NULL);
	sp_tarea3 = task_block3->sp;
    /* Loop forever */

	while (1) {
		//__WFI();
	}
}

