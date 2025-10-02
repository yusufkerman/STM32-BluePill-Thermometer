#ifndef DEVICES_LCD_H_
#define DEVICES_LCD_H_

#include "Peripherals/I2CDriver.h"
#include "Peripherals/SysTickDriver.h"
#include "Peripherals/PeripheralManager.h"
#include "Devices/Logger.h"
#include <stdint.h>
#include <string>


class Lcd {
public:
    // addr: 7-bit I2C address
    Lcd(PeripheralManager* peripherals,Logger* logger, uint8_t addr);

    void clearScreen();
    void write(char* txt);

	void scanI2CDevices();


private:

	Logger* logger;

    I2CDriver* i2c;
    SysTickDriver* systick;
    uint8_t address;

    void initializeLcd();

    // helper: send one byte to LCD; isData = true for character, false for command
    bool sendByte(uint8_t b, bool isData);

    // small blocking delay helper
    void smallDelayMs(uint32_t ms);
};

#endif // DEVICES_LCD_H_
