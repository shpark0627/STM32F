/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
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
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define MAX_LEN 100

// PC(USART2) 관련
uint8_t pc_data;             // 1바이트 수신용
char pc_buffer[MAX_LEN];     // 문장 보관용
uint8_t pc_idx = 0;

// ID 관련 변수
char id[20] = "User";        // 초기 ID (기본값)
uint8_t is_id_mode = 0;

// 상대 보드(USART1) 관련
uint8_t board_data;          // 1바이트 수신용
char board_buffer[MAX_LEN];  // 문장 보관용
uint8_t board_idx = 0;

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

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
  // 수신 인터럽트 활성화 (1바이트를 받으면 인터럽트 발생)
  HAL_UART_Receive_IT(&huart2, &pc_data, 1);
  HAL_UART_Receive_IT(&huart1, &board_data, 1);
  /* USER CODE END 2 */

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA10 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if (huart->Instance == USART2) {
	        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);

	        // [A] Tab 키(\t)를 눌렀을 때 -> ID 변경 모드로 진입
	        if (pc_data == '\t' || pc_data == 0x09) {
	            is_id_mode = 1;       // 모드 변경
	            pc_idx = 0;           // 버퍼 초기화

	            // 안내 메시지 출력
	            char *msg = "\r\n[System] Input User ID: ";
	            HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
	        }

	        else if (pc_data == 0x08){
	        	char clear_command[] = "\033[1D\033[K";
	        	pc_buffer[pc_idx--] = '\0';
				HAL_UART_Transmit(&huart2, (uint8_t*)clear_command, strlen(clear_command), HAL_MAX_DELAY);
	        }

	        else if (pc_data == 0x7F || pc_data == '1'){
	        	char clear_command[] = "\033[2J\033[H";
	        	HAL_UART_Transmit(&huart2, (uint8_t*)clear_command, strlen(clear_command), HAL_MAX_DELAY);

	        }

	        // [B] 엔터 키(\r, \n)를 눌렀을 때
	        else if (pc_data == '\r' || pc_data == '\n') {
	            if (pc_idx > 0) {
	                pc_buffer[pc_idx] = '\0'; // 문자열 마무리

	                // --- 상황 1: ID 입력 모드였다면? ---
	                if (is_id_mode == 1) {
	                    strcpy(id, pc_buffer); // ID 변경 적용!
	                    is_id_mode = 0;        // 다시 채팅 모드로 복귀

	                    // 1. 나에게 알림
	                    char log_msg[120];
	                    sprintf(log_msg, "\r\n[System] ID -> [%s]\r\n", id);
	                    HAL_UART_Transmit(&huart2, (uint8_t*)log_msg, strlen(log_msg), 100);

	                    // 2. 상대방에게 알림 (선택사항)
	                    char send_msg[120];
	                    sprintf(send_msg, "\r\n[Info] The other person ID -> [%s]\r\n", id);
	                    HAL_UART_Transmit(&huart1, (uint8_t*)send_msg, strlen(send_msg), 100);
	                }

	                // --- 상황 2: 그냥 채팅 모드였다면? ---
	                else {
	                    // 1. 상대방에게 전송 (내 ID를 붙여서!)
	                    char send_msg[150];
	                    sprintf(send_msg, "[%s]: %s\r\n", id, pc_buffer); // 예: [Seung]: 안녕
	                    HAL_UART_Transmit(&huart1, (uint8_t*)send_msg, strlen(send_msg), 100);

	                    // 2. 내 화면에 표시
	                    char log_msg[150];
	                    sprintf(log_msg, "\r\n[%s]: %s\r\n", id, pc_buffer);
	                    HAL_UART_Transmit(&huart2, (uint8_t*)log_msg, strlen(log_msg), 100);
	                }
	                pc_idx = 0; // 버퍼 비우기
	            }
	        }

	        // [C] 일반 글자 입력 (엔터, 탭 아님)
	        else {
	            if (pc_idx < MAX_LEN - 1) {
	                pc_buffer[pc_idx++] = pc_data;
	                // 에코(Echo): 내가 친 글자 화면에 보여주기
	                HAL_UART_Transmit(&huart2, &pc_data, 1, 10);
	            }
	        }
	        HAL_UART_Receive_IT(&huart2, &pc_data, 1);
	    }


	    if (huart->Instance == USART1) { // 상대 보드로부터 데이터가 왔을 때

	        if (board_data == '\r' || board_data == '\n') {
	            if (board_idx > 0) {
	                board_buffer[board_idx] = '\0';

	                char log_msg[120];
	                sprintf(log_msg, "\r\n%s\r\n", board_buffer);
	                HAL_UART_Transmit(&huart2, (uint8_t*)log_msg, strlen(log_msg), 100);
	                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);

	                board_idx = 0;
	            }
	        }
	        else {
	            if (board_idx < MAX_LEN - 1) {
	                board_buffer[board_idx++] = board_data;

	            }
	        }
	        HAL_UART_Receive_IT(&huart1, &board_data, 1);
	    }
}

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
#ifdef USE_FULL_ASSERT
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
