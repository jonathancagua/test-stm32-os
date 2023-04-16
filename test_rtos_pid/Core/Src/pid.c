/*
 * pid.c
 *
 *  Created on: Apr 9, 2023
 *      Author: jonathancagua
 */
#include <stdbool.h>
#include "arm_math.h"
#include "pid.h"
#include "app_adc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
/* Choose PID parameters */
#define PID_PARAM_KP        0.613            /* Proporcional */
#define PID_PARAM_KI        0.119        	/* Integral */
#define PID_PARAM_KD        0.029            /* Derivative */


float32_t lim_max = 1;
float32_t lim_min = 0;
uint32_t lim_size = 1;
// Declaración de variables globales
float32_t error, output;
float32_t set_point;
arm_pid_instance_f32 PID;
static uint16_t dacValue = 0;
static uint32_t dac_convert = 0;
static uint32_t sample_2 = 0;
static bool enable_pid = true;

float32_t read_sensor(){
	float32_t value = 0;
	sample_2 = adcRead(&hadc1);

	value = (float)sample_2 / 996.0;
	return value;
}

void write_actuator(float32_t output){
	dacValue = output * 0xFFFF;
	dac_convert = (output * 996);
	dacWrite(dacValue);
}

// Función principal
int app_pid(void)
{
    /* ARM PID Instance, float_32 format */
    arm_pid_instance_f32 PID;
    // Inicializar el controlador PID con los parámetros de ganancia Kp, Ki y Kd
    /* Set PID parameters */
    /* Set this for your needs */
    PID.Kp = PID_PARAM_KP;        /* Proporcional */
    PID.Ki = PID_PARAM_KI;        /* Integral */
    PID.Kd = PID_PARAM_KD;        /* Derivative */
    TickType_t lastWakeTime = xTaskGetTickCount();
    /* Initialize PID system, float32_t format */
    arm_pid_init_f32(&PID, 1);
    set_point = 0.5;

    // Bucle de control
    while (1)
    {
        // Leer el valor del sensor de la variable "input"
        float32_t input = read_sensor();
        if(true == enable_pid){
			// Calcular el error de seguimiento
			error = set_point - input;

			// Actualizar la salida del controlador PID
			output = arm_pid_f32(&PID, error);

			if (output > lim_max) {
				output = lim_max;
			} else if (output < lim_min) {
				output = lim_min;
			}
        }
        // Escribir la salida del controlador en el actuador
        write_actuator(output);

        printf ("%d / %d \r\n", ((uint16_t)sample_2), ((uint16_t)dac_convert));
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000 / 1000));
    }
}

void vTimerCallback(TimerHandle_t xTimer) {
	if(true == enable_pid){
		output =0;
		enable_pid = false;
	}
	else{
		enable_pid = true;
	}

}
void init_pid(void) {
    xTaskCreate(app_pid, "task_identification",configMINIMAL_STACK_SIZE*4, NULL, tskIDLE_PRIORITY + 1, NULL);
    // Crea un temporizador con un tiempo de espera de 5 segundos y una repetición automática
    TimerHandle_t xTimer = xTimerCreate("Timer Example", pdMS_TO_TICKS(1000), pdTRUE, NULL, vTimerCallback);

    // Inicia el temporizador
    xTimerStart(xTimer, 0);
}
