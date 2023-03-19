/*
 * app_adc.c
 *
 *  Created on: Mar 19, 2023
 *      Author: jonathancagua
 */


#include "app_adc.h"
#include "adc.h"
#include "dac.h"

void dacWrite(uint16_t value){

	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
}

uint32_t adcRead(ADC_HandleTypeDef *ptr_hadc)
{
  uint32_t ADCValue = 0;
  HAL_ADC_Start(ptr_hadc);
  if (HAL_ADC_PollForConversion(ptr_hadc, 1000000) == HAL_OK) {
      ADCValue = HAL_ADC_GetValue(ptr_hadc);
  }
  HAL_ADC_Stop(ptr_hadc);

  return ADCValue;
}

