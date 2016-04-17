#include "display.h"



Display::Display ()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	// Initialize all pins connected to the WH1602 module
	GPIO_InitTypeDef GPIO_InitStruct_RS;
	GPIO_InitStruct_RS.Pin = MT_WH1602_RS_PIN;
	GPIO_InitStruct_RS.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct_RS.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(MT_WH1602_RS_PORT, &GPIO_InitStruct_RS);

	GPIO_InitTypeDef GPIO_InitStruct_RW;
	GPIO_InitStruct_RW.Pin = MT_WH1602_RW_PIN;
	GPIO_InitStruct_RW.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct_RW.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(MT_WH1602_RW_PORT, &GPIO_InitStruct_RW);

	GPIO_InitTypeDef GPIO_InitStruct_E;
	GPIO_InitStruct_E.Pin = MT_WH1602_E_PIN;
	GPIO_InitStruct_E.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct_E.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(MT_WH1602_E_PORT, &GPIO_InitStruct_E);

	GPIO_InitTypeDef GPIO_InitStruct_DB7;
	GPIO_InitStruct_DB7.Pin = MT_WH1602_DB7_PIN;
	GPIO_InitStruct_DB7.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct_DB7.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(MT_WH1602_DB7_PORT, &GPIO_InitStruct_DB7);

	GPIO_InitTypeDef GPIO_InitStruct_DB6;
	GPIO_InitStruct_DB6.Pin = MT_WH1602_DB6_PIN;
	GPIO_InitStruct_DB6.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct_DB6.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(MT_WH1602_DB6_PORT, &GPIO_InitStruct_DB6);

	GPIO_InitTypeDef GPIO_InitStruct_DB5;
	GPIO_InitStruct_DB5.Pin = MT_WH1602_DB5_PIN;
	GPIO_InitStruct_DB5.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct_DB5.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(MT_WH1602_DB5_PORT, &GPIO_InitStruct_DB5);

	GPIO_InitTypeDef GPIO_InitStruct_DB4;
	GPIO_InitStruct_DB4.Pin = MT_WH1602_DB4_PIN;
	GPIO_InitStruct_DB4.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct_DB4.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(MT_WH1602_DB4_PORT, &GPIO_InitStruct_DB4);

	FirstStage();
	Set4BitMode();
	FunctionSet(1, 0);
	DisplayOnOff(1, 0, 0);
	ClearDisplay();
	EntryMode(1, 1);
	ReturnHome();
}

void Display::FirstStage(){
	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_SET);
	DataReadWrite();
	Delay(1000);
	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_SET);
	DataReadWrite();
	Delay(1000);
	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_SET);
	DataReadWrite();
	Delay(1000);
}

void Display::Set4BitMode(){
	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	DataReadWrite();
	Delay(1000);
}

void Display::Delay(uint32_t us)
{
  volatile uint32_t i;
  i = (HAL_RCC_GetSysClockFreq()/MT_WH1602_DELAY_uS)*us;

  for (; i != 0; i--);
}

void Display::DataReadWrite()
{
	HAL_GPIO_WritePin(MT_WH1602_E_PORT, MT_WH1602_E_PIN, GPIO_PIN_SET);
	Delay(MT_WH1602_DATA_RW_DELAY);
    HAL_GPIO_WritePin(MT_WH1602_E_PORT, MT_WH1602_E_PIN, GPIO_PIN_RESET);
}

void Display::ResetPins(){
	HAL_GPIO_WritePin(MT_WH1602_RS_PORT,MT_WH1602_RS_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MT_WH1602_RW_PORT,MT_WH1602_RW_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_RESET);
}

void Display::ClearDisplay()
{
	ResetPins();
	DataReadWrite();
	Delay(1000);

	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_SET);

	DataReadWrite();
	HAL_Delay(100);
}


void Display::ReturnHome()
{
	ResetPins();
	DataReadWrite();
	Delay(1000);
	HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	DataReadWrite();
	HAL_Delay(100);
}

void Display::DisplayOnOff(uint8_t Dbit, uint8_t Cbit, uint8_t Bbit)
{
	ResetPins();
	DataReadWrite();
	Delay(1000);

	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_SET);

	if (Dbit == 1) {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_RESET);
	}

	if (Cbit == 1) {
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_RESET);
	}

	if (Bbit == 1) {
		HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_RESET);
	}

	DataReadWrite();
	Delay(1000);
}

void Display::CursorOrDisplayShift(uint8_t SCbit, uint8_t RLbit)
{
	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_SET);
	DataReadWrite();
	Delay(1000);

	ResetPins();
	if (SCbit == 1) {
		HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_RESET);
	}

	if (RLbit == 1) {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_RESET);
	}

	DataReadWrite();
	Delay(1000);
}

void Display::FunctionSet(uint8_t Nbit, uint8_t Fbit)
{
	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	DataReadWrite();

	Delay(1000);
	ResetPins();
	if (Nbit == 1) {
		HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_RESET);
	}

	if (Fbit == 1) {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_RESET);
	}

	DataReadWrite();
	Delay(1000);
}

void Display::EntryMode(uint8_t Ibit, uint8_t Sbit)
{
	ResetPins();
	DataReadWrite();
	Delay(1000);

	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_SET);

	if (Ibit == 1){
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_RESET);
	}

	if (Sbit == 1){
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_RESET);
	}

	DataReadWrite();
	Delay(1000);
}

void Display::WriteData(uint8_t data)
{
	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_RS_PORT,MT_WH1602_RS_PIN,GPIO_PIN_SET);

	if (data & BIT_7_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_RESET);
	}

	if (data & BIT_6_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_RESET);
	}

	if (data & BIT_5_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_RESET);
	}

	if (data & BIT_4_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_RESET);
	}
	DataReadWrite();
	Delay(1000);

	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_RS_PORT,MT_WH1602_RS_PIN,GPIO_PIN_SET);

	if (data & BIT_3_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_RESET);
	}

	if (data & BIT_2_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_RESET);
	}

	if (data & BIT_1_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_RESET);
	}

	if (data & BIT_0_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_RESET);
	}
	DataReadWrite();
	Delay(1000);
}

void Display::SetDDRAMAddress(uint8_t address)
{
	ResetPins();
	HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_SET);

	if (address & BIT_6_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_RESET);
	}

	if (address & BIT_5_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_RESET);
	}

	if (address & BIT_4_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_RESET);
	}

	DataReadWrite();
	Delay(1000);

	ResetPins();

	if (address & BIT_3_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB7_PORT,MT_WH1602_DB7_PIN,GPIO_PIN_RESET);
	}

	if (address & BIT_2_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB6_PORT,MT_WH1602_DB6_PIN,GPIO_PIN_RESET);
	}

	if (address & BIT_1_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB5_PORT,MT_WH1602_DB5_PIN,GPIO_PIN_RESET);
	}

	if (address & BIT_0_MASK) {
		HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(MT_WH1602_DB4_PORT,MT_WH1602_DB4_PIN,GPIO_PIN_RESET);
	}

	DataReadWrite();
	Delay(1000);
}

void Display::writeString(std::string s, uint8_t line)
{
	if (line == 2){
		SetDDRAMAddress(40);
	}else{
		SetDDRAMAddress(0);
	}

	for(unsigned int i=0; i<s.length(); i++){
		WriteData(s.c_str()[i]);
	}
}

void Display::setLine1(std::string s){
	if( line1 == s ) return;
	SetDDRAMAddress(0);
	line1 = s;
	writeString(line1, 1);
	if (line1.length() < 40){
		for(unsigned int i=0; i<40-line1.length(); i++){
			WriteData(' ');
		}
	}
}

void Display::setLine2(std::string s){
	if( line2 == s ) return;

	SetDDRAMAddress(40);
	line2 = s;
	writeString(line2, 2);
	if (line2.length() < 40){
		for(unsigned int i=0; i<40-line2.length(); i++){
			WriteData(' ');
		}
	}
}
