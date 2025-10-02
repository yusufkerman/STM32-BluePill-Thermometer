#include "Devices/Dht22.h"



Dht22::Dht22(PeripheralManager* peripherals,GpioPin dataPin){

	gpio = peripherals->getGpio();
	sysTick = peripherals->getSysTick();
	this->dataPin = dataPin;


	gpio->setPinMode(dataPin.port, dataPin.pin,PinMode::Input,PinConfig::PullUpPullDown);
	gpio->writePin(dataPin.port, dataPin.pin, true);

}
void Dht22::sendStart(){

	gpio->setPinMode(dataPin.port, dataPin.pin, PinMode::Output10MHz, PinConfig::OutputPP);

	//Wake up DHT22: 18mslow
	gpio->writePin(dataPin.port, dataPin.pin, false); //LOW
	sysTick->delay_ms(18);

	// 2. Ready signal to DHT22: 30μs HIGH
	gpio->writePin(dataPin.port, dataPin.pin, true); // HIGH
	sysTick->delay_us(30);


	// 3. Set mode to input, DHT22 will take the control.
	gpio->setPinMode(dataPin.port, dataPin.pin, PinMode::Input, PinConfig::PullUpPullDown);
	gpio->writePin(dataPin.port, dataPin.pin, true);
}
bool Dht22::waitResponse(){

	// 1. DHT22 80μs LOW
	uint32_t startTime = sysTick->millis();
	while (gpio->readPin(dataPin.port, dataPin.pin) == false){
		if ((sysTick->millis() - startTime) >= 1) { // 1ms timeout
			return false;
		}
	}

	// 1. DHT22 80μs HIGH
	startTime = sysTick->millis();
	while (gpio->readPin(dataPin.port, dataPin.pin) == true){
		if ((sysTick->millis() - startTime) >= 1) { // 1ms timeout
			return false;
		}
	}

	// DHT22 is ready.
	return true;
}
bool Dht22::readBit(){

	uint32_t timeout;

	// All bits start 50μs LOW
	timeout = 100;
	while (gpio->readPin(dataPin.port, dataPin.pin) == false) {
		sysTick->delay_us(1);
	    if (--timeout == 0) return false;
	}

	// Measure HIGH time
	// Bit 0: 26-28μs HIGH
	// Bit 1: 70μs HIGH
	uint32_t highTime = 0;
	timeout = 100;
	while (gpio->readPin(dataPin.port, dataPin.pin) == true){
		sysTick->delay_us(1);

		highTime++;

		if (--timeout == 0) return false;
	}

	return (highTime > 40);

}
uint8_t Dht22::readByte() {
    uint8_t result = 0;

    // To MSB from LSB
    for (int i = 0; i < 8; i++) {
        result <<= 1; // Shift to left previous bits.

        if (readBit()) {
            result |= 0b00000001; // If bit is 1, set LSB bit to 1
        }
        // If bit is 0, do nothing
    }

    return result;
}
bool Dht22::validateChecksum(uint8_t data[5]) {
    // Sum first 4 bytes.
    uint8_t calculated = data[0] + data[1] + data[2] + data[3];

    // Compare with 5th byte.
    uint8_t received = data[4];

    return (calculated == received);
}
Dht22::SensorData Dht22::readSensor() {

	SensorData result = {0.0f, 0.0f, false};
	uint8_t data[5] = {0};

	//Send start signal.
	sendStart();

	//Wait response from DHT22
	if (!waitResponse()){
		return result;
	}

	//40 bit = 5 byte
	for (int i = 0; i < 5; i++) {
		data[i] = readByte();
	}

	// 4. Checksum control
	if (!validateChecksum(data)) {
		return result;
	}

	// 5. Get humidity (first 2 byte)
	uint16_t humidity_raw = (data[0] << 8) | data[1];
	result.humidity = humidity_raw / 10.0f;

	// 6. Get temperature (3-4 bytes)
	uint16_t temp_raw = ((data[2] & 0x7F) << 8) | data[3];
	result.temperature = temp_raw / 10.0f;

	// 7. Negatice tempereature control.
	if (data[2] & 0x80) {
		result.temperature = -result.temperature;
	}

	// 8. Data valid
	result.valid = true;
	return result;

}




