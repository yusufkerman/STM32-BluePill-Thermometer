#ifndef DEVICES_DEVICESMANAGER_H_
#define DEVICES_DEVICESMANAGER_H_


#include "Devices/Logger.h"
#include "Devices/Lcd.h"
#include "Devices/Dht22.h"


class DevicesManager{

public:

    DevicesManager(Logger* logger, Lcd* lcd,Dht22* dht22) : logger(logger), lcd(lcd), dht22(dht22) {}

    Logger* getLogger() { return logger; }
    Lcd* getLcd() { return lcd; }
    Dht22* getDht22() { return dht22; }

private:

    Logger* logger;
    Lcd* lcd;
    Dht22* dht22;

};




#endif
