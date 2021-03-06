/**
 * @file os_kernel.c
 * @author Jonathan Cagua (jonathan.cagua@gmail.com)
 * @brief kernel del sistema operativo
 * @version 0.1
 * @date 2022-07-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "os_kernel.h"
#include "stm32l4xx.h"

#define ONE_SEC_LOAD		4000000     //Startup clock ES DE 4mhz LO DICE EL DATA
#define CTRL_ENABLE			(1U<<0)
#define CTRL_TICK_INT		(1U<<1)
#define CTRL_CLCK_SRC		(1U<<2)
#define CTRL_COUNT_FLAG		(1U<<16)
#define TASK_WAITING 		0U                          // estado de waiting de la tarea
#define TASK_READY   		1U                          // estado de ready de la tarea y se pone cada 
                                                        // vez que se saca del switching a la tarea.
#define TASK_RUNNING 		2U                          // estado de running de la tarea esto es cuando se esta ejecutadno
//este stack es reservado
uint32_t 			tcb_stack_a[MAX_TASKS][STACKSIZE];  // tcb stack es la memoria que usa el stack en ram.
static int 			n_tasks = 1;                        // es usado como id de la tarea.
static struct 		task_block TASKS[MAX_TASKS];        // bloque de variables usadas.

struct task_block 	*task_list_active = NULL;           // puntero de las listas activas.
/**
 * @brief este es usado como el frame que se va respalda
 * 
 */
struct stack_frame {
    uint32_t r0, r1, r2, r3, r12, lr, pc, xpsr;
};
// sacamos el tamaño del stack frame
#define stack_frame_size  sizeof(struct stack_frame) / sizeof(uint32_t)
/**
 * @brief este es el extra frame con lr_pre_value
 * 
 */
struct extra_frame {
    uint32_t r4, r5, r6, r7, r8, r9, r10, r11,lr_prev_value;
};
// sacamos el tamaño del extra frame
#define extra_frame_size  sizeof(struct extra_frame) / sizeof(uint32_t)

/**
 * @brief task terminada con loop infinito.
 * 
 */
void task_terminated(void)
{
    while(1) ;;
}
/**
 * @brief funcion para hacer init del stack con sus respectiva configuracion.
 * 
 * @param t puntero a la tarea a inicilizar.
 */
static void task_stack_init(struct task_block *t)
{
    struct stack_frame *tf;                     // puntero al stack frame
    struct extra_frame *exf;                    // puntero al extra frame 
    t->sp -= stack_frame_size;                  // como stack pointer apunta al maximo se hace bajar hasta xpsr.
    tf = (struct stack_frame *)(t->sp);         // se saca copia del stack pointer
    tf->r0 = (uint32_t) t->arg;                 // en r0 se guarda el argumento
    tf->pc = (uint32_t) t->start;               // se guarda en pc el puntero a la funcion
    tf->lr = (uint32_t) task_terminated ;       // el looop cuando se termina tarea, esto no deberia pasar
    tf->xpsr =  (1 << 24);                      // se activa xPSR.T = 1
    t->sp -= extra_frame_size;                  // al stack pointer le bajamos ahora del extra frame.
    exf = (struct extra_frame *)(t->sp);        // puntero a extra frame
    exf->lr_prev_value = 0xFFFFFFF9;            // retornar a modo thread con MSP, FPU no utilizada
}

static void task_list_add(struct task_block **list, struct task_block *el)
{
    el->next = *list;
    *list = el;
}
/**
 * @brief Creamos la tarea del rtos y asignamos stack para la tarea y configuramos los
 *        contextos y asignamos el stack a la tarea.
 * 
 * @param name el nombre de la tarea.
 * @param start el puntero de la funcion.
 * @param arg el argumento de la funcion.
 * @return struct task_block* 
 */
struct task_block *task_create(char *name, void (*start)(void *arg), void *arg)
{
    struct task_block *t;
    int i;

    if (n_tasks > MAX_TASKS) return NULL;       // verificamos que no lleguemos al maximo de las tareas
    t = &TASKS[n_tasks - 1];                    // asigno al puntero la tarea que le toca
    t->id = n_tasks++;                          // incrementamos el contador de tarea.
    for (i = 0; i < TASK_NAME_MAX_LEN; i++) {   // codigo para asignar el nombre de la tarea.
        t->name[i] = name[i];
        if (name[i] == 0)
            break;
    }
    t->state = TASK_READY;                      // ponemos la tarea como ready
    t->start = start;                           // agregamos el puntero a la funcion
    t->arg = arg;                               // agregamos el argumento
    t->wakeup_time = 0;                         // seteamos en cero el wake up
    t->sp = &tcb_stack_a[(t->id)-1][STACKSIZE]; // como el stack pointer es el maximo valor del stack lo asignamos y x eso se usa stacksize
    task_stack_init(t);                         // hacemos un init de los registro de respaldo.
    task_list_add(&task_list_active, t);        // agregamos a la lista la tarea.
    return t;
}

//=========================PARA MANEJO DE SWITCH=======================
/**
 * @brief Init el tick del core y es configurado con el clock del arm
 * 
 */
static void os_tick_init(void)
{
	SysTick->LOAD =  ONE_SEC_LOAD - 1;      // Cargar el numero de ciclos por segundo
	SysTick->VAL  = 0;                      // Limpiar el Systick
	SysTick->CTRL  = CTRL_CLCK_SRC;         // seleccionar el clock interno
	SysTick->CTRL  |= CTRL_TICK_INT;        // activar interrupcion
	SysTick->CTRL  |= CTRL_ENABLE;          // activar el systick
	__enable_irq();                         // activar interrrupcion global
}
/**
 * @brief funcion para inicializar el sistema operativo la cual activa el tick y seteamos
 * la prioridad de pendsv 
 * 
 */
void os_init(void){
	os_tick_init();
	/*
	 * Todas las interrupciones tienen prioridad 0 (la maxima) al iniciar la ejecucion. Para que
	 * no se de la condicion de fault mencionada en la teoria, debemos bajar su prioridad en el
	 * NVIC. La cuenta matematica que se observa da la probabilidad mas baja posible.
	 */
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);
}
/**
 * @brief es el handler del tick, para ejecuatar se debe llama rprimero a la funcion os_tick_init
 * En cada interrupcion del tick se habilita el pendvset para el proceso de sitching.
 * 
 */
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
/**
 * @brief funcion usada para obtener el sigueinte stack pointer de la funcion y almacenar
 *        el actual stack pointer de la tarea que estaba funcionando.
 * 
 * @param sp_actual 
 * @return uint32_t el stack pointer de la siguiente tarea.
 */
uint32_t get_next_context(uint32_t sp_actual)  {

	static int run_task_id = -1;                // variable estatica para movernos entre el arreglo de tareas.

	if(run_task_id >=0){                        // vemos que la tarea este dentro del rango de tareas 
		TASKS[run_task_id].state = TASK_READY;  // ponemos la tarea actual que estaba corriendo en ready
		TASKS[run_task_id].sp = sp_actual;      // guardamos el stack pointer de la tarea que estaba corriendo
	}
	run_task_id++;                              // nos movemos a la siguiente tarea.
	if (run_task_id >= (n_tasks - 1))           // vemos si es que llegamos al limite
		run_task_id = 0;                        // iniciamos las tareas.
	TASKS[run_task_id].state = TASK_RUNNING;    // nos ponemos en la siguiete tarea y ponemos en estado de running
	return TASKS[run_task_id].sp;               // retornamos el stack pointer de donde se quedo la tarea que va ejecuatar ahora.
}
