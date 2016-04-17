#include <MT_WH1602.h.bak>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "sdcard.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "sstream"
#include "TinyGPS.h"
#include "display.h"


using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


int counter;
UART_HandleTypeDef huart2;
osThreadId defaultTaskHandle;
void StartDefaultTask(void const * argument);
void StartPrintToUARTTask(void const * argument);
void StartPrintToUARTTask2(void const * argument);
char itoa( int value, char* result, int base );

Uart uart = Uart();
SDCard sdcard = SDCard(&uart);

Display display = Display();

int main(int argc, char* argv[]){
 	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart2);
*/
	//sdcard.mount();

	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	osThreadDef(UARTPrintTask, StartPrintToUARTTask, osPriorityNormal, 0, 128);
	osThreadCreate(osThread(UARTPrintTask), NULL);

	osThreadDef(UARTPrintTask2, StartPrintToUARTTask2, osPriorityNormal, 0, 128);
	osThreadCreate(osThread(UARTPrintTask2), NULL);


	osKernelStart();

	//std::string settings_cont = sdcard.get_file_cont("settings.txt");
	//uart.send_string(settings_cont);

	//sdcard.write_str_to_file("file1.txt", "Hello!");

}

void StartDefaultTask(void const * argument) {
  for(;;) {

    osDelay(1);

  }
}

void StartPrintToUARTTask(void const * argument) {
  for(;;) {
	  counter++;
	  display.setLine1("Hello");
	  std::string line2 = "World! cnt=";
	  char count[1];
	  itoa(counter, count, 8);
	  line2 += count;
	  display.setLine2(line2);
	  osDelay(500);
  }
}

void StartPrintToUARTTask2(void const * argument) {
  TinyGPS gps;
  bool newData = false;
  uint8_t receiveBuffer[82] = {};
  std::string nmea;
  uart.send_string("$GPTXT,01,01,02,Start receive data\r\n");

  for(;;) {
	  osDelay(100);
	  HAL_UART_Receive_IT(&huart2, receiveBuffer, sizeof receiveBuffer);

	  for(unsigned int i=0;i<sizeof receiveBuffer; i++){
		  if (gps.encode(receiveBuffer[i])){
			  newData = true;
		  }

		  nmea+= receiveBuffer[i];
		  if(receiveBuffer[i] == '\n' && nmea.length() > 1){
			  uart.send_string(nmea);
			  nmea = "";
			  nmea.clear();

			  //double flat, flon;
			  //unsigned long age;
			  //gps.f_get_position(&flat, &flon, &age);

			  if( gps.sat_count() > 0 ){
				  string satinfo;
				  satinfo = "$GPTXT,01,01,02,Satellites: ";
				  satinfo += gps.sat_count();
				  satinfo += "\r\n";
				  uart.send_string(satinfo);
				  satinfo.clear();
			  }

			  osDelay(300);
		  }
		  if (nmea.length() > 512 ){
			  nmea.clear();
		  }
	  }
  }
}


char itoa( int value, char* result, int base )
{
  // check that the base if valid
  if (base < 2 || base > 36) { *result = '\0'; return 0; }

  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;

  do
  {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );

  // Apply negative sign
  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while(ptr1 < ptr)
  {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return 1;
}
#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
}

#endif

#pragma GCC diagnostic pop
