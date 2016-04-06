#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "sdcard.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

osThreadId defaultTaskHandle;
void StartDefaultTask(void const * argument);
void StartPrintToUARTTask(void const * argument);
void StartPrintToUARTTask2(void const * argument);

Uart uart = Uart();
SDCard sdcard = SDCard(&uart);

int main(int argc, char* argv[]){
	sdcard.mount();

	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	osThreadDef(UARTPrintTask, StartPrintToUARTTask, osPriorityNormal, 0, 128);
	osThreadCreate(osThread(UARTPrintTask), NULL);

	osThreadDef(UARTPrintTask2, StartPrintToUARTTask2, osPriorityNormal, 0, 128);
	osThreadCreate(osThread(UARTPrintTask2), NULL);
	osKernelStart();

	std::string settings_cont = sdcard.get_file_cont("settings.txt");
	uart.send_string(settings_cont);

	sdcard.write_str_to_file("file1.txt", "Hello!");

    while (1) {
	  HAL_Delay(1000);
    }
}

void StartDefaultTask(void const * argument) {
  for(;;) {
    osDelay(1);
  }
}

void StartPrintToUARTTask(void const * argument) {
  for(;;) {
	  uart.send_string("async print 1\r\n");
	  osDelay(1000);
  }
}

void StartPrintToUARTTask2(void const * argument) {
  for(;;) {
	  uart.send_string("async print 2\r\n");
	  osDelay(200);
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
}

#endif

#pragma GCC diagnostic pop
