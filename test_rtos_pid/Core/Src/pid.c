/*
 * pid.c
 *
 *  Created on: Apr 9, 2023
 *      Author: jonathancagua
 */

#include "arm_math.h"
#include "pid.h"
/* Choose PID parameters */
#define PID_PARAM_KP        1U            /* Proporcional */
#define PID_PARAM_KI        0U        /* Integral */
#define PID_PARAM_KD        0U            /* Derivative */


float32_t lim_max = 1.6;
float32_t lim_min = -1.65;
uint32_t lim_size = 1;
// Declaración de variables globales
float32_t error, output;
float32_t set_point;
arm_pid_instance_f32 PID;

float32_t read_sensor(){
	float32_t value = 0;
	return value;
}

void write_actuator(float32_t output){

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

    /* Initialize PID system, float32_t format */
    arm_pid_init_f32(&PID, 1);

    // Bucle de control
    while (1)
    {
        // Leer el valor del sensor de la variable "input"
        float32_t input = read_sensor();

        // Calcular el error de seguimiento
        error = set_point - input;

        // Actualizar la salida del controlador PID
        //arm_pid_f32(&PID, error, &output);
        output = arm_pid_f32(&PID, error);

        // Limitar la salida del controlador a un rango específico
        arm_max_f32(&output,1, &lim_max , &lim_size);
        arm_min_f32(&output,1, &lim_min , &lim_size);

        // Escribir la salida del controlador en el actuador
        write_actuator(output);
    }
}

