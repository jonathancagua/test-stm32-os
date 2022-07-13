/*
 * os_kernel.h
 *
 *  Created on: Jul 9, 2022
 *      Author: jonathancagua
 */

#ifndef OS_KERNEL_H_
#define OS_KERNEL_H_
#include <stdint.h>
#include "stm32l4xx.h"

void os_kernel_init(void);
void os_kernel_launch(uint32_t quanta);
uint8_t os_kernel_add_threads(void(*task_0)(void),void(*task_1)(void),void(*task_2)(void));
#endif /* OS_KERNEL_H_ */
