#include "sdcard.h"
#include "stm32f4xx_hal.h"

SDCard::SDCard(){

}

FRESULT SDCard::mount()
{
	FRESULT fr = FR_OK;
/*
	fr = f_mount(&FatFs, SD_Path, 1);
	if ( fr == FR_NO_FILESYSTEM ){
		error("NO FileSystem. Format card to FATFS\r\n");
		return fr;
	}else if( fr != FR_OK ){
		error("Disk mount error\r\n");
		return fr;
	}

	return fr;
	*/
}


void SDCard::error(const char* error_str){
	//send_string(error_str);
}
