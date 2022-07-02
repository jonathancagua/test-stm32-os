/**
 * @file uart.c
 * @author Jonathan Cagua(jonathan.cagua@gmail.com)
 * @brief componente que solo escribe por print usnado weak
 * @version 0.1
 * @date 2022-07-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "stm32l4xx.h"

#define GPIOBEN			(1U<<1)		//para activar el port b
#define UART1_EN		(1U<<14)

#define SYS_FREQ		4000000//Startup clock ES DE 4mhz LO DICE EL DATA
#define APB2_CLK		SYS_FREQ //SETEO LA FRECUENCIA DEL BUS
#define UART_BAUDRATE	115200

#define CR1_TE			(1U<<3)
#define CR1_UE			(1U<<0)

#define ISR_TXE			(1U<<7)

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_write(int ch);
int __io_putchar(int ch)//weak
{
	uart_write(ch);
	return ch;
}

void uart_tx_init(void)
{
	//clock access para el periferico pinb6-7
	//TX PIN6
	RCC->AHB2ENR  |= GPIOBEN;//activar el puerto donde se encuentra los pines

	/*SE SETEA EL PIN6 DEL PORTB PARA FUNCION ALTERNA*/
	GPIOB->MODER &=~(1U<<12);
	GPIOB->MODER |=(1U<<13);

	/*Setea la alternate function tipo AF7 (UART1_TX)*/
	GPIOB->AFR[0] |=(1U<<24);
	GPIOB->AFR[0] |=(1U<<25);
	GPIOB->AFR[0] |=(1U<<26);
	GPIOB->AFR[0] &= ~(1U<<27);

	/*Enable clock access to UART*/
	RCC->APB2ENR |= UART1_EN;

	uart_set_baudrate(APB2_CLK,UART_BAUDRATE);

	//ENABLE TRASNFERENCIA
	 USART1->CR1 = CR1_TE;

	///Enable EL MODULO UART
	 USART1->CR1 |= CR1_UE;
}

static void uart_write(int ch)
{
	//estar seguro que se transmitio
	while(!(USART1->ISR & ISR_TXE)){}

	//escribo el data
	USART1->TDR =  (ch & 0XFF);
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART1->BRR = compute_uart_bd(periph_clk,baudrate);
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate/2U))/baudrate);
}
