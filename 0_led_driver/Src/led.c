/**
 * @file led.c
 * @author Jonathan Cagua(jonathan.cagua@gmail.com)
 * @brief Test con el portb y pin 14
 * @version 0.1
 * @date 2022-06-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "led.h"

#define PIN				14U			//se selcciona el pin
#define MODE_REG		2U			//para usar con el registro de set
#define GPIOBEN			(1U<<1)		//para activar el port b
#define LED_PIN			(1U<<PIN)	//se desplaza para selccion el pin

void led_init(void)
{
	// Se habilita el acceso al port b con el clock
	RCC->AHB2ENR  |= GPIOBEN;
	// Se setea el pin como salida
	GPIOB->MODER |=(1U<<(PIN*MODE_REG));
	GPIOB->MODER &= ~(1U<<((PIN*MODE_REG)+1));
}

void led_on(void)
{
	GPIOB->ODR |= LED_PIN;
}

void led_off(void)
{
	GPIOB->ODR &= ~LED_PIN;
}
