#ifndef PERIPHERALS_I2CDRIVER_H_
#define PERIPHERALS_I2CDRIVER_H_


#include <stdint.h>
#include "stm32f103xb.h"

#include "SysTickDriver.h"
#include "GpioDriver.h"


struct I2CPinMap{

	I2C_TypeDef* i2c;
	bool remap;

	GpioPin scl;
	GpioPin sda;

};
static const I2CPinMap I2CMap[]{

		{I2C1, false, {GPIOB,6}, {GPIOB,7}},
		{I2C1, true, {GPIOB,8}, {GPIOB,9}},
		{I2C2, false, {GPIOB,10}, {GPIOB,11}}

};



class I2CDriver{

public:

	I2CDriver(I2C_TypeDef* i2cModule, GpioDriver* gpioDriver, SysTickDriver* sysTickDriver, bool remapOption)
	 : i2c(i2cModule), gpio(gpioDriver), sysTick(sysTickDriver), remap(remapOption) {}

	void init(int apb1_freq_mhz,uint32_t i2c_speed_hz);

	bool startCommunication(uint8_t deviceAddress, bool isRead);
	bool stopCommunication();

	bool writeByte(uint8_t data);
	bool writeBuffer(const uint8_t* buffer, uint32_t length);

	I2C_TypeDef* getI2C() { return i2c; }

private:

	I2C_TypeDef* i2c;
	GpioDriver* gpio;
	SysTickDriver* sysTick;
	bool remap;

	void config_clock();
	void config_i2c(int apb1_freq_mhz,uint32_t i2c_speed_hz);


};




#endif
