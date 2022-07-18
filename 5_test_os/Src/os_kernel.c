/*
 * os_kernel.c
 *
 *  Created on: Jul 14, 2022
 *      Author: dev_fw
 */

#include "os_kernel.h"
#include "stm32l4xx.h"

#define ONE_SEC_LOAD		4000000//Startup clock ES DE 4mhz LO DICE EL DATA
#define CTRL_ENABLE			(1U<<0)
#define CTRL_TICK_INT		(1U<<1)
#define CTRL_CLCK_SRC		(1U<<2)
#define CTRL_COUNT_FLAG		(1U<<16)

#define MAX_TASKS			3U
#define STACKSIZE			100U

#define TASK_WAITING 		0U
#define TASK_READY   		1U
#define TASK_RUNNING 		2U
//este stack es reservado
uint32_t 			tcb_stack_a[MAX_TASKS][STACKSIZE];//tcb stack
static int 			n_tasks = 1;
static struct 		task_block TASKS[MAX_TASKS];

struct task_block 	*task_list_active = NULL;
struct stack_frame {
    uint32_t r0, r1, r2, r3, r12, lr, pc, xpsr;
};
#define stack_frame_size  sizeof(struct stack_frame) / sizeof(uint32_t)
struct extra_frame {
    uint32_t r4, r5, r6, r7, r8, r9, r10, r11,lr_prev_value;
};
#define extra_frame_size  sizeof(struct extra_frame) / sizeof(uint32_t)

void task_terminated(void)
{
    while(1) ;;
}

static void task_stack_init(struct task_block *t)
{
    struct stack_frame *tf;
    struct extra_frame *exf;
    t->sp -= stack_frame_size;
    tf = (struct stack_frame *)(t->sp);
    tf->r0 = (uint32_t) t->arg;
    tf->pc = (uint32_t) t->start;
    tf->lr = (uint32_t) task_terminated ;
    tf->xpsr =  (1 << 24);
    t->sp -= extra_frame_size;
    exf = (struct extra_frame *)(t->sp);
    exf->lr_prev_value = 0xFFFFFFF9;
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

    if (n_tasks > MAX_TASKS) return NULL;
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

//=========================PARA MANEJO DE SWITCH=======================
static void os_tick_init(void)
{
	SysTick->LOAD =  ONE_SEC_LOAD - 1;//Cargar el numero de ciclos por segundo
	SysTick->VAL  = 0;//Limpiar el Systick
	SysTick->CTRL  = CTRL_CLCK_SRC;//seleccionar el clock interno
	SysTick->CTRL  |= CTRL_TICK_INT;//activar interrupcion
	SysTick->CTRL  |= CTRL_ENABLE;//activar el systick
	__enable_irq();//activar interrrupcion global
}

void os_init(void){
	os_tick_init();
	/*
	 * Todas las interrupciones tienen prioridad 0 (la maxima) al iniciar la ejecucion. Para que
	 * no se de la condicion de fault mencionada en la teoria, debemos bajar su prioridad en el
	 * NVIC. La cuenta matematica que se observa da la probabilidad mas baja posible.
	 */
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);
}

void SysTick_Handler(void)
{
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
	/**
	 * Instruction Synchronization Barrier; flushes the pipeline and ensures that
	 * all previous instructions are completed before executing new instructions
	 */
	__ISB();

	/**
	 * Data Synchronization Barrier; ensures that all memory accesses are
	 * completed before next instruction is executed
	 */
	__DSB();
}

uint32_t get_next_context(uint32_t sp_actual)  {

	static int run_task_id = -1;//task que esta corriendo

	if(run_task_id >=0){
		TASKS[run_task_id].state = TASK_READY;
		TASKS[run_task_id].sp = sp_actual;
	}
	run_task_id++;
	if (run_task_id >= (n_tasks - 1))
		run_task_id = 0;
	TASKS[run_task_id].state = TASK_RUNNING;
	return TASKS[run_task_id].sp;
}
