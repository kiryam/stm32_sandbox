/*
 * uart.c
 *
 *  Created on: Apr 6, 2016
 *      Author: kiryam
 */

#include "uart.h"

UART_HandleTypeDef huart3;

Uart::Uart() {
	init();
};

void Uart::send_string(std::string s) {
	const uint8_t* transmitBuffer;
	transmitBuffer = reinterpret_cast<const uint8_t*>(s.data());
	HAL_UART_Transmit(&huart3, (uint8_t*) transmitBuffer, s.length(), 1000);
}

void Uart::init() {
	__HAL_RCC_GPIOB_CLK_ENABLE();

	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
  	huart3.Init.Mode = UART_MODE_TX_RX;
  	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  	HAL_UART_Init(&huart3);
}
