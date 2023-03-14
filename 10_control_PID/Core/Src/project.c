/*
 * 1_app.c
 *
 *  Created on: Jul 16, 2022
 *      Author: dev_fw
 */
#include <stdio.h>
#include "arm_math.h"
#include "arm_const_structs.h"
#include "adc.h"
#include "dac.h"
#include "usart.h"
//#include "fir.h"
#define NUCLEO_CLOCK_SPEED 80000000U

#define BITS 10						// cantidad de bits usado para cuantizar
static uint32_t tick	= 0   ;
static uint16_t tone	= 100 ;
static uint16_t B		= 2500;
static uint16_t sweept = 5;

struct header_struct {
   char		pre[4];
   uint32_t id;
   uint16_t N;
   uint16_t fs ;
   uint32_t maxIndex; // indexador de maxima energia por cada fft
   q15_t maxValue;	  // maximo valor de energia del bin por cada fft
   char		pos[4];
} __attribute__ ((packed));

static struct header_struct header={"head",0,128,8000,0,0,"tail"};
static uint32_t cyclesCounterRead( void );
static void cyclesCounterReset( void );
static void cyclesCounterInit( void );
static uint32_t adcRead();
static void dacWrite(uint16_t value);

int SendChar(int ch)                    /* Write character to Serial Port     */
{
  HAL_UART_Transmit(&huart1, (void *)&ch, 1, 1000);
  return(ch);
}

int __io_putchar(int ch){ return (SendChar(ch)); }
static void trigger(int16_t threshold)
{
   while((adcRead()-512)>threshold)
	  ;
   while((adcRead()-512)<threshold)
	  ;
   return;
}

void DrawData(uint16_t *color) {
	char buffer[50];
	int size_data = snprintf(buffer, 50, "$%d %d %d;", color[0],color[1],color[2]);
	HAL_UART_Transmit(&huart1,buffer, size_data, 1);
}

int main_project( void ) {
	char buffer[50];
	FILE * fp;
   uint16_t sample = 0;
   arm_rfft_instance_q15 S;
   q15_t fftIn [ header.N	   ];// guarda copia de samples en Q15 como in para la fft.La fft corrompe los datos de la entrada!
   q15_t fftOut[ header.N *2   ];	// salida de la fft
   q15_t fftMag[ header.N /2+1 ]; // magnitud de la FFT
   int16_t adc [ header.N	   ];
   q15_t color[3];
   cyclesCounterInit ( );

   while(1) {
	  cyclesCounterReset();

	  adc[sample]	= (((int16_t )adcRead()-512)>>(10-BITS))<<(6+10-BITS);			 // PISA el sample que se acaba de mandar con una nueva muestra
	  fftIn[sample] = adc[sample];														 // copia del adc porque la fft corrompe el arreglo de entrada
	  if ( ++sample==header.N ) {
		 sample = 0;
		 arm_rfft_init_q15		   ( &S		,header.N	  ,0				,1				  ); // inicializa una estructira que usa la funcion fft para procesar los datos. Notar el /2 para el largo
		 arm_rfft_q15			   ( &S		,fftIn		  ,fftOut							  ); // por fin.. ejecuta la rfft REAL fft
		 arm_cmplx_mag_squared_q15 ( fftOut ,fftMag		  ,header.N/2+1						  );
		 arm_max_q15			   ( fftMag ,header.N/2+1 ,&header.maxValue ,&header.maxIndex );

		 //		 trigger(2);
		 header.id++;
		 int i=0;
		 int red = 22;
		 int green =22;


		for (i = 1; i < (header.N /2+1); i++) {

			if(i<red) color[0] += fftMag[(uint16_t)i];
			else if((red<i) && (i<red+green)) color[1] += fftMag[(uint16_t)i];
			else color[2] += fftMag[(uint16_t)i];
#ifdef plot_mag_fft
			int size_data = snprintf(buffer, 50, "$%d;", fftMag[(uint16_t)i]);
			HAL_UART_Transmit(&huart1,buffer, size_data, 1);
#endif

		}
#ifdef plot_fft
		int size_data = snprintf(buffer, 50, "$%d %d %d;", color[0],color[1],color[2]);
		HAL_UART_Transmit(&huart1,buffer, size_data, 1);
#endif
		color[0] /= red;
		color[1] /= green;
		color[2] /= ((header.N / 2+1) - red -green);
		DrawData(color);

	  }
      while(cyclesCounterRead()< NUCLEO_CLOCK_SPEED/header.fs) // el clk de la CIAA es 204000000
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

/* arm_rfft_init_f32 es la función utilizada para inicializar la configuración del algoritmo. Los parámetros &S y &S_CFFT son
estructuras globales que utiliza la función para almacenar los  valores  de  configuración,  FFT_LENGTH  determina
la longitud de la FFT, los valores admitidos son 128, 512 o 2048. ifftFlag  y  doBitReverse  son  banderas  binarias
que determinan si se realiza una transformación directa (FFT) o inversa (IFFT), y si la salida está en orden normal o en orden de bit invertido. */

//arm_rfft_init_f32(&S,  &S_CFFT,  FFT_LENGTH, ifftFlag, doBitReverse);

/* arm_rfft_f32 es la función encargada de realizar la FFT. El  parámetro  &S  es  la  estructura  global  utilizada
en  la función antes mencionada. FFT_DATA_IN es el arreglo de tipo float32_t y de longitud FFT_LENGTH que contiene
los datos  de  la  señal  muestreada.  FFT_CMPLX_DATA  es  el arreglo de tipo float32_t y de longitud FFT_LENGTH*2
que contiene los datos resultantes de la transformada. El arreglo resultante es el doble de tamaño del arreglo de
entrada porque los datos están en formato rectangular (real e imaginario).*/


//arm_rfft_f32(&S, FFT_DATA_IN, FFT_CMPLX_DATA);

/*
arm_cmplx_mag_f32 es la función que convierte los datos del  arreglo  FFT_CMPLX_DATA  de  rectangular  a
 polar (magnitud  y  fase),  y  almacena  únicamente  los  valores  de magnitud en FFT_MAG_DATA, que es un arreglo
 de tipo float32_t  y  de  longitud  FFT_LENGTH.  Debido  a  la naturaleza  del  algoritmo  para  realizar  la  transformada,
 los datos  en  el  arreglo  FFT_MAG_DATA  se  encuentran reflejados, siendo útiles FFT_LENGTH / 2. El  algoritmo  se  configuró
 para  trabajar  con FFT_LENGTH igual a 512 datos, y frecuencia de muestreo configurable. */

//arm_cmplx_mag_f32(FFT_CMPLX_DATA,  FFT_MAG_DATA, FFT_LENGTH);

/*R es  la resolución  de la FFT (Eq. 1  y Eq. 2), FS es  la frecuencia de muestreo y FN es la frecuencia de Nyquist.
FN es  equivalente  a  la  mitad  de  la  frecuencia  de  muestreo  y aparece como FMAX en la Fig. 4.
Puede ser modificada por el  usuario  dentro  de  una  gama  de  frecuencias  antes mencionadas.
Como  los  datos  útiles  son  la  mitad  de FFT_LENGTH,  la  resolución  se  calcula  de  la  siguiente manera:

R = (Fs)/FFT_length = Fs/512
R = (Fs/2)/(FFT_length/2) = Fn/256

*/


/*
 * 1_app.c
 *
 *  Created on: Jul 16, 2022
 *      Author: dev_fw
 */
//#include "arm_math.h"
//#include "arm_const_structs.h"
//#include "class3.h"
//#include "adc.h"
//#include "dac.h"
//#include "usart.h"
////#include "fir.h"
//#define NUCLEO_CLOCK_SPEED 80000000U
//
//#define BITS    10   // cantidad de bits usado para cuantizar
//
////uint32_t tick   = 0   ;
////uint16_t tone   = 100 ;
////uint16_t B      = 2500;
////uint16_t sweept = 5;
//// int16_t offset = 512;
//// int16_t zero = 0;
//
////----------------new code---------------
// /* FFT settings */
//#define SAMPLES					512 			/* 256 real party and 256 imaginary parts */
//#define FFT_SIZE				SAMPLES / 2		/* FFT size is always the same size as we have samples, so 256 in our case */
//#define FFT_BAR_MAX_HEIGHT		120 			/* 120 px on the LCD */
//float32_t Output[FFT_SIZE];
////--------------------------------------------
//struct header_struct {
//   char     pre[8];
//   uint32_t id;
//   uint16_t N;
//   uint16_t fs ;
//   uint16_t hLength ;
//   char     pos[4];
//} __attribute__ ((packed)); //importante para que no paddee
//
//static struct header_struct header={"*header*",0,SAMPLES,45000,500,"end*"};
//
//static uint32_t cyclesCounterRead( void );
//static void cyclesCounterReset( void );
//static void cyclesCounterInit( void );
//static uint32_t adcRead();
//static void dacWrite(uint16_t value);
//
//static void trigger(int16_t threshold)
//{
//   while((adcRead()-512)>threshold)
//      ;
//   while((adcRead()-512)<threshold)
//      ;
//   return;
//}
////-------------------------------------------------------------------------
//float32_t Input[SAMPLES];
//
///* Draw bar for LCD */
///* Simple library to draw bars */
//void DrawBar(uint16_t bottomX, uint16_t bottomY, uint16_t maxHeight, uint16_t maxValue, float32_t value, uint16_t foreground, uint16_t background) {
//	uint16_t height;
//	height = (uint16_t)((float32_t)value / (float32_t)maxValue * (float32_t)maxHeight);
//	if (height == maxHeight) {
//		printf("maximo valor");
//		//dibujo el maximo
//		//TM_ILI9341_DrawLine(bottomX, bottomY, bottomX, bottomY - height, foreground);
//	} else if (height < maxHeight) {
//		printf("valor menor");
//		//dibujo el restante
//		//TM_ILI9341_DrawLine(bottomX, bottomY, bottomX, bottomY - height, foreground);
//		//TM_ILI9341_DrawLine(bottomX, bottomY - height, bottomX, bottomY - maxHeight, background);
//	}
//}
//
//
//void main_project(){
//	arm_cfft_radix4_instance_f32 S;	/* ARM CFFT module */
//	float32_t maxValue;				/* Max FFT value is stored here */
//	uint32_t maxIndex;				/* Index in Output array where max value is */
//	uint16_t sample = 0;
//	uint16_t i;
//	cyclesCounterInit();
//
//	while(1) {
//		adcRead();
//		cyclesCounterReset();
//		Input[(uint16_t)sample] = (float32_t)((float32_t)adcRead() - (float32_t)2048.0) / (float32_t)2048.0;
//		Input[(uint16_t)(sample + 1)] = 0;
//		sample = sample+2;
//		if ( sample == SAMPLES ) {
//			/* Initialize the CFFT/CIFFT module, intFlag = 0, doBitReverse = 1 */
//			arm_cfft_radix4_init_f32(&S, FFT_SIZE, 0, 1);
//
//			/* Process the data through the CFFT/CIFFT module */
//			arm_cfft_radix4_f32(&S, Input);
//
//			/* Process the data through the Complex Magniture Module for calculating the magnitude at each bin */
//			arm_cmplx_mag_f32(Input, Output, FFT_SIZE);
//
//			/* Calculates maxValue and returns corresponding value */
//			arm_max_f32(Output, FFT_SIZE, &maxValue, &maxIndex);
//			if(maxIndex > 15){
//				printf("valor menor");
//			}
//			/* Display data on LCD */
//			for (i = 0; i < FFT_SIZE / 2; i++) {
//				/* Draw FFT results */
//				DrawBar(30 + 2 * i,
//						220,
//						FFT_BAR_MAX_HEIGHT,
//						(uint16_t)maxValue,
//						(float32_t)Output[(uint16_t)i],
//						0x1234,
//						0xFFFF
//				);
//			}
//			sample = 0;
//
//		}
//		//HAL_Delay(21);
//      while(cyclesCounterRead()< NUCLEO_CLOCK_SPEED/header.fs) // el clk de la CIAA es 204000000
//         ;
//   }
//}
