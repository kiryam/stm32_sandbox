#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#include "diag/Trace.h"
#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "BlinkLed.h"

using namespace std;

SD_HandleTypeDef hsd;
HAL_SD_CardInfoTypedef SDCardInfo;
UART_HandleTypeDef huart3;

static void MX_GPIO_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_USART3_UART_Init(void);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void send_string(string s){
	const uint8_t* transmitBuffer;
	transmitBuffer = reinterpret_cast<const uint8_t*>(s.data());
	HAL_UART_Transmit(&huart3, (uint8_t*) transmitBuffer, s.length(), 1000);
}

int main(int argc, char* argv[]){
	//BlinkLed led1(1, 1, false);
	//led1.powerUp();
	//led1.turnOn();
	MX_GPIO_Init();
	MX_SDIO_SD_Init();
	MX_FATFS_Init();
	MX_USART3_UART_Init();

	FIL fil;
	char line[82];
	FRESULT fr;
	FATFS FatFs;

	fr = f_mount(&FatFs, SD_Path, 1);
	if ( fr == FR_NO_FILESYSTEM ){
		send_string("NO FileSystem. Format card to FATFS\r\n");
		return 1;
	}else if( fr != FR_OK ){
		send_string("Disk mount error\r\n");
		return 1;
	}

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

  while (1)
    {
       // Add your code here.
    }
}


/* SDIO init function */
void MX_SDIO_SD_Init(void)
{
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
}

/** Pinout Configuration
*/
void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USART1 init function */
void MX_USART3_UART_Init(void)
{
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
  	huart3.Init.Mode = UART_MODE_TX_RX;
  	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  	HAL_UART_Init(&huart3);
}



#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
}

#endif



#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
