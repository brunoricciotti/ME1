/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "MPU9250.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
int16_t arcsen(uint16_t);
uint16_t fpsin(int16_t);

//LUT de seno con 360 puntos, su forma se explica en arcsen()
const uint16_t LUTsin[360] =
{
0 , 287 , 574 , 862 , 1149 ,
1435 , 1722 , 2007 , 2292 , 2577 ,
2860 , 3143 , 3425 , 3705 , 3985 ,
4263 , 4540 , 4816 , 5090 , 5362 ,
5633 , 5902 , 6170 , 6435 , 6698 ,
6960 , 7219 , 7476 , 7731 , 7983 ,
8233 , 8480 , 8725 , 8967 , 9206 ,
9443 , 9676 , 9907 , 10134 , 10359 ,
10580 , 10798 , 11012 , 11224 , 11431 ,
11635 , 11836 , 12033 , 12226 , 12416 ,
12602 , 12783 , 12961 , 13135 , 13305 ,
13471 , 13632 , 13790 , 13943 , 14092 ,
14236 , 14376 , 14512 , 14643 , 14770 ,
14892 , 15010 , 15123 , 15231 , 15334 ,
15433 , 15527 , 15617 , 15701 , 15781 ,
15856 , 15926 , 15991 , 16051 , 16106 ,
16156 , 16202 , 16242 , 16277 , 16307 ,
16332 , 16353 , 16368 , 16378 , 16383 ,
16383 , 16378 , 16368 , 16353 , 16332 ,
16307 , 16277 , 16242 , 16202 , 16156 ,
16106 , 16051 , 15991 , 15926 , 15856 ,
15781 , 15701 , 15617 , 15527 , 15433 ,
15334 , 15231 , 15123 , 15010 , 14892 ,
14770 , 14643 , 14512 , 14376 , 14236 ,
14092 , 13943 , 13790 , 13632 , 13471 ,
13305 , 13135 , 12961 , 12783 , 12602 ,
12416 , 12226 , 12033 , 11836 , 11635 ,
11431 , 11224 , 11012 , 10798 , 10580 ,
10359 , 10134 , 9907 , 9676 , 9443 ,
9206 , 8967 , 8725 , 8480 , 8233 ,
7983 , 7731 , 7476 , 7219 , 6960 ,
6698 , 6435 , 6170 , 5902 , 5633 ,
5362 , 5090 , 4816 , 4540 , 4263 ,
3985 , 3705 , 3425 , 3143 , 2860 ,
2577 , 2292 , 2007 , 1722 , 1435 ,
1149 , 862 , 574 , 287 , 0 ,
};
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int16_t Datos, resultado;
uint32_t ticks =0;
int16_t angulofinal;
int main(void)
{
  /* USER CODE BEGIN 1 */
//uint32_t ticks =0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  MPU9250_init();//inicializo el sensor

  /* USER CODE END 2 */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  ticks = HAL_GetTick();
    /* USER CODE END WHILE */
	  if(!(ticks %= 500))
	  {
		  //Datos: variable que va desde 0 a 65535
		  Datos = MPU_readRawData();

		  //Para pasar de datos a resultado, divido por la gravedad y hago regla de 3 para convertirlo al rango de resultado
		  //Arcoseno recibe de -1 a 1, si 65535 es 8192, entonces datos es x
		  //Resultado, variable que va de 4096 a 8192 (0: 4096, 8192:pi/2)
		  //resultado = 4097 + (5*Datos/(8*10)); //Gravedad 10
		  angulofinal = arcsen(Datos);

		  //Por si se quiere probar la funcion senoidal
		  //Al argumento se fpsin entra un entero con rango -4096 a 4096
		  //resultado = fpsin((int16_t)ii);
		  //resultado_real = (int16_t) resultado;
		  //result2 = ((int16_t)resultado*1000)/4096;
	  }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
//Funcion que implementa un seno con enteros
uint16_t fpsin(int16_t i)
{
    /* Convert (signed) input to a value between 0 and 8192. (8192 is pi/2, which is the region of the curve fit).-*/

	i <<= 1;
    uint8_t c = i<0; //set carry for output pos/neg

    if(i == (i|0x4000)) // flip input value to corresponding value in range [0..8192)
        i = (1<<15) - i;
    i = (i & 0x7FFF) >> 1;


	/* The following section implements the formula:
	 = y * 2^-n * ( A1 - 2^(q-p)* y * 2^-n * y * 2^-n * [B1 - 2^-r * y * 2^-n * C1 * y]) * 2^(a-q)
	Where the constants are defined as follows:
	*/
	uint32_t y;
    enum {A1=3370945099UL, B1=2746362156UL, C1=292421UL};
    enum {n=13, p=32, q=31, r=3, a=12};

    y = (C1*((uint32_t)i))>>n;
    y = B1 - (((uint32_t)i*y)>>r);
    y = (uint32_t)i * (y>>n);
    y = (uint32_t)i * (y>>n);
    y = A1 - (y>>(p-q));
    y = (uint32_t)i * (y>>n);
    y = (y+(1UL<<(q-a-1)))>>(q-a); // Rounding

    return c ? -y : y;;
}

int16_t arcsen(uint16_t dato)
{
	int i = 0;
	static int a = 0;
	uint16_t res = 0;
	static int masnoventa = 0;
	//Dato es resultado, un valor que va de -1 a 1, o mas exactamente de 0 a 8192 (unsigned) donde 0, 180 y 360 son 4096
	//90 grados son 8192 y 270 grados es 0
	//Dato empieza en 0, va para 1, baja a 0 y termina ahi (180 grados)
	//Yo tengo una LUT que va de 0 a 8192, empieza en 4096, sube hasta 8192, vuelve a bajar hasta 0 y termina en 4096
	//Comparando LUT y dato (hasta pi/2): si 4096 es 0(dato), entonces  es 2048(dato)
	for (i = 0; i<= 90; i++)
	{
		if ((dato >= LUTsin[i]) && ((dato <= LUTsin[i+1])))
		{
			res = i;
			a = 0;//No estoy en 90 grados
		}
	}
	if (dato == 8192) //Caso 90 grados, pto maximo de la funcion solo puedo verla de esta forma, 8191 ya son 89 grados
	{
		//A esta funcion puede entrar varias veces seguidas hasta que se cambie el angulo, defino variable a, que se pondrá en 1
		//si es la primera vez que entra a este if
		if (!a)
		{
			a = 1;
			if (!masnoventa) //Si estaba en 89 y subí a 90, ahora el resultado va a estar entre 91 y 180, por lo que cambio el modo
				masnoventa = 1;
			else masnoventa = 0;//si estaba en 91 y bajó, hago lo contrario
		}
		res = 90;
	}
	//ME PASO DE 90 GRADOS ---> sen(x) = sen(180-x)
	if ((masnoventa == 1) && (a == 0))
	{
		res = 180-res;
	}
	return res;
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
