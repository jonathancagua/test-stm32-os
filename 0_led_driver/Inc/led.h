/**
 * @file led.h
 * @author Jonathan Cagua(jonathan.cagua@gmail.com)
 * @brief Header y definicion de prototipos
 * @version 0.1
 * @date 2022-06-29
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef LED_H_
#define LED_H_

#include "stm32l4xx.h"
/**
 * @brief Se inicializa el portb pin14 con sus registro del cmsis
 * 
 */
void led_init(void);
/**
 * @brief se setea en alto el pint 14 portb
 * 
 */
void led_on(void);
/**
 * @brief se setea en bajo el pint 14 portb
 * 
 */
void led_off(void);
#endif /* LED_H_ */
