#ifndef DEVICES_LOGGER_H_
#define DEVICES_LOGGER_H_

#include <string>

#include "Peripherals/PeripheralManager.h"


class Logger {
public:
	Logger(PeripheralManager* peripheralManager);

	void log(const std::string& text);


private:

	PeripheralManager* peripheralManager;
	UsartDriver* usart;
};

#endif
