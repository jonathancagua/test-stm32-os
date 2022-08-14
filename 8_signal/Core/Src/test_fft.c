/*
 * test_fft.c
 *
 *  Created on: Aug 13, 2022
 *      Author: jonathancagua
 */


// CMSIS Math includes
#include "arm_math.h"
#include "arm_const_structs.h"
#include "FFTsignal.h"
#define ej2
#ifdef ej1
// using a 1024 point signal
#define FFT_SAMPLES 1024
#define FFT_SAMPLES_HALF (FFT_SAMPLES / 2)

// see gist

static float32_t complexFFT[FFT_SAMPLES], realFFT[FFT_SAMPLES_HALF],
		imagFFT[FFT_SAMPLES_HALF], angleFFT[FFT_SAMPLES_HALF],
		powerFFT[FFT_SAMPLES_HALF];

static uint32_t fftSize = FFT_SAMPLES;
static uint32_t ifftFlag = 0;
static arm_rfft_fast_instance_f32 S;
static uint32_t maxIndex = 0;
static arm_status status;
static float32_t maxValue;
int i;

void test_main() {
  status = ARM_MATH_SUCCESS;
  status = arm_rfft_fast_init_f32(&S, fftSize);
  // input is real, output is interleaved real and complex
  arm_rfft_fast_f32(&S, inputSignal, complexFFT, ifftFlag);

	// first entry is all real DC offset
	float32_t DCoffset = complexFFT[0];

  // de-interleave real and complex values
  for (i = 0; i < (FFT_SAMPLES / 2) - 1; i++) {
    realFFT[i] = complexFFT[i * 2];
    imagFFT[i] = complexFFT[(i * 2) + 1];
  }

  // find angle of FFT
  for (i = 0; i < FFT_SAMPLES / 2; i++) {
    angleFFT[i] = atan2f(imagFFT[i], realFFT[i]);
  }
	// compute power
	arm_cmplx_mag_squared_f32(complexFFT, powerFFT, FFT_SAMPLES_HALF);
	arm_max_f32(&powerFFT[1], FFT_SAMPLES_HALF - 1, &maxValue, &maxIndex);
	// correct index
	maxIndex += 1;
}
#endif
#ifdef ej2
#define TEST_LENGTH_SAMPLES 2048

/* -------------------------------------------------------------------
* External Input and Output buffer Declarations for FFT Bin Example
* ------------------------------------------------------------------- */
extern float32_t testInput_f32_10khz[TEST_LENGTH_SAMPLES];
static float32_t testOutput[TEST_LENGTH_SAMPLES/2];

/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */
uint32_t fftSize = 1024;
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
arm_cfft_instance_f32 varInstCfftF32;

/* Reference index at which max energy of bin ocuurs */
uint32_t refIndex = 213, testIndex = 0;

/* ----------------------------------------------------------------------
* Max magnitude FFT Bin test
* ------------------------------------------------------------------- */

int32_t test_main(void)
{

  arm_status status;
  float32_t maxValue;

  status = ARM_MATH_SUCCESS;

  //status=arm_cfft_init_f32(&varInstCfftF32,fftSize);

  /* Process the data through the CFFT/CIFFT module */
  arm_cfft_f32(&varInstCfftF32, testInput_f32_10khz, ifftFlag, doBitReverse);

  /* Process the data through the Complex Magnitude Module for
  calculating the magnitude at each bin */
  arm_cmplx_mag_f32(testInput_f32_10khz, testOutput, fftSize);

  /* Calculates maxValue and returns corresponding BIN value */
  arm_max_f32(testOutput, fftSize, &maxValue, &testIndex);

  status = (testIndex != refIndex) ? ARM_MATH_TEST_FAILURE : ARM_MATH_SUCCESS;

  if (status != ARM_MATH_SUCCESS)
  {
#if defined (SEMIHOSTING)
    printf("FAILURE\n");
#else
    while (1);                             /* main function does not return */
#endif
  }
  else
  {
#if defined (SEMIHOSTING)
    printf("SUCCESS\n");
#else
    while (1);                             /* main function does not return */
#endif
  }
}
#endif
