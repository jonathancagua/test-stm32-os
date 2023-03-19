/*
 * pid_Task.c
 *
 *  Created on: Mar 15, 2023
 *      Author: dev_fw
 */

/*=====[Inclusion of own header]=============================================*/
#include <stdbool.h>
#include "pidTask.h"
#include "adc.h"
#include "dac.h"
#include "usart.h"
#include "app_adc.h"
/*=====[Inclusions of private function dependencies]=========================*/

#include "pid_controller.h"

/*=====[Definition macros of private constants]==============================*/

#define SCALE_Y         0.003225806f    // 3.3 / 1023     10-bit ADC to Voltage
#define SCALE_R         0.003225806f    // 3.3 / 1023     10-bit ADC to Voltage
#define SCALE_U         310.0f          // 1023 / 3.3     Voltage to 10-bit DAC

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

// Para calcular el tiempo que tarda el algoritmo y establecer un h minimo
//#define COUNT_CYCLES

// Task implementation
void pidControlTask( void* taskParmPtr )
{
   // Controller signals
   float r = 0.0f; // Measure of reference r[k]
   float y = 0.0f; // Measure of system output y[k]
   float u = 0.0f; // Calculated controller's output u[k]

   // h no puede ser menor ni al tiempo del algoritmo, y con va a ser un
   // multiplo del periodo de tick del RTOS
   uint32_t h_ms = 4; // Task periodicity (sample rate)
   float h_s = ((float)h_ms)/1000.0f;

   // Enable ADC/DAC
   //adcInit( ADC_ENABLE );
   //dacInit( DAC_ENABLE );

   // PID Controller structure (like an object instance)
   PIDController_t PsPIDController;

   // PID Controller Initialization
   pidInit( &PsPIDController,
            2.8f,                  // Kp
            1.2f / h_s,            // Ki
            0.1f * h_s,            // Kd
            h_s,                   // h en [s]
            20.0f,                 // N
            1.0f,                  // b
            0.0f,                  // u_min
            3.3f                   // u_max
          );

   // Peridodic task each h_ms
   portTickType xPeriodicity =  h_ms / portTICK_RATE_MS;
   portTickType xLastWakeTime = xTaskGetTickCount();

   #ifdef COUNT_CYCLES
      // Configura el contador de ciclos con el clock de la EDU-CIAA NXP
      cyclesCounterConfig(EDU_CIAA_NXP_CLOCK_SPEED);
      volatile uint32_t cyclesElapsed = 0;
   #endif

   while(true) {

      #ifdef COUNT_CYCLES
         // Resetea el contador de ciclos
         cyclesCounterReset();
      #endif

      // Leer salida y[k] y refererencia r[k]
      y = adcRead(&hadc1) * SCALE_Y;
      r = adcRead(&hadc2) * SCALE_R;

      // Calculate PID controller output u[k]
      u = pidCalculateControllerOutput( &PsPIDController, y, r ) * SCALE_U;

      // Actualizar la salida u[k]
      dacWrite( u);

      // Update PID controller for next iteration
      pidUpdateController( &PsPIDController, y, r );

      #ifdef COUNT_CYCLES
         // Leer conteco actual de ciclos
         cyclesElapsed = DWT_CYCCNT;
         volatile float us = cyclesCounterToUs(cyclesElapsed);
      #endif

      // Send the task to the locked state during xPeriodicity
      // (periodical delay)
      vTaskDelayUntil( &xLastWakeTime, xPeriodicity );
   }
}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/
