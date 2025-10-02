#include "Devices/Lcd.h"


Lcd::Lcd(PeripheralManager* peripherals,Logger* logger, uint8_t addr){

	address = addr;
	this->logger = logger;

	i2c = peripherals->getI2C();
	systick = peripherals->getSysTick();

	// ✅ LCD INITIALIZE ET!
	initializeLcd();
}
void Lcd::scanI2CDevices() {
    std::string result = "I2C Scan Results:\n";

    for (uint8_t addr = 1; addr < 127; addr++) {
        if (i2c->startCommunication(addr, false)) {
            // Device found!
            char hexStr[20];
            sprintf(hexStr, "0x%02X (%d)", addr, addr); // HEX ve decimal ikisi de

            result += "Device found at: ";
            result += hexStr;
            result += "\n";

            // Send STOP condition
            i2c->getI2C()->CR1 |= I2C_CR1_STOP;
            systick->delay_ms(10);
        }
        systick->delay_ms(1);
    }

    if (result == "I2C Scan Results:\n") {
        result += "No I2C devices found!\n";
    }

    logger->log(result);
}
void Lcd::initializeLcd() {
    smallDelayMs(50); // Power-on delay

    // 4-bit mode initialization sequence
    sendByte(0x03, false); smallDelayMs(5);
    sendByte(0x03, false); smallDelayMs(1);
    sendByte(0x03, false); smallDelayMs(1);
    sendByte(0x02, false); smallDelayMs(1); // 4-bit mode

    // Function set: 4-bit, 2 line, 5x8 dots
    sendByte(0x28, false); smallDelayMs(1);

    // Display control: display on, cursor off, blink off
    sendByte(0x0C, false); smallDelayMs(1);

    // Clear display
    sendByte(0x01, false); smallDelayMs(2);

    // Entry mode set: increment cursor, no shift
    sendByte(0x06, false); smallDelayMs(1);
}
void Lcd::smallDelayMs(uint32_t ms) {
    systick->delay_ms(ms);
}
void Lcd::clearScreen(){

	sendByte(0x01, false); // 0x01 = clear command
	smallDelayMs(2);        // LCD clear needs >1.5ms

}
void Lcd::write(char* txt){
	while (*txt) {
		sendByte((uint8_t)(*txt), true);
	    txt++;
	}
}
bool Lcd::sendByte(uint8_t b, bool isData) {
    // Start
    if (!i2c->startCommunication(address, false)) {
        return false;
    }

    uint8_t highNibble = b & 0b11110000;
    uint8_t lowNibble = (b << 4) & 0b11110000;

    // ✅ DOĞRU BIT POZİSYONLARI:
    uint8_t rsBit = isData ? 0b00000001 : 0b00000000; // P0 = RS
    uint8_t blBit = 0b00001000; // P3 = Backlight
    uint8_t enBit = 0b00000100; // P2 = Enable

    // High nibble
    uint8_t byteToSend = highNibble | rsBit | blBit | enBit;
    if (!i2c->writeByte(byteToSend)) return false;
    smallDelayMs(1);

    byteToSend = highNibble | rsBit | blBit;
    if (!i2c->writeByte(byteToSend)) return false;
    smallDelayMs(1);

    // Low nibble
    byteToSend = lowNibble | rsBit | blBit | enBit;
    if (!i2c->writeByte(byteToSend)) return false;
    smallDelayMs(1);

    byteToSend = lowNibble | rsBit | blBit;
    if (!i2c->writeByte(byteToSend)) return false;
    smallDelayMs(1);

    if (!i2c->stopCommunication()){
        return false;
    }

    return true;
}
