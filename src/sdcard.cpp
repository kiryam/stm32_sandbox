#include "sdcard.h"
#include "stm32f4xx_hal.h"
#include "sstream"

SD_HandleTypeDef hsd;
HAL_SD_CardInfoTypedef SDCardInfo;
FATFS FatFs;

SDCard::SDCard(Uart *uart){
	this->uart = uart;
	init();
}

void SDCard::init() {
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	hsd.Instance = SDIO;
	hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
	hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
	hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd.Init.ClockDiv = 0;

	MX_FATFS_Init();

	//info("SD card united");
}

FRESULT SDCard::mount() {
	FRESULT fr = FR_OK;

	fr = f_mount(&FatFs, SD_Path, 1);
	if ( fr == FR_NO_FILESYSTEM ){
		error("NO FileSystem. Format card to FATFS");
		return fr;
	}else if( fr != FR_OK ){
		error("Disk mount error");
		return fr;
	}

	info("SD card mounted");
	return fr;
}

std::string SDCard::get_file_cont(const char* filename){
	std::string result;
	FIL fil;
	char line[82];
	FRESULT fr;

	fr = f_open(&fil, filename, FA_READ);
	if ( fr == FR_OK ) {
		while (f_gets(line, sizeof line, &fil))
			result.append(line);
	}else{
		//char msg[50] = {};
		//sprintf(msg, "File %s not found.", filename);
		//error(msg);
		return "";
	}

	f_close(&fil);
	return result;
}

int SDCard::write_str_to_file(const char* filename, std::string s){
	FIL fil;
	FRESULT fr;
	UINT testBytes=0;

	fr = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
	if ( fr != FR_OK) {
		//error("Failed to open out.txt for writing");
	    return 0;
	}

	fr = f_write(&fil, s.c_str(), s.length(), &testBytes);
	f_close(&fil);

	if( testBytes == s.length() ){
		//info("Data write ok.");
	}
	return testBytes;
}

void SDCard::info(const char* info_str) {
	std::ostringstream stringStream;
	stringStream << "[INFO] ";
	stringStream << info_str;
	stringStream << "\r\n";
	std::string copyOfStr = stringStream.str();
	this->uart->send_string(copyOfStr);
}

void SDCard::error(const char* error_str) {
	std::ostringstream stringStream;
	stringStream << "[ERROR] ";
	stringStream << error_str;
	stringStream << "\r\n";
	std::string copyOfStr = stringStream.str();
	this->uart->send_string(copyOfStr);
}
