#include "UsbService.h"

UsbService::UsbService() 
    : keyboard(), layout(KeyboardLayout_en_US), initialized(false), initTime(0) {}

void UsbService::setLayout(const uint8_t* newLayout) {
    layout = newLayout;
}

void UsbService::begin() {
    if (!initialized) {
        USB.begin();
        keyboard.begin(layout);
        initialized = true;
        initTime = millis(); // HID needs approx 1.5sec to initialize
    }
}

void UsbService::end() {
    keyboard.end();
}

void UsbService::sendString(const std::string& text) {
    // We wait 1.5sec for hid init
    while (millis() - initTime < 1500) {
        delay(10);
    }
    
    keyboard.releaseAll();
    for (const char& c : text) {
        keyboard.write(c);
    }
}

void UsbService::sendStringAndEnter(const std::string& text) {
    sendString(text);
    keyboard.write(KEY_RETURN);
}

bool UsbService::isReady() const {
    return initialized;
}

void UsbService::sendChunkedString(const std::string& data, size_t chunkSize, unsigned long delayBetweenChunks) {
    size_t totalLength = data.length();
    size_t sentLength = 0;

    while (sentLength < totalLength) {
        size_t remainingLength = totalLength - sentLength;
        size_t currentChunkSize = (remainingLength > chunkSize) ? chunkSize : remainingLength;

        std::string chunk = data.substr(sentLength, currentChunkSize);
        sendString(chunk);

        sentLength += currentChunkSize;
        delay(delayBetweenChunks);
    }
}
