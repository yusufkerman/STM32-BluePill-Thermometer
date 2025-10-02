#include "Peripherals/I2CDriver.h"




void I2CDriver::init(int apb1_freq_mhz,uint32_t i2c_speed_hz){


	config_clock();

	config_i2c(apb1_freq_mhz,i2c_speed_hz);



}
void I2CDriver::config_clock(){

	//Enable clock access for relevant I2C module
	if (i2c == I2C1)
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	else if (i2c == I2C2)
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

	//I2C1 and I2C2 modules use same gpio port
	gpio->enableClock(GPIOB);

	for (I2CPinMap item : I2CMap){
		if (item.i2c == i2c && item.remap == remap){

			GpioPin sclPin = item.scl;
			GpioPin sdaPin = item.sda;

			// SCL pin configuration
			gpio->setPinMode(sclPin.port, sclPin.pin,
				                           PinMode::Output50MHz, PinConfig::AFOutputOD);

			// SDA pin configuration
			gpio->setPinMode(sdaPin.port, sdaPin.pin,
				                           PinMode::Output50MHz, PinConfig::AFOutputOD);
			break;

		}
	}

}
void I2CDriver::config_i2c(int apb1_freq_mhz,uint32_t i2c_speed_hz){

	// Disable the i2c module first
	i2c->CR1 &= ~I2C_CR1_PE;

	// Set APB1 bus freq
	i2c->CR2 &= ~I2C_CR2_FREQ;
	i2c->CR2 |= apb1_freq_mhz;

	uint32_t apb1_freq_hz = apb1_freq_mhz * 1000000;
	uint16_t ccrVal;
	uint8_t triseVal;

	if (i2c_speed_hz <= 100000) {
		// STANDARD MODE
	    ccrVal = apb1_freq_hz / (2 * i2c_speed_hz);

	    // Rise time
	    triseVal = (apb1_freq_mhz * 1000 / 1000) + 1;  // Simplified: apb1_freq_mhz + 1

		// Clear fast mode bit
		i2c->CCR &= ~I2C_CCR_FS;

	} else {
		// FAST MODE
		ccrVal = apb1_freq_hz / (3 * i2c_speed_hz);  // DUTY=0 (default)

		// Rise time: max 300ns
		triseVal = (apb1_freq_mhz * 300 / 1000) + 1;

		// Set fast mode bit
		i2c->CCR |= I2C_CCR_FS;
	}

	// Set CCR value
	i2c->CCR &= ~I2C_CCR_CCR;
	i2c->CCR |= (ccrVal & 0xFFF);  // 12-bit mask

	// Set rise time
	i2c->TRISE = triseVal & 0x3F;  // 6-bit mask

	// Enable the i2c module
	i2c->CR1 |= I2C_CR1_PE;

}
bool I2CDriver::startCommunication(uint8_t deviceAddress, bool isRead){

	//Primative timeout control
	uint32_t start = sysTick->millis();

	//Generate start condition
	i2c->CR1 |= I2C_CR1_START;

	//Wait until start condition generated;
	while (!(i2c->SR1 & I2C_SR1_SB)) {
		if ((sysTick->millis() - start) >= 100) return false; // 100 ms default
	}

	//Write address to register.
	uint8_t addr_byte = (deviceAddress << 1) | (isRead ? 1 : 0);
	i2c->DR = addr_byte;

	start = sysTick->millis();
	while (1){
		uint32_t sr1 = i2c->SR1;

		if (sr1 & I2C_SR1_ADDR){
			//Slave give ACK
			//Clear ADDR
			(void)i2c->SR1;
			(void)i2c->SR2;

			return true;
		}

		if (sr1 & I2C_SR1_AF){
			//ACK Failure
			//Clear AF
			i2c->SR1 &= ~I2C_SR1_AF;
			return false;
		}

		if ((sysTick->millis() - start) >= 100) {
			return false; // timeout 100 ms
		}
	}
}
bool I2CDriver::stopCommunication() {
    // Generate STOP condition
    i2c->CR1 |= I2C_CR1_STOP;

    // Wait until STOP condition is generated
    uint32_t start = sysTick->millis();
    while (i2c->CR1 & I2C_CR1_STOP) {
        if ((sysTick->millis() - start) >= 100) {
            return false; // timeout
        }
    }
    return true;
}
bool I2CDriver::writeByte(uint8_t data){

	uint32_t start = sysTick->millis();

	// Put data into data register
	i2c->DR = data;

	// Wait until TXE (Transmit Data Register Empty)
	while (!(i2c->SR1 & I2C_SR1_TXE)) {
		if ((sysTick->millis() - start) >= 100) {
			return false; // timeout
	    }
	}

	return true;

}
bool I2CDriver::writeBuffer(const uint8_t* buffer, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        if (!writeByte(buffer[i])) {
            return false; // if one fails, stop
        }
    }
    return true;
}


