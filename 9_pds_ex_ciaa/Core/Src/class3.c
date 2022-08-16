/*
 * 1_app.c
 *
 *  Created on: Jul 16, 2022
 *      Author: dev_fw
 */
#include "arm_math.h"
#include "arm_const_structs.h"
#include "class3.h"
#include "adc.h"
#include "dac.h"
#include "usart.h"
#include "fir.h"
#define NUCLEO_CLOCK_SPEED 80000000U

#define BITS    10   // cantidad de bits usado para cuantizar

uint32_t tick   = 0   ;
uint16_t tone   = 100 ;
uint16_t B      = 2500;
uint16_t sweept = 5;
 int16_t offset = 512;
 int16_t zero = 0;

 struct header_struct {
    char     pre[4];
    uint32_t id;
    uint16_t N;
    uint16_t fs ;
    uint32_t maxIndex; // indexador de maxima energia por cada fft
    q15_t maxValue;    // maximo valor de energia del bin por cada fft
    char     pos[4];
 } __attribute__ ((packed));

 static struct header_struct header={"head",0,128,10000,0,0,"tail"};

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


//void class3_1_main(){
//   uint16_t sample = 0;
//   int16_t adc   [ header.N            ];
//   int16_t y     [ h_LENGTH+header.N-1 ]; //
//   cyclesCounterInit();
//   while(1) {
//      cyclesCounterReset();
//
//      adc[sample]       = (((int16_t )adcRead()-512)>>(10-BITS))<<(6+10-BITS);          // PISA el sample que se acaba de mandar con una nueva muestra
//      if ( ++sample==header.N ) {
//         sample = 0;
////------------CONVOLUCION------------------
//         arm_conv_fast_q15  ( adc,header.N,h,h_LENGTH,y); //126+74-1
////------------ENVIO DE TRAMA------------------
//         header.id++;
//         HAL_UART_Transmit(&huart1,(uint8_t*)&header ,sizeof(struct header_struct ),1);
//         for (int i=0;i<(header.N+h_LENGTH-1 );i++) {
//        	 HAL_UART_Transmit(&huart1,(uint8_t* )(i<header.N?&adc[i]:&offset ),sizeof(adc[0]),1);
//        	 HAL_UART_Transmit(&huart1,(uint8_t* )(i<h_LENGTH?&h  [i]:&zero   ),sizeof(h[0])  ,1);
//        	 HAL_UART_Transmit(&huart1,(uint8_t* )(           &y  [i]         ),sizeof(y[0])  ,1);
//         }
//      }
//
//      while(cyclesCounterRead()< NUCLEO_CLOCK_SPEED/header.fs) // el clk de la CIAA es 204000000
//         ;
//   }
//}

void class3_2_main(){
	uint16_t sample = 0;
	arm_rfft_instance_f32 S;
	arm_cfft_radix4_instance_f32  cS;
	float fftIn [ header.N      ]; // guarda copia de samples en Q15 como in para la fft.La fft corrompe los datos de la entrada!
	float fftOut[ header.N *2   ]; // salida de la fft
	float adc [ header.N        ];
	cyclesCounterInit();
	while(1) {
		int16_t adcRaw;
		cyclesCounterReset();
		HAL_UART_Transmit(&huart1, (uint8_t* )&adc[sample], sizeof(adc[0]), 1);
		HAL_UART_Transmit(&huart1, (uint8_t* )&fftOut[sample*2],sizeof(fftOut[0]), 1);// envia la fft del sample ANTERIO
		HAL_UART_Transmit(&huart1, (uint8_t* )&fftOut[sample*2+1],sizeof(fftOut[0]), 1);// envia la fft del sample ANTERIO
		adcRaw      = adcRead()-2048.0;
		adc[sample]   = adcRaw/2048.0;            // PISA el sample que se acaba de mandar con una nueva muestra
		fftIn[sample] = adcRaw;

		if ( ++sample==header.N ) {
			sample = 0;
			//------------TRANSFORMADA------------------
			arm_rfft_init_f32 ( &S ,&cS   ,header.N ,0 ,1 ); // inicializa una estructira que usa la funcion fft para procesar los datos. Notar el /2 para el largo
			arm_rfft_f32      ( &S ,fftIn ,fftOut         ); // por fin.. ejecuta la rfft REAL fft

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
