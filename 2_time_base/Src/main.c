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
#include "timebase.h"
int main(void)
{
	led_init();
	uart_tx_init();
	tick_init();
    /* Loop forever */
	for(;;){
		tick_delay(1);
		printf("pasando un segundo!!\n\r");

	}
}
