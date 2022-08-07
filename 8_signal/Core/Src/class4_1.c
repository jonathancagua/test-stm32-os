/*
 * 1_app.c
 *
 *  Created on: Jul 16, 2022
 *      Author: dev_fw
 */
#include "arm_math.h"
#include "arm_const_structs.h"
#include "class4_1.h"
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


static uint32_t cyclesCounterRead( void );
static void cyclesCounterReset( void );
static void cyclesCounterInit( void );
static uint32_t adcRead();
static void dacWrite(uint16_t value);

static void trigger(int16_t threshold)
{
   while((adcRead()-512)>threshold)
      ;
   while((adcRead()-512)<threshold)
      ;
   return;
}
//void init_cfft_instance(arm_cfft_instance_q15* CS,int length)/*{{{*/
//{
//   switch(length){
//      case 16:
//         *CS=arm_cfft_sR_q15_len16;
//         break;
//      case 32:
//         *CS=arm_cfft_sR_q15_len32;
//         break;
//      case 64:
//         *CS=arm_cfft_sR_q15_len64;
//         break;
//      case 128:
//         *CS=arm_cfft_sR_q15_len128;
//         break;
//      case 256:
//         *CS=arm_cfft_sR_q15_len256;
//         break;
//      case 512:
//         *CS=arm_cfft_sR_q15_len512;
//         break;
//      case 1024:
//         *CS=arm_cfft_sR_q15_len1024;
//         break;
//      case 2048:
//         *CS=arm_cfft_sR_q15_len2048;
//         break;
//      case 4096:
//         *CS=arm_cfft_sR_q15_len4096;
//         break;
//      default:
//         *CS=arm_cfft_sR_q15_len128;
//   }
//   return;
//}

void class1_main(){
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

static uint32_t cyclesCounterRead( void ){
   return DWT->CYCCNT;
}

static void cyclesCounterReset( void ){
   //resetea el contador de ciclos de clock
	DWT->CYCCNT = 0;
}

static void cyclesCounterInit( void ){
   //resetea el contador de ciclos de clock
	DWT->CTRL |= 1;
}

static void dacWrite(uint16_t value){

	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
}

static uint32_t adcRead()
{
  uint32_t ADCValue = 0;
  HAL_ADC_Start(&hadc2);
  if (HAL_ADC_PollForConversion(&hadc2, 1000000) == HAL_OK) {
      ADCValue = HAL_ADC_GetValue(&hadc2);
  }
  HAL_ADC_Stop(&hadc2);

  return ADCValue;
}
