/*
 * app_adc.h
 *
 *  Created on: Mar 19, 2023
 *      Author: jonathancagua
 */

#ifndef INC_APP_ADC_H_
#define INC_APP_ADC_H_
#include "adc.h"
#include "dac.h"

void dacWrite(uint16_t value);
uint32_t adcRead(ADC_HandleTypeDef *ptr_hadc);

#endif /* INC_APP_ADC_H_ */
