#include <Devices/Logger.h>


Logger::Logger(PeripheralManager* peripheralManager){

	this->peripheralManager = peripheralManager;

	usart = peripheralManager->getUsart();

	usart->init(TxOutputSpeed::Output10MHz, 8000000, 115200);

}

void Logger::log(const std::string& text){

	usart->writeString(text);

}
