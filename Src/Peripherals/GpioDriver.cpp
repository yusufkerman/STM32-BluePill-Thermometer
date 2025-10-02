#include <Peripherals/GpioDriver.h>



void GpioDriver::enableClock(GPIO_TypeDef* port){

	uint32_t offset = (uint32_t)port - GPIOA_BASE;

	//0x400 = 1024byte, diff with ports addresses
	uint32_t bit = 2 + (offset / 0x400) ;
	RCC->APB2ENR |= (1U << bit);
}

void GpioDriver::setPinMode(GPIO_TypeDef* port, uint8_t pin, PinMode mode, PinConfig cnf){

	uint32_t tempBits = (static_cast<uint32_t>(cnf) << 2) | static_cast<uint32_t>(mode);

	if (pin <= 7){
		// CRL
		uint32_t mask = (0xF << (4 * pin));
		uint32_t config = (tempBits << (4 * pin));
		port->CRL &= ~mask;
		port->CRL |= config;
	}
	else{
		// CRH
		pin -= 8;
		uint32_t mask = (0xF << (4 * pin));
		uint32_t config = (tempBits << (4 * pin));
		port->CRH &= ~mask;
		port->CRH |= config;
	}
}

void GpioDriver::writePin(GPIO_TypeDef* port, uint8_t pin, bool value){

	 if (value)
	    port->BSRR = (1 << pin);          // pin HIGH (atomic)
	 else
	    port->BSRR = (1 << (pin + 16));   // pin LOW (atomic)

}

bool GpioDriver::readPin(GPIO_TypeDef* port, uint8_t pin){

	return (port->IDR & (1 << pin)) != 0;

}

void GpioDriver::togglePin(GPIO_TypeDef* port, uint8_t pin){

	if(port->ODR & (1 << pin))
	   port->BSRR = (1 << (pin + 16));   // reset
	else
	   port->BSRR = (1 << pin);          // set

}
