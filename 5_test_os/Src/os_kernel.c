/*
 * os_kernel.c
 *
 *  Created on: Jul 14, 2022
 *      Author: dev_fw
 */

#include "os_kernel.h"

#define MAX_TASKS			3U
#define STACKSIZE			100U

#define TASK_WAITING 		0U
#define TASK_READY   		1U
#define TASK_RUNNING 		2U

//este stack es reservado
uint32_t 	tcb_stack_a[MAX_TASKS][STACKSIZE];//tcb stack
static int n_tasks = 1;
static struct task_block TASKS[MAX_TASKS];
struct task_block *task_list_active = NULL;
struct stack_frame {
    uint32_t r0, r1, r2, r3, r12, lr, pc, xpsr;
};
#define stack_frame_size  sizeof(struct stack_frame) / sizeof(uint32_t)
struct extra_frame {
    uint32_t r4, r5, r6, r7, r8, r9, r10, r11;
};
#define extra_frame_size  sizeof(struct extra_frame) / sizeof(uint32_t)
void task_terminated(void)
{
    while(1) ;;
}

static void task_stack_init(struct task_block *t)
{
    struct stack_frame *tf;
    t->sp -= stack_frame_size;
    tf = (struct stack_frame *)(t->sp);
    tf->r0 = (uint32_t) t->arg;
    tf->pc = (uint32_t) t->start;
    tf->lr = (uint32_t) task_terminated;
    tf->xpsr =  (1 << 24);
    t->sp -= extra_frame_size;
}

static void task_list_add(struct task_block **list, struct task_block *el)
{
    el->next = *list;
    *list = el;
}

struct task_block *task_create(char *name, void (*start)(void *arg), void *arg)
{
    struct task_block *t;
    int i;

    if (n_tasks >= MAX_TASKS) return NULL;
    t = &TASKS[n_tasks - 1];
    t->id = n_tasks++;
    for (i = 0; i < TASK_NAME_MAX_LEN; i++) {
        t->name[i] = name[i];
        if (name[i] == 0)
            break;
    }
    t->state = TASK_READY;
    t->start = start;
    t->arg = arg;
    t->wakeup_time = 0;
    t->sp = &tcb_stack_a[(t->id)-1][STACKSIZE];
    task_stack_init(t);
    task_list_add(&task_list_active, t);
    return t;
}
