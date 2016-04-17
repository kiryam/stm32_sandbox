#ifndef HDISPLAY_H_
#define HDISPLAY_H_

#include "stm32f4xx_hal.h"
#include "string";

#define MT_WH1602_DELAY_uS						(10000000)
#define MT_WH1602_DATA_RW_DELAY				(2)

// Set RS port
#define MT_WH1602_RS_PORT							(GPIOC)
// Set RS pin
#define MT_WH1602_RS_PIN							(GPIO_PIN_1)

// Set RW port
#define MT_WH1602_RW_PORT							(GPIOC)
// Set RW pin
#define MT_WH1602_RW_PIN							(GPIO_PIN_3)

// Set E port
#define MT_WH1602_E_PORT							(GPIOA)
// Set E pin
#define MT_WH1602_E_PIN					  		(GPIO_PIN_1)

// Set DB7 port
#define MT_WH1602_DB7_PORT						(GPIOA)
// Set DB7 pin
#define MT_WH1602_DB7_PIN	 						(GPIO_PIN_3)

// Set DB6 port
#define MT_WH1602_DB6_PORT						(GPIOA)
// Set DB6 pin
#define MT_WH1602_DB6_PIN							(GPIO_PIN_5)

// Set DB5 port
#define MT_WH1602_DB5_PORT						(GPIOA)
// Set DB5 pin
#define MT_WH1602_DB5_PIN							(GPIO_PIN_7)

// Set DB4 port
#define MT_WH1602_DB4_PORT						(GPIOC)
// Set DB4 pin
#define MT_WH1602_DB4_PIN							(GPIO_PIN_5)

// Bit masks for different bits in byte
#define BIT_7_MASK										(0x80)
#define BIT_6_MASK										(0x40)
#define BIT_5_MASK										(0x20)
#define BIT_4_MASK										(0x10)
#define BIT_3_MASK										(0x08)
#define BIT_2_MASK										(0x04)
#define BIT_1_MASK										(0x02)
#define BIT_0_MASK										(0x01)

class Display
{
public:
	Display ();
	void setLine1(std::string s);
	void setLine2(std::string s);

private:
	void DisplayOnOff(uint8_t Dbit, uint8_t Cbit, uint8_t Bbit);
	void Delay(uint32_t us);
	void FirstStage();
	void DataReadWrite();
	void ResetPins();
	void Set4BitMode();
	void FunctionSet(uint8_t Nbit, uint8_t Fbit);
	void EntryMode(uint8_t Ibit, uint8_t Sbit);
	void SetDDRAMAddress(uint8_t address);
	void CursorOrDisplayShift(uint8_t SCbit, uint8_t RLbit);
	void ReturnHome();
	void ClearDisplay();
	void WriteData(uint8_t data);
	void writeString(std::string s, uint8_t line);

	std::string line1 = "";
	std::string line2 = "";
};


#endif
