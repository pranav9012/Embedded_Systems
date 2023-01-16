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

/* ======================================================================================
 * The command and response is stored in a CSV file. There are 200 commands from user
 * and the corresponding 200 responses. It is given that I'm able to read commands
 * from CSV file. To store the 200 reponses we can use an array of Structures.
 * ======================================================================================
 */


#include "main.h"
#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"

UART_HandleTypeDef huart2; // USART2 handler

void SystemClock_Config(void);
//static void MX_GPIO_Init(void);
//static void MX_USART2_UART_Init(void);
void UART2_Init(void);
void Error_Handler(void);
void USART2_TRANSMIT(uint8_t * buff , uint8_t length);

// Code for when dealing with user data
//--------------------------------------
//typedef struct{
//	uint16_t start;
//	uint32_t Adder;
//	uint8_t PID;
//	uint16_t Paklen;
//	uint8_t Data[253];
//	uint16_t Checksum;// 01+07 + 12345678;
//
//}verify_pw;
//-------------------------------------
// Code for when dealing with user data

//---------------------------------------------------------------------------------------------
// Buff array contain the package values according to what was given.
uint8_t buff[] = {0xEF, 0x01, 0xFF,0xFF,0xFF,0xFF,0x01,0x07,0x12,0x34,0x56,0x78,0x12,0x34,0x56,0x80};
uint8_t rcd_data[12];
uint8_t received_data;
int count = 0;


int main(void)
{

  HAL_Init();
  SystemClock_Config();
  //MX_GPIO_Init();
  //MX_USART2_UART_Init();


  // Code for when dealing with user data
  //--------------------------------------


//  verify_pw test;
//  test.start = 0xEF01;
//  test.Adder = 0xFFFFFFFF;
//  test.PID = 0x01;
//  test.Paklen = 0x07;
//  test.Data = 0x12345678;
//  test.Checksum = 0x12345680;
//
//
//  //Checksum
//  test.Checksum = (test.PID) + (test.Paklen) + (test.Data);
//
//  //Data to be transferred
//  uint8_t* buff = (uint8_t*)malloc(sizeof(verify_pw));
//  bool flag = 1;int i = 0;


  //=========================================================================
  //STM32F446RE is a little endian MCU. In the problem statement, we need to
  //first send the higher bytes followed by lower bytes. For this I wrote a
  // a loop which will store data in the buffer in the desired format.
  //==========================================================================


//  while(flag){
//	  // start, constant
//      buff[i++] = 0xEF;
//      buff[i++] = 0x01;
//
//      // Address, constant
//      buff[i++] = 0xFF;
//      buff[i++] = 0xFF;
//      buff[i++] = 0xFF;
//      buff[i++] = 0xFF;
//
//      //PID
//      buff[i++] = test.PID;
//
//      //Package length
//      uint8_t temp = test.Paklen;
//      buff[i++] = test.Paklen >> 8;
//      buff[i++] = temp;
//
//      //data
//      int k = sizeof(test.Data) - 1;
//      while(k >= 0){
//    	  buff[i + k] = test.Data >> 8;
//    	  k--;
//      }
//      i = i + sizeof(test.Data);
//
//      //Checksum
//      temp = test.Checksum;
//      buff[i++] = test.Checksum >> 8;
//      buff[i++] = temp;
//
//  }


  //--------------------------------------
  // Code for when dealing with user data





  //--------------------------Transmission--------------------------------------------------
  //uint8_t buff[] = {0xEF, 0x01, 0xFF,0xFF,0xFF,0xFF,0x01,0x07,0x12,0x34,0x56,0x78,0x12,0x34,0x56,0x80};
  //uint8_t length = (i - 1);
  uint8_t length = (sizeof(buff)/sizeof(uint8_t));
  UART2_Init();
  USART2_TRANSMIT(buff, length);


  //--------------------------------Reception-----------------------------------------------
  HAL_UART_Receive_IT(&huart2, &received_data, 1);
  switch(rcd_data[9]){
  case(0x00):
		  // Password verified
		  //print successfully
		  // I have not put anything in the cases since it was not asked. If required I can
		  // write something in these
		  break;

  case(0x01):
		  //there was some error which receiving the package from fingerprint module
		  Error_Handler();
  	  	  break;

  case(0x13):
		  // Incorrect password
		  break;

  default:
	  	  break;
  }


  while(1);


  return(0);
}





void SystemClock_Config(void){
}


void UART2_Init(void){
	huart2.Instance = USART2;// using Asynchronous mode
	//parameter initialization
	huart2.Init.BaudRate = 57600;// default of fingerprint module is 9600*6
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	if(HAL_UART_Init(&huart2) != HAL_OK){
		//there is some problem
		Error_Handler();
	}

	}


void USART2_TRANSMIT(uint8_t * buff , uint8_t length){
	  if(HAL_UART_Transmit(&huart2, buff, length, HAL_MAX_DELAY) != HAL_OK){
		  Error_Handler();
	  }
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{// first LSB will be received
	//static int count = 0;
	rcd_data[count++] = received_data;
	HAL_UART_Receive_IT(&huart2, &received_data, 1);
}



void Error_Handler(void)
{
	while(1);
}


//void USART2_IRQHandler(void)
//{
//	HAL_UART_IRQHandler(&huart2);
//
//
//}



