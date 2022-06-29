#include "led.h"

#define PIN				14U
#define MODE_REG		2U
#define GPIOBEN			(1U<<1)//  0b 0000 0000 0000 0000 0000 0000 0000 0001
#define LED_PIN			(1U<<PIN)

void led_init(void)
{
	/*Enable clock access to led port (Port b)*/
	RCC->AHB2ENR  |= GPIOBEN;

	/*Set led pin as output pin*/
	GPIOB->MODER |=(1U<<(PIN*MODE_REG));
	GPIOB->MODER &= ~(1U<<((PIN*MODE_REG)+1));
}

void led_on(void)
{
	//set led pin en high (GPIOB 14)
	GPIOB->ODR |= LED_PIN;
}

void led_off(void)
{
	//set led pin en low (GPIOB 14)
	GPIOB->ODR &= ~LED_PIN;
}
