/*
 * sdcard.h
 *
 *  Created on: Apr 6, 2016
 *      Author: kiryam
 */

#ifndef SDCARD_H_
#define SDCARD_H_

#include "fatfs.h"

class SDCard
{

public:
	SDCard();
	FRESULT mount();

private:
	void error(const char* error_str);
};

#endif /* SDCARD_H_ */
