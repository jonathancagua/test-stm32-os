/*
 * button.c
 *
 *  Created on: Aug 12, 2022
 *      Author: jonathancagua
 */
#include "button.h"

#define PIN					13U			//se selcciona el pin
#define MODE_REG			2U			//para usar con el registro de set
#define GPIOCEN				(1U<<2)		//para activar el port c
#define BUTTON_PIN			(1U<<PIN)	//se desplaza para selccion el pin
volatile uint32_t 			button_presses = 0;

void button_init(void)
{
	//pc13
	//EXTI13[3:0] 0010: PC[13] pin
	RCC->AHB2ENR  	|= RCC_AHB2ENR_GPIOCEN;//AHB2ENR
	RCC->APB2ENR 	|= RCC_APB2ENR_SYSCFGEN;
	GPIOC->MODER 	&= ~(0x03<<(PIN*MODE_REG));
	SYSCFG->EXTICR[3] &= (uint32_t) ~(0XFFFFFFFF);
	SYSCFG->EXTICR[3]|=(uint32_t) (SYSCFG_EXTICR4_EXTI13_PC);

	EXTI->IMR1 		|= (uint32_t) (BUTTON_PIN);
	EXTI->RTSR1 	|= (uint32_t) (BUTTON_PIN);
	EXTI->FTSR1 	&= ~(BUTTON_PIN);
	EXTI->PR1 		= (BUTTON_PIN);

	NVIC_SetPriority(EXTI15_10_IRQn,0);
	NVIC_EnableIRQ(EXTI15_10_IRQn);

}



void EXTI15_10_IRQHandler(void)
{
	EXTI->PR1 = (uint32_t) (BUTTON_PIN);
	button_presses++;
}
