#ifndef SERVICECONFIGS_SERVICECONFIGS_H_
#define SERVICECONFIGS_SERVICECONFIGS_H_


#include "ServiceContainer.h"

#include "stm32f103xb.h"


#include "Peripherals/UsartDriver.h"
#include "Peripherals/GpioDriver.h"
#include "Peripherals/SysTickDriver.h"
#include "Peripherals/I2CDriver.h"
#include "Peripherals/PeripheralManager.h"

#include "Devices/Logger.h"
#include "Devices/Lcd.h"
#include "Devices/Dht22.h"
#include "Devices/DevicesManager.h"


class ServiceConfigs{

public:
	ServiceConfigs(ServiceContainer* serviceContainer) : serviceContainer(serviceContainer) {}

	void ConfigurePeripherals();
	void ConfgiureDevices();
	void ConfigureLcd();

	~ServiceConfigs();

private:

	ServiceContainer* serviceContainer;
};


#endif
