/*
 * 1_app.c
 *
 *  Created on: Jul 16, 2022
 *      Author: dev_fw
 */
#include "arm_math.h"
#include "arm_const_structs.h"
#include "1_app.h"
#include "adc.h"
#include "usart.h"
#define NUCLEO_CLOCK_SPEED 80000000U
struct header_struct {
   char     head[4];
   uint32_t id;
   uint16_t N;
   uint16_t fs ;
   char     tail[4];
} header={"head",0,256,5000,"tail"};

uint32_t cyclesCounterRead( void );
void cyclesCounterReset( void );
void cyclesCounterInit( void );
uint32_t adcRead();

void trigger(int16_t threshold)
{
   while((adcRead()-512)>threshold)
      ;
   while((adcRead()-512)<threshold)
      ;
   return;
}

void app1_main(){
	uint16_t sample = 0;
	//uint32_t muestra_inicial;
	arm_cfft_instance_q15 CS;
	int16_t adc [ header.N ];
	cyclesCounterInit();
	while(1) {
	  cyclesCounterReset();
	  adc[sample] = (int16_t )adcRead()-512;                        // va de -512 a 511
	  HAL_UART_Transmit(&huart1, (uint8_t* )&adc[sample], sizeof(adc[0]), 1);
	  //muestra_inicial = DWT->CTRL ;//HAL_GetTick();
	  if ( ++sample==header.N ) {
		sample = 0;
		//trigger(2);
		header.id++;
		HAL_UART_Transmit(&huart1, (uint8_t* )&header, sizeof(header ), 0xffff);
		adcRead(); //why?? hay algun efecto minimo en el 1er sample.. puede ser por el blinkeo de los leds o algo que me corre 10 puntos el primer sample. Con esto se resuelve.. habria que investigar el problema en detalle
	  }
	  while(cyclesCounterRead() < NUCLEO_CLOCK_SPEED/header.fs) // el clk de la nucleo es 80M
	    ;
	}

}
//---------------SACADO DE LA EDU CIAA-------------

uint32_t cyclesCounterRead( void ){
   return DWT->CYCCNT;
}

void cyclesCounterReset( void ){
   //resetea el contador de ciclos de clock
	DWT->CYCCNT = 0;
}

void cyclesCounterInit( void ){
   //resetea el contador de ciclos de clock
	DWT->CTRL |= 1;
}

uint32_t adcRead()
{
  uint32_t ADCValue = 0;
  HAL_ADC_Start(&hadc2);
  if (HAL_ADC_PollForConversion(&hadc2, 1000000) == HAL_OK) {
      ADCValue = HAL_ADC_GetValue(&hadc2);
  }
  HAL_ADC_Stop(&hadc2);

  return ADCValue;
}
