/*
 * ident_step.c
 *
 *  Created on: Mar 26, 2023
 *      Author: jonathancagua
 */
#include "FreeRTOS.h"
#include "task.h"
#include "app_adc.h"
#define sample_freq 1000
#define tak_freq 5
#define tak_freq_prbs 100
#//define onda_cuadrada
static uint16_t dacValue = 0;
static uint32_t dac_convert = 0;
uint32_t sample_2 = 0;
static void task_generador(void *taskParmPtr) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    srand(time(NULL));
    for (;;) {
#ifdef onda_cuadrada
    	dacValue ^= 0xFFFF;
    	dacValue &= 0xFFFF;
    	dacWrite(dacValue);
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000 / tak_freq));
#else
    	dacValue = rand() % 65535;
    	dac_convert = (dacValue * 996)/65535;//996
        dacWrite(dacValue);
        sample_2 = adcRead(&hadc1);
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000 / tak_freq_prbs));
#endif
    }
}


static void task_identification(void *taskParmPtr) {
    static uint32_t sample;
    TickType_t lastWakeTime = xTaskGetTickCount();
    for (;;) {

#ifdef onda_cuadrada
    	uint32_t sample = adcRead(&hadc1);
    	printf ("%d / %d \r\n", ((uint16_t)sample), ((uint16_t)dacValue));
#else
    	printf ("%d / %d \r\n", ((uint16_t)sample_2), ((uint16_t)dac_convert));
#endif
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000 / sample_freq));
    }
}


void init_ident_step(void) {

    xTaskCreate(task_generador, "task_generador", configMINIMAL_STACK_SIZE, NULL,tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(task_identification, "task_identification",configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

}
