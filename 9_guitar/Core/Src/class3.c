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
#include "fir_65_400_10000.h"
#define NUCLEO_CLOCK_SPEED 80000000U

#define INTERPOL_WIDTH 200
#define FFT_LENGTH     4096
#define OVERSAMPLE     10 //ojo que tiene que conincidir con el disenio del filtro

struct header_struct {
   char     pre[8];
   uint32_t id;
   uint16_t N;
   uint16_t fs ;
   q15_t maxValue ;
   uint32_t maxIndex ;
   char     pos[4];
} __attribute__ ((packed)); //importante para que no paddee

int16_t chord,tune;
float chordsF[]={329.63, 246.94, 196.00, 146.83, 110.00, 82.41};

struct header_struct header={"*header*",0,512,1000,0,0,"end*"};



//static struct header_struct header={"head",0,128,2000,0,0,"tail"};
void init_cfft_instance ( arm_cfft_instance_q15* CS ,int length                      );
void noAgc              ( int16_t* adc              ,uint16_t len                    );
void agc                ( int16_t* adc              ,uint16_t len                    );
void findChord          ( uint32_t index            ,uint16_t* chord ,uint16_t* tune );
void interpol           ( q15_t* magFft             ,uint32_t* maxIndex              );

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



void class3_1_main(){
	int16_t  adc_array[ header.N*OVERSAMPLE+h_LENGTH-1];
	int16_t  fft_array[ header.N*OVERSAMPLE+h_LENGTH-1];
	int16_t  *adc=adc_array;
	int16_t  *fft=fft_array;
	arm_cfft_instance_q15 CS;
	q15_t fftAbs    [ FFT_LENGTH ];
	q15_t fftAbsProm[ FFT_LENGTH ];
	int16_t  x   [ header.N   ];
	int16_t  sample      = 0;
	int16_t  downSample  = 0;
	q15_t    maxAdc      = 0;
	uint32_t maxAdcIndex = 0;

	cyclesCounterInit();
	while(1) {
		cyclesCounterReset();
		//---------------sampleado y offset--------------------------
		adc[sample] = adcRead()-505; //ajusto el offset

		//---------------envio datos a rate de downsample--------------------------
		if(sample<header.N) {
			HAL_UART_Transmit(&huart1,(uint8_t* )&x     [sample] ,sizeof(x[0]) ,1);
			HAL_UART_Transmit(&huart1,(uint8_t* )&fftAbs[sample] ,sizeof(fftAbs[0]) ,1);
		}
		//---------------sampleo a oversampling--------------------------
		sample++;
		if ( sample>=(header.N*OVERSAMPLE) ) {
			//---------------escalado automatico--------------------------
			//noAgc(adc,header.N*OVERSAMPLE);
			agc(adc,header.N*OVERSAMPLE);

			//---------------filtrado antialias en digitial--------------------------
			arm_conv_fast_q15  ( adc,header.N*OVERSAMPLE,h,h_LENGTH,fft); //se podria reutilizar el adc en vez de y como salida

			//---------------escalado automatico a posteriori del filtrado--------------------------
			agc(fft,header.N*OVERSAMPLE+h_LENGTH-1);

			//---------------downsampling--------------------------
			for(int i=0;i<header.N;i++){
				x[i]       = fft[i*OVERSAMPLE+(h_LENGTH-1)/2];//arranca desde la zona valida
			}
			//---------------prpearo vector para hacer fft--------------------------
			int i;
			for(i=0;i<header.N;i++){
				fft[2*i+0] = x[i];
				fft[2*i+1] = 0;
			}
			//---------------zero padding--------------------------
			for(;i<FFT_LENGTH;i++){
				fft[2*i+0] = 0;
				fft[2*i+1] = 0;
			}

			//------------TRANSFORMADA------------------
			init_cfft_instance ( &CS,FFT_LENGTH);
			arm_cfft_q15       ( &CS ,fft ,0 ,1 );

			//------------MAGNITUD------------------
			arm_cmplx_mag_squared_q15 ( fft ,fftAbs ,FFT_LENGTH);

			//------------Promedio de 2 espectros------------------
			//         for(int i=0;i<FFT_LENGTH;i++){
			//            fftAbsProm[i] = fftAbsProm[i]/2 + fftAbs[i]/2;
			//         }
			//------------opcional SIN Promedio (queda mas fluido)------------------
			for(int i=0;i<FFT_LENGTH;i++){
				fftAbsProm[i] = fftAbs[i];
			}


			//------------BUSCO EL MAXIMO------------------
			arm_max_q15 ( fftAbsProm ,FFT_LENGTH ,&header.maxValue ,&header.maxIndex );
			header.maxValue<<=10;

			//------------Centro de masas------------------
			interpol(fftAbsProm,&header.maxIndex); //TODO ojo! aca el max index sale x1000

			//------------Downsample ABS (uso internamente FFT_LENGTH pero muestro solo N------------------
			int down=FFT_LENGTH/header.N;
			for(int i=0;i<header.N;i++) {
				fftAbs[i] = fftAbsProm[i*down]<<10;
			}

			//------------BUSCO CUERDA------------------
			findChord(header.maxIndex,&chord,&tune);

			//------------ENCIENDO LEDS------------------
			//ledManagement(chord,tune );

			HAL_UART_Transmit(&huart1, (uint8_t*)&header ,sizeof(struct header_struct ),1);
			header.id++;
			sample     = 0;
			downSample = 0;
			adcRead(); //why?? hay algun efecto minimo en el 1er sample.. puede ser por el blinkeo de los leds o algo que me corre 10 puntos el primer sample. Con esto se resuelve.. habria que investigar el problema en detalle
		}
//		HAL_UART_Transmit(&huart1, (uint8_t* )&adc[sample], sizeof(adc[0]), 1);
//		HAL_UART_Transmit(&huart1, (uint8_t* )&fftOut[sample*2],sizeof(fftOut[0]), 1);// envia la fft del sample ANTERIO
//		HAL_UART_Transmit(&huart1, (uint8_t* )&fftOut[sample*2+1],sizeof(fftOut[0]), 1);// envia la fft del sample ANTERIO
//		adc[sample]       = (((int16_t )adcRead()-512)>>(10-BITS))<<(6+10-BITS);
//		fftIn[sample]   = adc[sample];
//		if ( ++sample==header.N ) {
//			sample = 0;
//			//------------TRANSFORMADA------------------
//			arm_rfft_init_q15		   	( &S		,header.N	  ,0				,1				  ); // inicializa una estructira que usa la funcion fft para procesar los datos. Notar el /2 para el largo
//			arm_rfft_q15			   	( &S		,fftIn		  ,fftOut							  ); // ejecuta la rfft REAL fft
//			arm_cmplx_mag_squared_q15 	( fftOut 	,fftMag		  ,header.N/2+1						  );
//			arm_max_q15			   		( fftMag 	,header.N/2+1 ,&header.maxValue ,&header.maxIndex );
//
//			header.id++;
//			HAL_UART_Transmit(&huart1, (uint8_t* )&header, sizeof(struct header_struct ), 1);
//			adcRead();
//		}
		while(cyclesCounterRead() < NUCLEO_CLOCK_SPEED/header.fs) // el clk de la nucleo es 80M
		;
	}

}
void ledManagement(uint16_t chord,uint16_t tune)
{
   //gpioWrite(LED1,tune&0x01?1:0);
   //gpioWrite(LED2,tune&0x02?1:0);
   //gpioWrite(LED3,tune&0x04?1:0);
   switch (chord){
      case 0:
         //gpioWrite(LEDR,1);
         break;
      case 1:
         //gpioWrite(LEDG,1);
         break;
      case 2:
         //gpioWrite(LEDB,1);
         break;
      case 3:
         //gpioWrite(LEDR,1);
         //gpioWrite(LEDG,1);
         break;
      case 4:
         //gpioWrite(LEDR,1);
         //gpioWrite(LEDB,1);
         break;
      case 5:
         //gpioWrite(LEDB,1);
         //gpioWrite(LEDG,1);
         break;
      default:
         //gpioWrite(LEDR,0);
         //gpioWrite(LEDB,0);
         //gpioToggle(LEDG);
         break;
   }
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
}/*}}}*/

void noAgc(int16_t* adc,uint16_t len)
{
   for(int i=0;i<len;i++)
      adc[i]<<=6;
   return;
}
void agc(int16_t* adc,uint16_t len)
{
   int max=0;
   int i,gainFactor;
   for(i=0;i<len;i++) {
      int abs=adc[i]>0?adc[i]:-adc[i];
      if(abs>max)
         max=abs;
   }
   gainFactor=0x8000/max;
   for(i=0;i<len;i++) {
      adc[i]*=gainFactor;
   }
   return;
}

void findChord(uint32_t index,uint16_t* chord, uint16_t* tune)
{
#define MARGIN  10.00*1000
#define TUNE     0.25*1000
#define MID_TUNE 2.00*1000

   float frec = (header.fs*index)/FFT_LENGTH;
   *chord = 7;
   *tune  = 0;
   for(int i=0;i<6;i++){
      if(frec>(chordsF[i]*1000-MARGIN) && frec<(chordsF[i]*1000+MARGIN)) {
         *chord=i;
         if(frec>(chordsF[i]*1000+TUNE))
            *tune=0x04;
         else
            if(frec<(chordsF[i]*1000-TUNE))
               *tune=0x01;

         if(frec>(chordsF[i]*1000-MID_TUNE) && frec<(chordsF[i]*1000+MID_TUNE))
            (*tune)|=0x02;
         break;
      }
   }
}

void interpol(q15_t* magFft,uint32_t* maxIndex)
{
   int32_t interpolF;
   int32_t f=0;
   int32_t sum=0;

   for (int i=0;i<(2*INTERPOL_WIDTH+1);i++) {
      interpolF=magFft[*maxIndex-INTERPOL_WIDTH+i];
      sum+=interpolF;
      f += interpolF*(*maxIndex-INTERPOL_WIDTH+i);
   }

   *maxIndex=(f*1000)/sum;
   if(*maxIndex>((FFT_LENGTH/2)*1000))
      *maxIndex=(FFT_LENGTH/2)*1000;
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
