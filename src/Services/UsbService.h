#ifndef USBSERVICE_H
#define USBSERVICE_H

#include <Arduino.h>
#include <USB.h>
#include <USBHIDKeyboard.h> // custom from local lib
#include <string>
#include <M5Cardputer.h>

class UsbService {
public:
    UsbService();
    void begin();
    void end();
    void sendString(const std::string& text);
    void sendStringAndEnter(const std::string& text);
    void sendChunkedString(const std::string& data, size_t chunkSize=128, unsigned long delayBetweenChunks=50);
    bool isReady() const;
    void setLayout(const uint8_t* newLayout);

private:
    USBHIDKeyboard keyboard;
    const uint8_t* layout;
    bool initialized = false;
    uint32_t initTime;
};

#endif // USBSERVICE_H
