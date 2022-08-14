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
#include "button.h"
struct semaphore sem_task2;
struct queue queue_task1;
void task1(void *arg)
{
	int i=0;
	int k=0;
	while (1) {
		i++;
		led_on();
		queue_write(&queue_task1, &i);
		task_delay_s(3);
		queue_write(&queue_task1, &i);
		led_off();
		task_delay_s(3);
		k++;
	}
}

void task2(void *arg)
{
	int j;
	int k;
	while (1) {
		j++;
		//semaphore_take(&sem_task2);
		queue_read(&queue_task1, &j);
		printf("task2 recibe porla cola de task 1 = %d \n\r",j);
		k++;
	}
}

void task3(void *arg)
{
	int j;
	int k;
	uint32_t valor = *((uint32_t *)arg);
	while (1) {
		j += valor ;
		task_delay_s(1);
		//semaphore_give(&sem_task2);
		k++;
	}
}

int main(void)
{
	struct task_block *task_block1;
	struct task_block *task_block2;
	struct task_block *task_block3;

	uint32_t valor = 32;
	led_init();
	uart_tx_init();
	button_init();
//	os_init();
//	task_block1 = task_create("tarea1", task1, NULL,1);
//	task_block2 = task_create("tarea2", task2, NULL,2);
//	task_block3 = task_create("tarea3", task3, (void *)&valor,3);
//	queue_init(&queue_task1, sizeof(uint32_t));
    /* Loop forever */

	while (1) {
	}
}

