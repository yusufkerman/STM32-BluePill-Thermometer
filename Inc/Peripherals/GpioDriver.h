#ifndef PERIPHERALS_GPIODRIVER_H_
#define PERIPHERALS_GPIODRIVER_H_

#include <stdint.h>
#include "stm32f103xb.h"




enum class PinMode {
    Input  = 0b00,   // MODE[1:0]
    Output10MHz = 0b01,
    Output2MHz  = 0b10,
    Output50MHz = 0b11
};

enum class PinConfig {
    Analog          = 0b00,  // CNF[1:0] input mode
    FloatingInput   = 0b01,
    PullUpPullDown  = 0b10,
    ReservedInput   = 0b11,

    OutputPP        = 0b00,  // CNF[1:0] output mode
    OutputOD        = 0b01,
    AFOutputPP      = 0b10,
    AFOutputOD      = 0b11
};


struct GpioPin {
    GPIO_TypeDef* port;
    uint8_t pin;
};

class GpioDriver{

public:

	GpioDriver() {}

	//Enable clock access for relevant GPIO port.
	void enableClock(GPIO_TypeDef* port);

	//Configure the pin mode for relevant pin and port.
	void setPinMode(GPIO_TypeDef* port, uint8_t pin, PinMode mode, PinConfig cnf);

	//Set pin logical value 0-1
	void writePin(GPIO_TypeDef* port, uint8_t pin, bool value);

	//Read pin logical value 0-1
	bool readPin(GPIO_TypeDef* port, uint8_t pin);

	//Toggle pin logical state
	void togglePin(GPIO_TypeDef* port, uint8_t pin);

};


#endif
