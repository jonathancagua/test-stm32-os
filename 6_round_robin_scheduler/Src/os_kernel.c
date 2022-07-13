/*
 * os_kernel.c
 *
 *  Created on: Jul 9, 2022
 *      Author: jonathancagua
 */
#include "os_kernel.h"

#define NBR_THREADS			3U
#define STACKSIZE			100U
#define BUS_FREQ			4000000

#define CTRL_ENABLE			(1U<<0)
#define CTRL_TICK_INT		(1U<<1)
#define CTRL_CLCK_SRC		(1U<<2)
#define CTRL_COUNT_FLAG		(1U<<16)
#define SYSTICK_RST			0U
//Thread control block
struct tcb{
	int32_t 	*stackPt;	//puntero a stck
	struct tcb 	*nextPt;	// siguientes thread
};
// POSICION DEL PSR STACK ES EN LA POSICION MAXIMA MENOS UNO
enum statck_frame_pos{ r4_pos= STACKSIZE-16, r5_pos, r6_pos ,r7_pos, r8_pos, r9_pos, r10_pos ,r11_pos ,
					   r0_pos, r1_pos, r2_pos, r3_pos, r12_pos, lr_pos, pc_pos, psr_pos};
typedef struct tcb tcb_t;
uint32_t 	MILLIS_PRESCALER;
tcb_t 		*currentPt;
tcb_t 		tcbs[NBR_THREADS];
//este stack es reservado
int32_t 	tcb_stack_a[NBR_THREADS][STACKSIZE];//tcb stack
void os_scheduler_launch(void);

void os_kernel_stack_init(int i)
{
	tcbs[i].stackPt =  &tcb_stack_a[i][r4_pos]; //Stack Pointer
	//Seteo del bit21 (T-bit) ENn PSR  se lo setea a uno en thumb modo
	//The Cortex-M4 Processor, Figure 2-3 Exception stack frame and table 2-2 Core register set summary
	tcb_stack_a[i][psr_pos] =  (1U<<24); //PSR
	//con contenido dummy solo para debug
	tcb_stack_a[i][lr_pos]  = 0x55555555;
	tcb_stack_a[i][r12_pos] = 0x55555555;
	tcb_stack_a[i][r3_pos]  = 0x55555555;
	tcb_stack_a[i][r2_pos]  = 0x55555555;
	tcb_stack_a[i][r1_pos]  = 0x55555555;
	tcb_stack_a[i][r0_pos]  = 0x55555555;

	tcb_stack_a[i][r11_pos] = 0x55555555;
	tcb_stack_a[i][r10_pos] = 0x55555555;
	tcb_stack_a[i][r9_pos]  = 0x55555555;
	tcb_stack_a[i][r8_pos]  = 0x55555555;
	tcb_stack_a[i][r7_pos]  = 0x55555555;
	tcb_stack_a[i][r6_pos]  = 0x55555555;
	tcb_stack_a[i][r5_pos]  = 0x55555555;
	tcb_stack_a[i][r4_pos]  = 0x55555555;
}

uint8_t os_kernel_add_threads(void(*task_0)(void),void(*task_1)(void),void(*task_2)(void))
{
	__disable_irq();
	for(int i=0;i<NBR_THREADS;i++){
		tcbs[i].nextPt =  &tcbs[(i+1) % NBR_THREADS];//se enlaza con el sig thread
		os_kernel_stack_init(i);//se iniciliza el stack para el thread
	}
	/*iniciliza PC*/
	tcb_stack_a[0][pc_pos] =  (int32_t)(task_0);
	tcb_stack_a[1][pc_pos] =  (int32_t)(task_1);
	tcb_stack_a[2][pc_pos] =  (int32_t)(task_2);
	/*comienzo del thread*/
	currentPt  = &tcbs[0];
	__enable_irq();
}

void os_kernel_init(void)
{
	MILLIS_PRESCALER  = (BUS_FREQ/1000);
}

void os_kernel_launch(uint32_t quanta)
{
	SysTick->CTRL = SYSTICK_RST;		//reseteamos el systick
	SysTick->VAL  = 0;					//Limpiar el Systick
	SysTick->LOAD = (quanta * MILLIS_PRESCALER) - 1;//cargamos el quanta para definir el max
	NVIC_SetPriority(SysTick_IRQn,15);	// seteamos la mas baja prioridad ya que el rtos debe
										// dar privilegio a las otras interrup

	SysTick->CTRL  = CTRL_CLCK_SRC;		//seleccionar el clock interno
	SysTick->CTRL  |= CTRL_ENABLE;		//activar el systick
	SysTick->CTRL  |= CTRL_TICK_INT;	//activar interrupcion
	os_scheduler_launch();
}
/**
 * Cuando ocurra la exceptio entonces son guardado en el stack
 * r0,r1,r2,r3,r12,lr,pc,psr
 */
__attribute__((naked)) void SysTick_Handler(void)
{
	//suspender el thread
	__asm("CPSID	I");				//deshabilitar las interrupciones globales
	__asm("PUSH  {R4-R11}");			//guardo r4,r5,r6,r7,r8,r9,r10,11
	__asm("LDR R0, =currentPt");		//carga la direccion de comienzo de thread dentro de R0
	__asm("LDR R1,[R0]");				// cargo en R1 la direccion de R0, R1 = currentPt
	__asm("STR SP,[R1]");				//Almaceno Cortex-M SP en la dirección equivalente a R1
	// seleccionar el siguiente thread
	__asm("LDR R1,[R1,#4]");			//Cargar r1 desde una ubicación 4bytes por
    									//encima de la dirección r1, i.e r1 = currentPt->next
	__asm("STR	R1,[R0]");				//Almacenar r1 en la dirección igual a r0, currentPt =  r1
	__asm("LDR SP,[R1]");				//cargar Cortex-M SP de la dirección es igual a r1,
										//SP =  currentPt->stackPt
	__asm("POP {R4-R11}");				//Se restaura r4,r5,r6,r7,r8,r9,r10,11
	__asm("CPSIE	I");				//Activar las interrupciones globales
	__asm("BX	LR");					//Volver de la excepción y restaurar r0,r1,r2,r3,r12,lr,pc,psr
}
void os_scheduler_launch(void)
{
	__asm("LDR R0,=currentPt");			//Cargar la dirección de currentPt en R0
	__asm("LDR R2,[r0]");				//Cargar en r2 desde la dirección r0, r2 =currentPt
	__asm("LDR  SP,[R2]");				//Cargar en SP la direccion R2,SP = currentPt->stackPt
	__asm("POP {R4-R11}");				//Se restaura r4,r5,r6,r7,r8,r9,r10,11
	__asm("POP {R12}");					//restaurar r12
	__asm("POP	{R0-R3}");				//Se restaura r0,r1,r2,r3

	__asm("ADD  SP,SP,#4");				//salto LR
	__asm("POP {LR}");					//Cree una nueva ubicación de popping LR
	__asm("ADD  SP,SP,#4");				//Salto el PSR sumando 4 a SP
	__asm("CPSIE	I");				//Activar las interrupciones globales
	__asm("BX	LR");					//Volver de la excepción y restaurar r0,r1,r2,r3,r12,lr,pc,psr
}

