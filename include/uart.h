/*
 * uart.h
 *
 *  Created on: Apr 6, 2016
 *      Author: kiryam
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx_hal.h"
#include <string>

class Uart {
public:
	Uart();
	void send_string(std::string s);

private:
	void init();
};


#endif /* UART_H_ */
