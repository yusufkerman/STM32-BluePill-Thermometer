#ifndef DEVICES_DHT22_H_
#define DEVICES_DHT22_H_


#include "Peripherals/PeripheralManager.h"
#include "Peripherals/GpioDriver.h"
#include "Peripherals/SysTickDriver.h"


class Dht22 {
private:
    GpioDriver* gpio;
    SysTickDriver* sysTick;
    GpioPin dataPin;

    // Protocol functions
    void sendStart();           // 18ms LOW + 30μs HIGH
    bool waitResponse();        // 80μs LOW + 80μs HIGH wait
    bool readBit();            // Read single bit with timing
    uint8_t readByte();        // read 8 bit
    bool validateChecksum(uint8_t data[5]);

public:

    Dht22(PeripheralManager* peripherals, GpioPin dataPin);

    struct SensorData {
        float humidity;     // %RH
        float temperature;  // °C
        bool valid;
    };

    SensorData readSensor();   // Main func
};



#endif
