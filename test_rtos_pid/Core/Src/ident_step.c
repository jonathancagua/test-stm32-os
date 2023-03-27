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
#define tak_freq 100
#define onda_cuadrada
static int16_t dacValue = 0;

static void task_generador(void *taskParmPtr) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    for (;;) {
#ifdef onda_cuadrada
    	dacValue ^= 0xFFFF;
    	dacValue &= 0xFFFF;
    	dacWrite(dacValue);
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000 / tak_freq));
#else
    	dacValue = rand() % 1024;
        dacWrite(dacValue);
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000 / tak_freq));
#endif
    }
}


static void task_identification(void *taskParmPtr) {
    static uint32_t sample;
    TickType_t lastWakeTime = xTaskGetTickCount();
    for (;;) {
    	uint32_t sample = adcRead(&hadc1);
    	printf ("%d / %d \r\n", ((uint16_t)sample), ((uint16_t)dacValue));
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000 / sample_freq));
    }
}


void init_ident_step(void) {

    xTaskCreate(task_generador, "task_generador", configMINIMAL_STACK_SIZE, NULL,tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(task_identification, "task_identification",configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

}
