/*
 * button.h
 *
 *  Created on: Aug 12, 2022
 *      Author: jonathancagua
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32l4xx.h"
#define PIN					13U			//se selcciona el pin
#define BUTTON_PIN			(1U<<PIN)	//se desplaza para selccion el pin
void button_init(void);

#endif /* BUTTON_H_ */
