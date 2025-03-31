#ifndef BLESERVICE_H
#define BLESERVICE_H

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <NimBLEHIDDevice.h>
#include <NimBLECharacteristic.h>
#include <NimBLEServer.h>
#include <string>
#include <vector>
#include <Preferences.h>

class BleKeyboardCallbacks : public NimBLEServerCallbacks {
public:
    BleKeyboardCallbacks();
    void onConnect(NimBLEServer* server);
    void onDisconnect(NimBLEServer* server);
    void onAuthenticationComplete(NimBLEConnInfo& connInfo);
    bool isConnected() const;
    bool checkConnection(NimBLEServer* server) const;

private:
    bool connected;
};

class BleService {
public:
    BleService();
    void begin(const std::string& deviceName = "Cardputer Keyboard");
    void end();
    bool isEnabled() const;
    void setEnabled(bool enabled);
    bool isConnected() const;
    bool checkServerConnection() const;
    bool isReady() const;
    void sendString(const std::string& text);
    void sendChunkedString(const std::string& data, size_t chunkSize=128, unsigned long delayBetweenChunks=50);
    const std::string& getDeviceName() const;
    
    // BLE bonding and pairing management
    void clearBondingData();
    bool hasBondedDevices() const;
    std::vector<std::string> getBondedDeviceNames() const;
    
    // Connection activation
    void triggerConnection();
    
    // Security
    static uint32_t generateRandomPasskey();
    uint32_t getCurrentPasskey() const;
    void setDisplayPairingCodeCallback(std::function<void(uint32_t)> callback);

    // Keyboard layout management
    void setLayout(const uint8_t* newLayout);
    const uint8_t* getCurrentLayout() const;

private:
    void initBLE(const std::string& deviceName);
    void setupSecurity();
    void sendKey(uint8_t keyCode);
    void sendKeyDown(uint8_t keyCode);
    void sendKeyUp(uint8_t keyCode);
    void sendHIDReport(uint8_t* report, size_t reportSize);
    
    // HID report descriptor for a keyboard
    static const uint8_t reportDescriptor[];
    
    // HID reports
    uint8_t keyboardReport[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    
    std::string deviceName;
    BleKeyboardCallbacks* callbacks;
    NimBLEHIDDevice* hid;
    NimBLECharacteristic* inputKeyboard;
    NimBLEServer* server;
    
    bool enabled;
    bool initialized;
    uint32_t initTime;
    uint32_t currentPasskey;
    std::function<void(uint32_t)> displayPairingCodeCallback;
    Preferences preferences;
    const uint8_t* currentLayout;  // Current keyboard layout
};

#endif // BLESERVICE_H
