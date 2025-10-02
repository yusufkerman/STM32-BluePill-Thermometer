#include <stdint.h>
#include "stm32f103xb.h"

#include "ServiceContainer.h"
#include "ServiceConfigs/ServiceConfigs.h"
#include "Devices/DevicesManager.h"
#include "Devices/Logger.h"
#include "Peripherals/PeripheralManager.h"


void delays(int s) {
    volatile uint32_t i;
    for (i = 0; i < 800000UL * s; i++) {
        __asm("nop");
    }
}

int main(void)
{
    ServiceContainer* container = new ServiceContainer();
    ServiceConfigs* services = new ServiceConfigs(container);

    services->ConfigurePeripherals();
    services->ConfgiureDevices();

    DevicesManager* deviceService = container->resolve<DevicesManager>("devicesManager");
    PeripheralManager* peripherals = container->resolve<PeripheralManager>("peripheralManager");

    // DHT verisi
    auto data = deviceService->getDht22()->readSensor(); // başlangıç için
    uint32_t lastDhtRead = peripherals->getSysTick()->millis();

    while (1) {
        // 2 saniyede bir DHT22 oku
        uint32_t now = peripherals->getSysTick()->millis();
        if (now - lastDhtRead > 2000) {
            data = deviceService->getDht22()->readSensor();
            lastDhtRead = now;
        }

        // LCD temizle ve yaz
        deviceService->getLcd()->clearScreen();

        if (data.valid) {
            std::string line = "Nem: " + std::to_string(data.humidity) +
                               "% Temp: " + std::to_string(data.temperature) + "C";
            deviceService->getLcd()->write(const_cast<char*>(line.c_str()));
        } else {
            deviceService->getLcd()->write(const_cast<char*>("Hata: Sensor Error"));
            deviceService->getLogger()->log("HATA: DHT22 Sensor Error");
        }
    }
}


