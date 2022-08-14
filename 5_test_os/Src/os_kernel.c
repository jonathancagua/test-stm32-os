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
#define TASK_BLOCKED 		0U                          // estado de waiting de la tarea
#define TASK_READY   		1U                          // estado de ready de la tarea y se pone cada 
                                                        // vez que se saca del switching a la tarea.
#define TASK_RUNNING 		2U                          // estado de running de la tarea esto es cuando se esta ejecutadno
#define PRIO_MAX  			4U
#define schedule()			SCB->ICSR = SCB_ICSR_PENDSVSET_Msk
//este stack es reservado
uint32_t 					tcb_stack_a[MAX_TASKS+TASK_IDLE][STACKSIZE];  // tcb stack es la memoria que usa el stack en ram.
static int 					n_tasks = 1;                        // es usado como id de la tarea.
static struct 				task_block TASKS[MAX_TASKS+TASK_IDLE];        // bloque de variables usadas.
#define idle_index			0U
#define idle 				TASKS[0]
#define queue_len_validate(S) (S->write_index + 1 <= S->data_max)? true:false
#define queue_empty(q) (q->idx_write == q->idx_read)? true:false
#define queue_full(q) ( (q->idx_write + 1) % q->data_max == q->idx_read) ? true:false

struct task_block 			*task_list_active[PRIO_MAX] = { };	// puntero de las listas activas.
struct task_block 			*task_list_block[PRIO_MAX] = { };	// puntero de las listas bloqueadas.
static struct task_block 	*t_cur = NULL;

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
 * @brief tarea  usada cuando todas esten bloqueadas. no debe ser llamada por usuario para hacer una tarea
 * 
 */
void __attribute__((weak)) task_idle(void)  {
	while(1)  {
		__WFI();
	}
}
/**
 * @brief usado para debug de errores del os
 *
 * @param caller		Puntero a la funcion donde fue llamado errorHook. Implementado solo a
 *  					fines de trazabilidad de errores
 */
void __attribute__((weak)) os_error(void *caller)  {
	while(1);
}
/**
 * @brief va ser usada en tick timer
 *
 * @warning	Esta funcion debe ser lo mas corta posible porque se ejecuta dentro del handler
 *   			mencionado, por lo que tiene prioridad sobre el cambio de contexto y otras IRQ.
 *
 * @warning 	Esta funcion no debe bajo ninguna circunstancia utilizar APIs del OS dado
 *  			que podria dar lugar a un nuevo scheduling.
 */
void __attribute__((weak)) task_tick(void)  {
	__asm volatile( "nop" );
}

/**
 * @brief task terminada con loop infinito, no se deberia entrar en este estado
 * 
 */
void __attribute__((weak)) task_terminated(void)
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
/**
 * @brief Se agrega el puntero de task_block al puntero de la lista de tareas
 * 
 * @param list la lista de task_block. 
 * @param el la task_block que se va agregar.
 */
static void task_list_add(struct task_block **list, struct task_block *el)
{
    el->next = *list;
    *list = el;
}
/**
 * @brief funcion usada para agregar la tarea a las listas activas.
 * 
 * @param el la tarea que se va agregar.
 */
static void task_list_add_active(struct task_block *el)
{
    task_list_add(&task_list_active[el->priority], el);
}
/**
 * @brief funcion para agregar a la tarea a la lista de bloqueadas
 * 
 * @param el la tarea a bloquear
 */
static void task_list_add_block(struct task_block *el)
{
    task_list_add(&task_list_block[el->priority], el);
}
/**
 * @brief funcion para borrar la tarea de la lista
 * 
 * @param list lista de tareas
 * @param act_del tarea a eliminar
 * @return int -1 si es q no encuentra la tarea
 */
static int task_list_del(struct task_block **list, struct task_block *act_del)
{
    struct task_block *t = *list;
    struct task_block *p = NULL;
    while (t) {
        if (t == act_del) {
            if (p == NULL)
                *list = t->next;
            else
                p->next = t->next;
            return 0;
        }
        p = t;
        t = t->next;
    }
    return -1;
}

/**
 * @brief se borra una tarea de las listas activas
 * 
 * @param task_to_desact tarea a borrar de las activas.
 * @return int 
 */
static int task_list_del_active(struct task_block *task_to_desact)
{
    return task_list_del(&task_list_active[task_to_desact->priority], task_to_desact);
}
/**
 * @brief borra una tarea de la lista de bloqueadas
 * 
 * @param task_to_act tarea a eliminar de bloqueadas
 * @return int retorna -1 si es q no encuentra la tarea
 */
static int task_list_del_block(struct task_block *task_to_act)
{
    return task_list_del(&task_list_block[task_to_act->priority], task_to_act);
}
/**
 * @brief recibe la tarea a bloquear y cambia su estado
 * 
 * @param t la tarea a bloquear.
 */
static void task_blocking(struct task_block *t)
{
    if (task_list_del_active(t) == 0) {
    	task_list_add_block(t);
        t->state = TASK_BLOCKED;
    }
}
/**
 * @brief cambia a ready la tarea y la borra de lista de bloqueadas. 
 * Activa la tarea
 * 
 * @param t la tarea a poner ready
 */
static void task_ready(struct task_block *t)
{
    if (task_list_del_block(t) == 0) {
    	task_list_add_active(t);
        t->state = TASK_READY;
    }
}
/**
 * @brief cuenta la cantidad de tareas activas
 * 
 * @return int retorna el numero de tareas
 */
static int task_active_n(){
	int cnt = 0;
	for (int i = PRIO_MAX-1; i > 0; i--) {
		if(task_list_active[i]) cnt++;
	}
	return cnt;
}

/**
 * @brief busca entre las tareas ready en las listas o
 * se retorna la del idle si es q ninguna tarea esta ready.
 * 
 * @param t se busca al que sigue en la lista de prioridad
 * @return struct task_block* retorna al siguiente en la lista de prioridad.
 */
static inline struct task_block *task_list_next_ready(struct task_block *t)
{
	static int idx = PRIO_MAX;
	while(task_active_n()>0){
		idx--;
		if(idx<=0) idx = PRIO_MAX-1;
        if ((idx == t->priority) &&
                (t->next != NULL) &&
                (t->next->state == TASK_READY))
            return t->next;
        if (task_list_active[idx])
            return task_list_active[idx];
    }
    return task_list_active[idle_index];//retorno el kernel en caso que todas las tareas esten bloqueadas no exista tareas
}
/**
 * @brief usado para decrementar un tick a las tareas bloqueada.
 * 
 */
static void task_list_block_tick()
{
    for(int i=PRIO_MAX-1;i>=0;i--){
    	struct task_block *t = task_list_block[i];
    	if((t->start != NULL) && (t-> wakeup_time > 0)){
    		t->wakeup_time--;
    		if(t->wakeup_time <= 0){
    			task_ready(t);
    		}
    	}

    }
}

void queue_init(struct queue *queue_os, uint16_t data_size){
	queue_os->data_size = data_size;
	queue_os->idx_write = 0;
	queue_os->idx_read = 0;
	queue_os->task_queue = NULL;
	queue_os->data_max = QUEUE_SIZE/data_size;
}

void queue_write(struct queue* queue_os, void* data){
	//pasamos la tarea a un estado de desbloqueo, ya que estaba esperando dato de la cola
	if((queue_empty(queue_os)) && (queue_os->task_queue != NULL)){
		task_ready(queue_os->task_queue);
	}
	while((queue_os->idx_write + 1) % queue_os->data_max == queue_os->idx_read){
		task_blocking(t_cur);
		queue_os->task_queue = t_cur;
		schedule();
	}
	memcpy(queue_os->data+(queue_os->idx_write * queue_os->data_size),data,queue_os->data_size);
	queue_os->idx_write = (queue_os->idx_write + 1) % queue_os->data_max;
	queue_os->task_queue = NULL;
}

void queue_read(struct queue* queue_os, void* data){
	//pasamos la tarea a un estado de desbloqueo, ya que estaba esperando dato de la cola
	if((queue_full(queue_os)) && (queue_os->task_queue != NULL)){
		task_ready(queue_os->task_queue);
	}
	while(queue_os->idx_write  == queue_os->idx_read){
		task_blocking(t_cur);
		queue_os->task_queue = t_cur;
		schedule();
	}
	memcpy(data,queue_os->data+(queue_os->idx_read * queue_os->data_size),queue_os->data_size);
	queue_os->idx_read = (queue_os->idx_read + 1) % queue_os->data_max;
	queue_os->task_queue = NULL;

}

void semaphore_take(struct semaphore *sem){
	if(!sem)return;
	sem->task_sem = t_cur;
	sem->take = true;
	task_blocking(t_cur);
	schedule();
}

void semaphore_give(struct semaphore *sem){
	if(!sem)return;
	task_ready(sem->task_sem);
}
/**
 * @brief funcion de retardo y bloquea la tarea para dar paso a la sigueinte.
 * 
 * @param sec tiempo en segundo a estar bloqueada.
 */
void task_delay_s(int sec)
{
    if (sec < 1)
        return;
    t_cur->wakeup_time = sec;
    task_blocking(t_cur);
    schedule();
}
/**
 * @brief Creamos la tarea del rtos y asignamos stack para la tarea y configuramos los
 *        contextos y asignamos el stack a la tarea. la prioridad debe ser diferente para cada tarea.
 * 
 * @param name el nombre de la tarea.
 * @param start el puntero de la funcion.
 * @param arg el argumento de la funcion.
 * @param prio la prioridad de la tarea, la cual no se debe repetir. Cada prioridad es unica para cada tarea.
 * @return struct task_block* 
 */
struct task_block *task_create(char *name, void (*start)(void *arg), void *arg, int prio)
{
    struct task_block *t;
    int i;

    if (n_tasks > MAX_TASKS) return NULL;       // verificamos que no lleguemos al maximo de las tareas
    t = &TASKS[n_tasks];                    	// asigno al puntero la tarea que le toca
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
    t->sp = &tcb_stack_a[t->id][STACKSIZE]; 	// como el stack pointer es el maximo valor del stack lo asignamos y x eso se usa stacksize
    t->priority = prio;
    task_stack_init(t);                         // hacemos un init de los registro de respaldo.
    task_list_add_active(t);        			// agregamos a la lista la tarea.
    return t;
}
/**
 * @brief funcion para hacer un init de la tarea idle del os se la pone estado de running para no ejecutar en 
 * el switch context.
 * 
 */
static void kernel_task_init(){
    idle.name[0] = 0;
    idle.id = 0;
    idle.state = TASK_RUNNING;
    idle.wakeup_time = 0;
    idle.priority = 0;
    idle.start = task_idle;
    idle.sp = &tcb_stack_a[0][STACKSIZE];
    task_stack_init(&idle);
    task_list_add_active(&idle);
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
	SysTick->CTRL = CTRL_CLCK_SRC;         	// seleccionar el clock interno
	SysTick->CTRL |= CTRL_TICK_INT;        	// activar interrupcion
	SysTick->CTRL |= CTRL_ENABLE;          	// activar el systick
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
	kernel_task_init();						// init de la tarea idle
}
/**
 * @brief es el handler del tick, para ejecuatar se debe llama rprimero a la funcion os_tick_init
 * En cada interrupcion del tick se habilita el pendvset para el proceso de sitching.
 * 
 */
void SysTick_Handler(void)
{
	task_list_block_tick();
	schedule();
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
	if(t_cur){
		t_cur->state = TASK_READY;
		t_cur->sp = sp_actual;
	}
	t_cur = task_list_next_ready(t_cur);
	return t_cur->sp;
}
static void* isr_vector[FPU_IRQn];

bool os_irq_subscribe(IRQn_Type irq, void* ptr_func)  {
	bool resp = 0;
	if (isr_vector[irq] == NULL) {
		isr_vector[irq] = ptr_func;
		NVIC_ClearPendingIRQ(irq);
		NVIC_EnableIRQ(irq);
		resp = true;
	}
	return resp;
}

static void os_irq_handler(IRQn_Type IRQn){
	void (*funcion_usuario)(void);
	funcion_usuario = isr_vector[IRQn];
	NVIC_ClearPendingIRQ(IRQn);
	funcion_usuario();

}

void RCC_IRQHandler  (void){ os_irq_handler(RCC_IRQn);}		/* RCC global interrupt                                                */
void EXTI0_IRQHandler(void){ os_irq_handler(EXTI0_IRQn);}		/* EXTI Line 0 interrupt                                               */
void EXTI1_IRQHandler(void){ os_irq_handler(EXTI1_IRQn);}		/* EXTI Line 1 interrupt                                               */
void EXTI2_IRQHandler(void){ os_irq_handler(EXTI2_IRQn);}		/* EXTI Line 2 interrupt                                               */
void EXTI3_IRQHandler(void){ os_irq_handler(EXTI3_IRQn);}		/* EXTI Line 3 interrupt                                               */
void EXTI4_IRQHandler(void){ os_irq_handler(EXTI4_IRQn);}		/* EXTI Line4 interrupt                                                */
void EXTI15_10_IRQHandler(void){os_irq_handler(EXTI15_10_IRQn);}// EXTI Lines 10 to 15 interrupts


