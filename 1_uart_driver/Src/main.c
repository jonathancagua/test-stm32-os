/**
 * @file main.c
 * @author Jonathan Cagua(jonathan.cagua@gmail.com)
 * @brief testing de componentes
 * @version 0.1
 * @date 2022-07-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdint.h>
#include "led.h"
#include "uart.h"
int main(void)
{
	led_init();
	uart_tx_init();
    /* Loop forever */
	for(;;){
		led_on();
		for(int i=0;i<90000;i++);
		led_off();
		printf("Hello from STM32L4.........\n\r");
		for(int i=0;i<90000;i++);
	}
}
