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

uint32_t sp_antes, sp_durante, sp_despues;
uint32_t stackFrame[8];

int main(void)
{
	led_init();
	uart_tx_init();
	tick_init();
    /* Loop forever */
	while (1) {
		__ASM volatile ("mrs %[sp_antes], MSP" : [sp_antes] "=r" (sp_antes));
		__WFI();
		__ASM volatile ("mrs %[sp_despues], MSP" : [sp_despues] "=r" (sp_despues));
	}
}

