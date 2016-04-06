#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "sdcard.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


int main(int argc, char* argv[]){
	Uart uart = Uart();
	SDCard sdcard = SDCard(&uart);
	sdcard.mount();

	std::string settings_cont = sdcard.get_file_cont("settings.txt");
	uart.send_string(settings_cont);


/*
	FIL fil;
	char line[82];
	FRESULT fr;

	fr = f_open(&fil, "settings.txt", FA_READ);
	if ( fr == FR_OK ) {
		send_string("Contents of settings.txt\r\n");
		send_string("==========================\r\n");
		while (f_gets(line, sizeof line, &fil))
			send_string(line);
		send_string("\r\n==========================\r\n");
	}else{
		send_string("File settings.txt not found.\r\n");
		return 1;
	}

	UINT testBytes=0;
	uint8_t testBuffer[9] = "SD write";
    FIL fil2;
    fr = f_open(&fil2, "out.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if ( fr != FR_OK) {
    	send_string("Failed to open out.txt for writing\r\n");
    	return 1;
    }

    fr = f_write(&fil2, testBuffer, sizeof testBuffer, &testBytes);
    f_close(&fil2);
    if( testBytes == sizeof testBuffer ){
    	send_string("Data write ok.\r\n");
    }
*/
  while (1)
    {
	  //uart.send_string("hello");
	  HAL_Delay(1000);
       // Add your code here.
    }
}




#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
}

#endif



#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
