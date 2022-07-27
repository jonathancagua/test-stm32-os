/*
 * 1_app.c
 *
 *  Created on: Jul 16, 2022
 *      Author: dev_fw
 */
#include "arm_math.h"
//#include "arm_const_structs.h"
#include "3_app.h"
#include "adc.h"
#include "dac.h"
#include "usart.h"
#define NUCLEO_CLOCK_SPEED 80000000U
#define BITS 4 //10
uint32_t tick   = 0   ;
uint16_t tone   = 440 ;
uint16_t B      = 4000;
uint16_t sweept = 10;
//#define original
struct header_struct {
   char     head[4];
   uint32_t id;
   uint16_t N;
   uint16_t fs ;
   uint32_t maxIndex;
   uint32_t minIndex;
   q15_t    maxValue;
   q15_t    minValue;
   q15_t    rms;
   char     tail[4];
} header={"head",0,128,10000,0,0,0,0,0,"tail"};


uint32_t cyclesCounterRead( void );
void cyclesCounterReset( void );
void cyclesCounterInit( void );
uint32_t adcRead();
void dacWrite(uint16_t value);

void trigger(int16_t threshold)
{
   while((adcRead()-512)>threshold)
      ;
   while((adcRead()-512)<threshold)
      ;
   return;
}

void app3_main(){
	uint16_t sample = 0;
	int16_t adc [ header.N ];
	float t;
	cyclesCounterInit();
	while(1) {
		cyclesCounterReset();
		t =((tick)/(float)header.fs);
		tick++;
		q15_t q_sample = 512*arm_sin_f32(t*tone*2*PI);
		#ifdef original
		adc[sample] = q_sample << 6;
		#else
		adc[sample] = (((adcRead()-512))>>(10-BITS))<<(6+(10-BITS));
		#endif
		HAL_UART_Transmit(&huart1, (uint8_t* )&adc[sample], sizeof(adc[0]), 1);
		dacWrite(q_sample+512); // sweept
		if ( ++sample==header.N ) {
			arm_max_q15 ( adc, header.N, &header.maxValue,&header.maxIndex );
			arm_min_q15 ( adc, header.N, &header.minValue,&header.minIndex );
			arm_rms_q15 ( adc, header.N, &header.rms                       );
			header.id++;
			HAL_UART_Transmit(&huart1, (uint8_t* )&header, sizeof(header), 1);
			sample = 0;
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

void dacWrite(uint16_t value){

	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
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