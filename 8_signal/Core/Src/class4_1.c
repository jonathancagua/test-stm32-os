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
#define BITS    10   // cantidad de bits usado para cuantizar
#define CUTFREC 500 // frec de corte para filtrar en frec
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
void init_cfft_instance(arm_cfft_instance_q15* CS,int length)/*{{{*/
{
   switch(length){
      case 16:
         *CS=arm_cfft_sR_q15_len16;
         break;
      case 32:
         *CS=arm_cfft_sR_q15_len32;
         break;
      case 64:
         *CS=arm_cfft_sR_q15_len64;
         break;
      case 128:
         *CS=arm_cfft_sR_q15_len128;
         break;
      case 256:
         *CS=arm_cfft_sR_q15_len256;
         break;
      case 512:
         *CS=arm_cfft_sR_q15_len512;
         break;
      case 1024:
         *CS=arm_cfft_sR_q15_len1024;
         break;
      case 2048:
         *CS=arm_cfft_sR_q15_len2048;
         break;
      case 4096:
         *CS=arm_cfft_sR_q15_len4096;
         break;
      default:
         *CS=arm_cfft_sR_q15_len128;
   }
   return;
}

void clearFloatBuf(float *b,uint16_t len)
{
   while(len--)
      b[len]=0.0;
}

void class1_main(){
	uint16_t sample = 0;
	int16_t adc [ header.N ];
	arm_cfft_instance_q15 CS;
	q15_t fftInOut[ header.N *2 ];//
	float t;
	cyclesCounterInit();
	while(1) {
		cyclesCounterReset();
		HAL_UART_Transmit(&huart1, (uint8_t* )&adc[sample], sizeof(adc[0]), 1);
		HAL_UART_Transmit(&huart1, (uint8_t* )&fftInOut[sample*2],sizeof(fftInOut[0]), 1);
		adc[sample]       = (((int16_t )adcRead()-512)>>(10-BITS))<<(6+10-BITS);
		fftInOut[sample*2]   = adc[sample];
		fftInOut[sample*2+1] = 0;

		if ( ++sample==header.N ) {
			sample = 0;
			//------------TRANSFORMADA------------------
			init_cfft_instance ( &CS,header.N        );
			arm_cfft_q15       ( &CS ,fftInOut ,0 ,1 ); //0 directa, 1 inversa

			// FILTRADO BASICO RECORTANDO EN FREC
			fftInOut[0]=0; //elimino la continua
			fftInOut[1]=0;
			int cutBin=CUTFREC/(header.fs/header.N); //defino el n donde comenzar a cortar
			for(int i=0;i<=(header.N/2);i++) {        //solo recorro la mitad, porque la otra mitad es compleja conjugada asi que borro de los 2 lados
			  if(i>=cutBin ) {
				 fftInOut[i*2]                  = 0; //borro bin parte real
				 fftInOut[i*2+1]                = 0; //borro bin parte compleja
				 fftInOut[(header.N-1)*2-i*2]   = 0; //lo mismo pero de atras para adelante
				 fftInOut[(header.N-1)*2-i*2+1] = 0;
			  }
			}
			//---------ANTI transformada---------------------
			init_cfft_instance ( &CS,header.N        );
			arm_cfft_q15       ( &CS ,fftInOut ,1 ,1 );

			header.id++;
			HAL_UART_Transmit(&huart1, (uint8_t* )&header, sizeof(struct header_struct ), 1);

		}
		while(cyclesCounterRead() < NUCLEO_CLOCK_SPEED/header.fs) // el clk de la nucleo es 80M
		;
	}

}

void class2_main(){
	uint16_t sample = 0;
	int16_t adc [ header.N ];
	arm_cfft_instance_q15 CS;
	q15_t fftInOut[ header.N *2 ];//
	float coorIn [ header.N      ];// guarda copia de samples en Q15 como in para la fft.La fft corrompe los datos de la entrada!
	float corrOut[ header.N *2   ];
	float t;
	int i;
	float signal [ header.N      ];
	cyclesCounterInit();
	clearFloatBuf(signal,header.N);
	for(i=0;i<10;i++) {
	  signal[i]=-i/10.0;
	}
	for(;i<20;i++) {
	  signal[i]=(i-10)/10.0;
	}
	while(1) {
		cyclesCounterReset();
		int16_t adcRaw;
		HAL_UART_Transmit(&huart1, (uint8_t* )&adc[sample], sizeof(adc[0]), 1);// envia el sample ANTERIOR
		HAL_UART_Transmit(&huart1, (uint8_t* )&corrOut[sample*2], sizeof(corrOut[0]), 1);
		HAL_UART_Transmit(&huart1, (uint8_t* )&corrOut[sample*2+1], sizeof(corrOut[0]), 1);
		adcRaw        = adcRead()-512;
		adc[sample]   = adcRaw/512.0;             // PISA el sample que se acaba de mandar con una nueva muestra
		coorIn[sample] = adcRaw;


		if ( ++sample==header.N ) {
			clearFloatBuf ( corrOut,2*header.N );
			arm_correlate_f32 ( signal,header.N,adc ,header.N,corrOut  );
			// trigger(2);
			sample = 0;
			header.id++;
			HAL_UART_Transmit(&huart1, (uint8_t* )&header, sizeof(struct header_struct ), 1);

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
