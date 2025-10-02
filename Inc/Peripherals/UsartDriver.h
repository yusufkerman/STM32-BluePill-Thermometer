#ifndef PERIPHERALS_USARTDRIVER_H_
#define PERIPHERALS_USARTDRIVER_H_

#include <string>

#include <Peripherals/GpioDriver.h>
#include <stdint.h>
#include "stm32f103xb.h"




struct UsartPinMap{

	USART_TypeDef* usart;
	GPIO_TypeDef* port;
	bool remap;

	GpioPin tx;
	GpioPin rx;
};

enum class TxOutputSpeed{
	 Output10MHz,
	 Output2MHz,
	 Output50MHz
};

static const UsartPinMap usartMap[] = {

		//USART1
		{USART1,GPIOA,false, {GPIOA,9}, {GPIOA,10}},
		{USART1,GPIOB,true, {GPIOB,6}, {GPIOB,7}},

		//USART2
		{USART2,GPIOA,false, {GPIOA,2}, {GPIOA,3}},
		{USART2,GPIOD,true, {GPIOD,5}, {GPIOD,6}},

		//USART3
		{USART3,GPIOB,false, {GPIOB,10}, {GPIOB,11}},
		{USART3,GPIOC,true, {GPIOC,10}, {GPIOC,11}}
};


class UsartDriver {

public:
	UsartDriver(USART_TypeDef* usartModule, uint8_t remapOption = 0, GpioDriver* gpioDriver = nullptr)
	: usart(usartModule), remap(remapOption), gpio(gpioDriver) {}

	//Should be called from actual IRQ method.
	static void IRQHandlerStatic(UsartDriver* driver);

	void (*rxCallback)(char) = nullptr;

	//Enable usart module depends on configuration.
	void init(TxOutputSpeed txSpeed,uint32_t clk, uint32_t baudrate);

	//Enable interrupt for relevant module.
	void enableInterrupt(IRQn_Type irqNumber);

	//Set interrupt callback function.
	void setRxCallback(void (*cb)(char)) { rxCallback = cb; }

	USART_TypeDef* getUsart() { return usart; }
	uint8_t getRemapOption()  { return remap; }

	void write(uint8_t data);
	void writeString(const std::string& str);


private:
	GpioDriver* gpio;
	USART_TypeDef* usart;
	uint8_t remap;

	uint32_t clk;
	uint32_t baudrate;


	PinMode mapTxSpeedToPinMode(TxOutputSpeed speed);
	uint16_t calculate_baudrate(uint32_t clockFreq, uint32_t baudrate);
	void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t clockFreq, uint32_t baudrate);


};



#endif
