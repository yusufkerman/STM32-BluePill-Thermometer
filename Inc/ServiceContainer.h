#ifndef SERVICECONTAINER_H_
#define SERVICECONTAINER_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "Peripherals/GpioDriver.h"
#include "Peripherals/UsartDriver.h"
#include "Peripherals/PeripheralManager.h"

#include "Devices/Logger.h"
#include "Devices/DevicesManager.h"

class ServiceContainer {
public:
    ServiceContainer() {}
    virtual ~ServiceContainer() {}

    template<typename T>
    void registerService(const std::string& name, T* instance) {
        auto key = typeKey<T>();
        services[key][name] = static_cast<void*>(instance);
    }

    template<typename T>
    T* resolve(const std::string& name) {
        auto key = typeKey<T>();
        auto itType = services.find(key);
        if (itType != services.end()) {
            auto itName = itType->second.find(name);
            if (itName != itType->second.end()) {
                return static_cast<T*>(itName->second);
            }
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, std::unordered_map<std::string, void*>> services;

    template<typename T>
    std::string typeKey() {
        return typeidName<T>();
    }

    template<typename T>
    static std::string typeidName();
};

// ---- specialization'lar tam burada olacak ----
template<>
inline std::string ServiceContainer::typeidName<GpioDriver>() { return "GpioDriver"; }

template<>
inline std::string ServiceContainer::typeidName<UsartDriver>() { return "UsartDriver"; }

template<>
inline std::string ServiceContainer::typeidName<PeripheralManager>() { return "PeripheralManager"; }

template<>
inline std::string ServiceContainer::typeidName<Logger>() { return "Logger"; }

template<>
inline std::string ServiceContainer::typeidName<DevicesManager>() { return "DevicesManager"; }

#endif
