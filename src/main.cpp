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

	sdcard.write_str_to_file("file1.txt", "Hello!");

    while (1) {
	  HAL_Delay(1000);
    }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
}

#endif

#pragma GCC diagnostic pop
