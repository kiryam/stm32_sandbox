/*
 * sdcard.h
 *
 *  Created on: Apr 6, 2016
 *      Author: kiryam
 */

#ifndef SDCARD_H_
#define SDCARD_H_

#include <string>
#include "fatfs.h"
#include "uart.h"


class SDCard
{

public:
	SDCard(Uart *uart);
	FRESULT mount();
	std::string get_file_cont(const char* filename);

private:
	Uart* uart;
	void init();
	void error(const char* error_str);
	void info(const char* info_str);
};

#endif /* SDCARD_H_ */
