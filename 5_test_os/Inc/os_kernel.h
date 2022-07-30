/*
 * os_kernel.h
 *
 *  Created on: Jul 14, 2022
 *      Author: dev_fw
 */

#ifndef OS_KERNEL_H_
#define OS_KERNEL_H_
#include <stdint.h>
#include <stddef.h>

#define TASK_NAME_MAX_LEN   16U                     // maximo d del len del nombre
#define MAX_TASKS			3U                      // maximo de tareas a usar.
#define TASK_IDLE			1U
#define STACKSIZE			100U                    // tamaño asignado al stack, y es quemado a cada tarea de 
                                                    // manera estatica y no dinamica.
struct task_block {
    uint32_t            *sp;					    // el stack pointer
    struct task_block   *next;		                // la tarea q sigue
    char                name[TASK_NAME_MAX_LEN];    /* donde se va almacenar el nombre de la tarea*/
    int                 id;							//para enumerar el numero de tareas dentro del array de tareas
	int                 state;						//para poner si esta ready o waiting
	void                (*start)(void *arg);		//siempre sera tak(void *argumento))
	void                *arg;						//el argumento
    uint32_t            wakeup_time;			    // el de despertar
    uint8_t 			priority;
};
struct task_block *task_create(char *name, void (*start)(void *arg), void *arg, int prio);
void os_init(void);
void task_delay_s(int sec);
#endif /* OS_KERNEL_H_ */
