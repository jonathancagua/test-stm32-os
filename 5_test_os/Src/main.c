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
#include "os_kernel.h"

void task1(void *arg)
{
	int i;
	int k;
	while (1) {
		i++;
		k++;
	}
}

void task2(void *arg)
{
	int j;
	int k;
	while (1) {
		j++;
		k++;
	}
}

void task3(void *arg)
{
	int j;
	int k;
	while (1) {
		j++;
		k++;
	}
}

int main(void)
{
	struct task_block *task_block1;
	struct task_block *task_block2;
	struct task_block *task_block3;

	led_init();
	uart_tx_init();
	os_init();
	task_block1 = task_create("tarea1", task1, NULL);
	task_block2 = task_create("tarea2", task2, NULL);
	task_block3 = task_create("tarea3", task3, NULL);
    /* Loop forever */

	while (1) {
	}
}

