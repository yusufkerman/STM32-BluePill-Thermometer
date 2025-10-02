#ifndef PERIPHERALS_PERIPHERALMANAGER_H_
#define PERIPHERALS_PERIPHERALMANAGER_H_


#include <vector>
#include "Peripherals/GpioDriver.h"
#include "Peripherals/UsartDriver.h"
#include "Peripherals/SysTickDriver.h"
#include "Peripherals/I2CDriver.h"


class PeripheralManager {
public:

	PeripheralManager(UsartDriver* usartModule, GpioDriver* gpioDriver, SysTickDriver* systckDriver, I2CDriver* i2cDriver)
	 : usart(usartModule) ,gpio(gpioDriver), systick(systckDriver), i2c(i2cDriver) {}

	UsartDriver* getUsart() { return usart; }
	GpioDriver* getGpio() { return gpio; }
	SysTickDriver* getSysTick() { return systick; }
	I2CDriver* getI2C() { return i2c; }

	~PeripheralManager();

private:

	UsartDriver* usart;
	GpioDriver* gpio;
	SysTickDriver* systick;
	I2CDriver* i2c;

};

#endif
