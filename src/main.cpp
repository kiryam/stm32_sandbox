#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "stm32f4xx_hal.h"
#include "fatfs.h"


using namespace std;

SD_HandleTypeDef hsd;
HAL_SD_CardInfoTypedef SDCardInfo;

static void MX_GPIO_Init(void);
static void MX_SDIO_SD_Init(void);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"



int main(int argc, char* argv[]){
	MX_GPIO_Init();
	MX_SDIO_SD_Init();
	MX_FATFS_Init();

	Uart uart = Uart();


/*
	FIL fil;
	char line[82];
	FRESULT fr;
	FATFS FatFs;



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
	  uart.send_string("hello");
	  HAL_Delay(1000);
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


}


#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{
}

#endif



#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
