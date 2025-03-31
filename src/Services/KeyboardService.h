#ifndef KEYBOARDSERVICE_H
#define KEYBOARDSERVICE_H

#include <Arduino.h>
#include "UsbService.h"
#include "BleService.h"
#include <string>

// Enum to represent the keyboard connection type
enum class KeyboardConnectionType {
    USB,
    BLE,
    NONE
};

class KeyboardService {
public:
    KeyboardService();
    void begin();
    void end();
    void setBleEnabled(bool enabled);
    bool isBleEnabled() const;
    bool isBleConnected() const;
    void setLayout(const uint8_t* newLayout);
    void sendString(const std::string& text);
    void sendChunkedString(const std::string& data, size_t chunkSize=128, unsigned long delayBetweenChunks=50);
    KeyboardConnectionType getActiveConnectionType() const;
    bool isReady() const;
    void clearBleBondingData();
    bool hasBondedBleDevices() const;

private:
    UsbService usbService;
    BleService bleService;
    bool bleEnabled;
};

#endif // KEYBOARDSERVICE_H
