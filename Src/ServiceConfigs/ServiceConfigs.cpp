#include "ServiceConfigs/ServiceConfigs.h"



void ServiceConfigs::ConfigurePeripherals(){

	GpioDriver* gpio = new GpioDriver();
	UsartDriver* usart2 = new UsartDriver(USART2,false,gpio);
	SysTickDriver* systck = new SysTickDriver();
	systck->init(8000000);
	I2CDriver* i2c2 = new I2CDriver(I2C2,gpio,systck,false);
	i2c2->init(8000000, 100000);


	PeripheralManager* manager = new PeripheralManager(usart2,gpio,systck,i2c2);

	serviceContainer->registerService<PeripheralManager>("peripheralManager", manager);

}

void ServiceConfigs::ConfgiureDevices(){

	GpioPin dhtDataPin = {GPIOA,11};

	PeripheralManager* peripheralManager = serviceContainer->resolve<PeripheralManager>("peripheralManager");
	Logger* logger = new Logger(peripheralManager);
	Lcd* lcd = new Lcd(peripheralManager,logger,0x27);
	Dht22* dht22 = new Dht22(peripheralManager,dhtDataPin);

	DevicesManager* devicesManager = new DevicesManager(logger,lcd,dht22);

	serviceContainer->registerService("devicesManager", devicesManager);

}
