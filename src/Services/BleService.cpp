#include "BleService.h"
#include "../lib/USBHIDKeyboard/KeyboardLayout.h"

// Declare the external keyboard layout
extern const uint8_t KeyboardLayout_en_US[128];

// HID Report Descriptor for keyboard
const uint8_t BleService::reportDescriptor[] = {
    0x05, 0x01,  // Usage Page (Generic Desktop)
    0x09, 0x06,  // Usage (Keyboard)
    0xA1, 0x01,  // Collection (Application)
    0x85, 0x01,  // Report ID (1)
    0x05, 0x07,  // Usage Page (Key Codes)
    0x19, 0xE0,  // Usage Minimum (224)
    0x29, 0xE7,  // Usage Maximum (231)
    0x15, 0x00,  // Logical Minimum (0)
    0x25, 0x01,  // Logical Maximum (1)
    0x75, 0x01,  // Report Size (1)
    0x95, 0x08,  // Report Count (8)
    0x81, 0x02,  // Input (Data, Variable, Absolute)
    0x95, 0x01,  // Report Count (1)
    0x75, 0x08,  // Report Size (8)
    0x81, 0x01,  // Input (Constant) reserved byte(1)
    0x95, 0x06,  // Report Count (6)
    0x75, 0x08,  // Report Size (8)
    0x15, 0x00,  // Logical Minimum (0)
    0x25, 0x65,  // Logical Maximum (101)
    0x05, 0x07,  // Usage Page (Key codes)
    0x19, 0x00,  // Usage Minimum (0)
    0x29, 0x65,  // Usage Maximum (101)
    0x81, 0x00,  // Input (Data, Array) Key array(6 bytes)
    0xC0        // End Collection
};

// Callbacks implementation
BleKeyboardCallbacks::BleKeyboardCallbacks() : connected(false) {}

void BleKeyboardCallbacks::onConnect(NimBLEServer* server) {
    connected = true;
    Serial.println("BLE Client connected");
}

void BleKeyboardCallbacks::onDisconnect(NimBLEServer* server) {
    connected = false;
    Serial.println("BLE Client disconnected");
    
    // Restart advertising to allow reconnections
    server->startAdvertising();
}

void BleKeyboardCallbacks::onAuthenticationComplete(NimBLEConnInfo& connInfo) {
    if (connInfo.isBonded()) {
        Serial.println("BLE Client bonded successfully");
    } else {
        Serial.println("BLE Client bonding failed");
    }
}

bool BleKeyboardCallbacks::isConnected() const {
    return connected;
}

bool BleKeyboardCallbacks::checkConnection(NimBLEServer* server) const {
    // Check our internal connection flag first
    if (!connected) {
        return false;
    }
    
    // Also verify with the server if possible
    if (server) {
        return server->getConnectedCount() > 0;
    }
    
    return connected;
}

// BleService implementation
BleService::BleService() 
    : deviceName("Cardputer Keyboard"), 
      callbacks(nullptr), 
      hid(nullptr), 
      inputKeyboard(nullptr), 
      server(nullptr),
      enabled(false),
      initialized(false), 
      initTime(0),
      currentPasskey(0),
      displayPairingCodeCallback(nullptr),
      currentLayout(KeyboardLayout_en_US) {}

void BleService::begin(const std::string& name) {
    if (!enabled || initialized) {
        return;
    }
    
    deviceName = name;
    
    // Open preferences for reading/writing BLE bonding data
    preferences.begin("ble_keyboard", false);
    
    // Initialize BLE with security
    initBLE(deviceName);
    setupSecurity();
    
    initialized = true;
    initTime = millis();
    
    // Log initialization
    Serial.println("BLE service initialized and ready");
}

void BleService::initBLE(const std::string& deviceName) {
    // Initialize BLE
    NimBLEDevice::init(deviceName);
    
    // Set custom device information - use maximum power for better range
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
    
    // Create the BLE Server
    server = NimBLEDevice::createServer();
    callbacks = new BleKeyboardCallbacks();
    server->setCallbacks(callbacks);
    
    // Create the BLE HID Device
    hid = new NimBLEHIDDevice(server);
    hid->reportMap((uint8_t*)reportDescriptor, sizeof(reportDescriptor));
    
    // Create the Input, Output, and Feature characteristics for the HID keyboard
    inputKeyboard = hid->inputReport(1);
    
    // Set up the HID device appearance, manufacturer, etc.
    hid->manufacturer()->setValue("M5Stack");
    hid->pnp(0x02, 0x05ac, 0x820a, 0x0210);
    hid->hidInfo(0x00, 0x01);
    
    // Start the HID service - this was missing!
    hid->startServices();
    
    // Security must be set before starting advertising
    setupSecurity();
    
    // Start the service advertising with parameters optimized for Windows compatibility
    NimBLEAdvertising* advertising = server->getAdvertising();
    advertising->setAppearance(HID_KEYBOARD);
    advertising->addServiceUUID(hid->hidService()->getUUID());
    
    // These parameters improve Windows compatibility
    advertising->setScanResponse(true);
    advertising->setMinPreferred(0x06);  // functions that help with iPhone/Windows connections
    advertising->setMaxPreferred(0x12);
    
    // Start advertising - this makes the device visible for connections
    advertising->start();
    
    Serial.println("BLE HID Keyboard device advertising started with enhanced parameters");
}

void BleService::setupSecurity() {
    // Generate a random passkey
    currentPasskey = generateRandomPasskey();
    
    // Enable security features including bonding
    // Force KEYBOARD_DISPLAY mode to require PIN entry
    NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_BOND | BLE_SM_PAIR_AUTHREQ_MITM | BLE_SM_PAIR_AUTHREQ_SC);
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_KEYBOARD_DISPLAY);
    NimBLEDevice::setSecurityPasskey(currentPasskey);
    
    // Additional security settings that help with Windows connections
    NimBLEDevice::setSecurityInitKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);
    NimBLEDevice::setSecurityRespKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);
    
    Serial.printf("BLE security configured with passkey: %d (Enhanced security enabled)\n", currentPasskey);
    
    // If a callback is registered, call it with the generated passkey
    if (displayPairingCodeCallback) {
        displayPairingCodeCallback(currentPasskey);
    }
}

void BleService::end() {
    if (initialized) {
        NimBLEDevice::deinit(true);
        initialized = false;
        
        // Close preferences
        preferences.end();
        
        // Clean up allocated resources
        if (callbacks) {
            delete callbacks;
            callbacks = nullptr;
        }
        
        // Note: NimBLE library handles the deletion of server and characteristics
        server = nullptr;
        inputKeyboard = nullptr;
        hid = nullptr;
    }
}

void BleService::clearBondingData() {
    if (initialized) {
        // Use NimBLE's own API for clearing bonds
        NimBLEDevice::deleteAllBonds();
        Serial.println("All BLE bonds cleared");
    } else if (enabled) {
        // Initialize temporarily to clear bonds
        NimBLEDevice::init(deviceName);
        NimBLEDevice::deleteAllBonds();
        NimBLEDevice::deinit(true);
        Serial.println("All BLE bonds cleared");
    }
}

bool BleService::hasBondedDevices() const {
    // For NimBLE, we can simply check if we're currently connected
    return isConnected();
}

std::vector<std::string> BleService::getBondedDeviceNames() const {
    std::vector<std::string> bondedDevices;
    
    // If we're connected, add the current device
    if (isConnected() && server) {
        bondedDevices.push_back("Connected BLE Device");
    }
    
    return bondedDevices;
}

bool BleService::isEnabled() const {
    return enabled;
}

void BleService::setEnabled(bool en) {
    enabled = en;
    
    // If disabled and was initialized, shut down BLE
    if (!enabled && initialized) {
        end();
    }
    // If enabled and not initialized, start BLE
    else if (enabled && !initialized) {
        begin(deviceName);
    }
}

bool BleService::checkServerConnection() const {
    if (!server) {
        return false;
    }
    
    // Direct check from the server
    return server->getConnectedCount() > 0;
}

bool BleService::isConnected() const {
    // Simple check based on the callback state
    if (!initialized || !callbacks) {
        return false;
    }
    
    return callbacks->isConnected();
}

bool BleService::isReady() const {
    // Device is ready if it's initialized
    return initialized;
}

void BleService::sendString(const std::string& text) {
    if (!isReady()) {
        return;
    }
    
    // Wait for BLE to initialize properly
    while (millis() - initTime < 1000) {
        delay(10);
    }
    
    // Reset all keys
    memset(keyboardReport, 0, sizeof(keyboardReport));
    sendHIDReport(keyboardReport, sizeof(keyboardReport));
    
    // Send each character
    for (const char& c : text) {
        sendKey(c);
    }
}

void BleService::sendKey(uint8_t keyCode) {
    // Press and release the key
    sendKeyDown(keyCode);
    delay(10);
    sendKeyUp(keyCode);
    delay(10);
}

void BleService::sendKeyDown(uint8_t keyCode) {
    // Use the current keyboard layout
    const uint8_t* layout = currentLayout;
    
    // Lookup the HID code and modifier
    uint8_t hidCode = layout[keyCode];
    
    // If the character is not supported, return
    if (hidCode == 0x00) {
        return;
    }
    
    uint8_t key = hidCode & 0x7F;  // Remove modifier bit
    uint8_t modifier = (hidCode & 0x80) ? 0x02 : 0x00;  // Left Shift
    
    // Prepare the HID report
    memset(keyboardReport, 0, sizeof(keyboardReport));
    keyboardReport[0] = modifier;  // Modifier keys
    keyboardReport[2] = key;       // Key code
    
    // Send the HID report
    sendHIDReport(keyboardReport, sizeof(keyboardReport));
}

void BleService::sendKeyUp(uint8_t keyCode) {
    // Release all keys
    memset(keyboardReport, 0, sizeof(keyboardReport));
    sendHIDReport(keyboardReport, sizeof(keyboardReport));
}

void BleService::sendHIDReport(uint8_t* report, size_t reportSize) {
    if (inputKeyboard) {
        inputKeyboard->notify(report, reportSize);
        // Allow some time for the notification to be sent
        delay(5);
    }
}

void BleService::sendChunkedString(const std::string& data, size_t chunkSize, unsigned long delayBetweenChunks) {
    if (!isReady()) {
        return;
    }
    
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

const std::string& BleService::getDeviceName() const {
    return deviceName;
}

void BleService::triggerConnection() {
    if (!isConnected()) {
        return;
    }
    
    // Send a dummy report to activate the connection
    uint8_t emptyReport[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    inputKeyboard->setValue(emptyReport, sizeof(emptyReport));
    inputKeyboard->notify();
    
    // Small delay
    delay(10);
    
    Serial.println("Sent dummy keystroke to activate BLE connection");
}

uint32_t BleService::generateRandomPasskey() {
    // Generate a random 6-digit passkey (100000-999999)
    return 100000 + random(900000);
}

uint32_t BleService::getCurrentPasskey() const {
    return currentPasskey;
}

void BleService::setDisplayPairingCodeCallback(std::function<void(uint32_t)> callback) {
    displayPairingCodeCallback = callback;
}

void BleService::setLayout(const uint8_t* newLayout) {
    // Ensure layout is not null, default to US layout if it is
    currentLayout = newLayout ? newLayout : KeyboardLayout_en_US;
}

const uint8_t* BleService::getCurrentLayout() const {
    return currentLayout;
}
