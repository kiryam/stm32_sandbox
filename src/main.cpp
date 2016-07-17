//#include <stdlib.h>
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


int sat_count;
int visible_sat_count;
UART_HandleTypeDef huart6;
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

	huart6.Instance = USART6;
	huart6.Init.BaudRate = 9600;
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.Parity = UART_PARITY_NONE;
	huart6.Init.Mode = UART_MODE_TX_RX;
	huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart6.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart6);

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

	  char count[1];

	  std::string line2 = "sat=";
	  itoa(sat_count, count, 8);
	  line2 += count;
	  display.setLine2(line2);
	  osDelay(500);
  }
}



void StartPrintToUARTTask2(void const * argument) {
  TinyGPS gps;
  bool newData = false;
  uint8_t receiveBuffer[128] = {};
  std::string nmea = "";
  uart.send_string("$GPTXT,01,01,02,Start receive data\r\n");

  for(;;) {
	  osDelay(100);
	  HAL_UART_Receive_IT(&huart6, receiveBuffer, sizeof receiveBuffer);

	  for(unsigned int i=0;i<sizeof receiveBuffer; i++){
		  if (gps.encode(receiveBuffer[i])){
			  newData = true;
		  }


		  nmea+= receiveBuffer[i];
		  if( nmea.length() > 100 ){
			  nmea = "";
			  nmea.clear();
		  }
		  if(receiveBuffer[i] == '\n' && nmea.length() > 1){
			  if( nmea.length() > 10 ){
				  if ( nmea.substr(0, 7).compare("$GPGSV,") == 0 ){
					  int index_number_of_messages_end = nmea.find_first_of(',', 7);
					  if( index_number_of_messages_end != -1 ){
						  int index_message_number_end = nmea.find_first_of(',', index_number_of_messages_end+1);
						  if( index_message_number_end != -1 ){
							  int index_number_sat_in_view_end = nmea.find_first_of(',', index_message_number_end+1);
							  if( index_number_sat_in_view_end != -1 && index_number_sat_in_view_end > index_message_number_end ){
								  if( index_number_sat_in_view_end-index_message_number_end > 1 ){
									  string number_sat_in_view = nmea.substr(index_message_number_end+1, index_number_sat_in_view_end-index_message_number_end-1);
									  display.setLine1("vsat="+number_sat_in_view);
								  }
							  }else{
								  display.setLine1("vsat=0");
							  }
						  }
					  }

					  HAL_Delay(1);
					  //if( current_index == in )

					  //string buf = "";
					 // string result = "";
					  //for(int e=6; e<i; e++){
						//  if (nmea.at(e) ==','){
						//	  result = nmea.substr(6, e-6);
						//	  buf.clear();
						//  }
					 // }

				  }
			  }

			  nmea = "";
			  nmea.clear();


			  //uart.send_string(nmea);

/*

			  //double flat, flon;
			  //unsigned long age;
			  //gps.f_get_position(&flat, &flon, &age);

			  sat_count = gps.sat_count();
			  visible_sat_count = gps.visible_sat_count();
			  if( visible_sat_count > 0 ){
				  string satinfo;
				  satinfo = "$GPTXT,01,01,02,Visible satellites: ";
				  satinfo += visible_sat_count;
				  satinfo += "\r\n";
				  uart.send_string(satinfo);
				  satinfo.clear();
			  }

			  if( sat_count > 0 ){
				  string satinfo;
				  satinfo = "$GPTXT,01,01,02,Satellites: ";
				  satinfo += sat_count;
				  satinfo += "\r\n";
				  uart.send_string(satinfo);
				  satinfo.clear();
			  }
			  */
			  osDelay(300);

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
