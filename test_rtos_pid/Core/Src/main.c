/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dac.h"
#include "dfsdm.h"
#include "i2c.h"
#include "quadspi.h"
#include "spi.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h" //<< si usas esto agregar ARM_MATH_CM4
							//__FPU_PRESENT=1
							//__TARGET_FPU_VFP en config
#include "pidTask.h"
#include "identification_ls.h"
#include "identification_rls.h"
#include "identification_tasks.h"
#include "app_adc.h"
#include "pid.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PID
// Noise signal limits
#define DAC_REFERENCE_VALUE_HIGH   666  // 1023 = 3.3V, 666 = 2.15V
#define DAC_REFERENCE_VALUE_LOW    356  // 1023 = 3.3V, 356 = 1.15V
#define ADC0_CH_Y       3

//#define getVoltsSampleFrom(adc0Channel) 3.3*(float)adcRead((adc0Channel))/1023.0

static StackType_t taskIdentificationStack[configMINIMAL_STACK_SIZE*15];
static StaticTask_t taskIdentificationTCB;

t_IRLSdata* tIRLS1;
t_ILSdata* tILS1;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
void receiveData (float* buffer);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//arm_matrix_instance_f32 E;      // Error(k) = y(k) - aux4 -> 1 x 1
// Generación del DAC y captura del ADC


static float getVoltsSampleFrom(ADC_HandleTypeDef *ptr_hadc) {
	return (3.3*(float)adcRead(ptr_hadc)/1023.0);
}
void receiveData(float* buffer) {
    float Y, U;

    uint16_t dacValue = 0;
    uint32_t dac_convert = 0;

    dacValue = rand() % 65535;
    dac_convert = (dacValue * 996)/65535;//996
    dacWrite(dacValue);
    vTaskDelay(5);
    U = (float)dacValue * 3.3 / 65535.0;
    uint32_t sample = adcRead(&hadc1);
    Y = 3.3 * (float)sample / 996.0;

    printf ("%d / %d \r\n", ((int16_t)sample), ((int16_t)dac_convert));

    buffer[0] = U;
    buffer[1] = Y;
}
/*void receiveData (float* buffer)
{
    float Y, U;

    uint16_t dacValue = 0;
    uint32_t adcValue = 0;

    // random = limite_inferior + rand() % (limite_superior +1 - limite_inferior);
    dacValue = DAC_REFERENCE_VALUE_LOW + rand() % (DAC_REFERENCE_VALUE_HIGH+1 - DAC_REFERENCE_VALUE_LOW);

    dacWrite( dacValue );

    // Need at least 2.5 us to uptate DAC.
    //delayInaccurateUs(5);

    // dacSample = (1023.0 / 3.3) * sampleInVolts
    // 1023.0 / 3.3 = 310.0
    U = (float) dacValue * 3.3 / 1023.0;
	Y = (float) getVoltsSampleFrom( &hadc2 );
	adcValue = adcRead(&hadc2);

	printf ("%d - %d \r\n", adcValue,dacValue);

	buffer[0] = U;
	buffer[1] = Y;
}*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DFSDM1_Init();
  MX_I2C2_Init();
  MX_QUADSPI_Init();
  MX_SPI3_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_ADC2_Init();
  MX_DAC1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
#if defined(PID)
  init_pid();
  /*xTaskCreate(
     pidControlTask,                 // Function that implements the task.
     (const char *)"pidControlTask", // Text name for the task.
     configMINIMAL_STACK_SIZE*2,     // Stack size in words, not bytes.
     0,                              // Parameter passed into the task.
     tskIDLE_PRIORITY+1,             // Priority at which the task is created.
     0                               // Pointer to the task created in the system
  );*/
#elif defined(ILS)
	tILS1 = (t_ILSdata*) malloc (sizeof(t_ILSdata));
	ILS_Init(tILS1, 50, 10, receiveData);

	xTaskCreate(
	  ILS_Task,                   // task function
	  "Identification Task",      // human-readable neame of task
	  configMINIMAL_STACK_SIZE,   // task stack size
	  (void*)tILS1,               // task parameter (cast to void*)
	  tskIDLE_PRIORITY+1,         // task priority
//	  taskIdentificationStack,    // task stack (StackType_t)
	  &taskIdentificationTCB      // pointer to Task TCB (StaticTask_t)
	);
#elif defined(STEP)
	init_ident_step();
#elif defined(POLE)
	init_pole();
#else
	tIRLS1 = (t_IRLSdata*) pvPortMalloc (sizeof(t_IRLSdata));
	IRLS_Init(tIRLS1, 10, receiveData);
	xTaskCreateStatic(
	   IRLS_Task,                  // task function
	   "Identification Task",      // human-readable neame of task
	   configMINIMAL_STACK_SIZE,   // task stack size
	   (void*)tIRLS1,              // task parameter (cast to void*)
	   tskIDLE_PRIORITY+1,         // task priority
	   taskIdentificationStack,    // task stack (StackType_t)
	   &taskIdentificationTCB      // pointer to Task TCB (StaticTask_t)
	);
#endif
  vTaskStartScheduler(); // Initialize scheduler
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK|RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
