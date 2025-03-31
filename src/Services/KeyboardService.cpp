#include "KeyboardService.h"

KeyboardService::KeyboardService() : bleEnabled(false) {}

void KeyboardService::begin() {
    // Always initialize USB service
    usbService.begin();
    
    // Initialize BLE only if enabled
    if (bleEnabled) {
        bleService.begin();
    }
}

void KeyboardService::end() {
    usbService.end();
    
    if (bleEnabled) {
        bleService.end();
    }
}

void KeyboardService::setBleEnabled(bool enabled) {
    bleEnabled = enabled;
    bleService.setEnabled(enabled);
    
    // If BLE was enabled and wasn't initialized, initialize it
    if (bleEnabled && !bleService.isReady()) {
        bleService.begin();
    }
}

bool KeyboardService::isBleEnabled() const {
    return bleEnabled;
}

bool KeyboardService::isBleConnected() const {
    return bleEnabled && bleService.isConnected();
}

void KeyboardService::setLayout(const uint8_t* newLayout) {
    // Set layout for USB keyboard
    usbService.setLayout(newLayout);
    
    // Set layout for BLE keyboard
    bleService.setLayout(newLayout);
}

void KeyboardService::sendString(const std::string& text) {
    // Try to trigger a BLE connection first if BLE is enabled
    if (bleEnabled && bleService.isConnected()) {
        Serial.println("KeyboardService: Triggering BLE connection before sending");
        bleService.triggerConnection();
        delay(100); // Give a moment for the connection to become active
    }

    // Get the current connection status
    auto connectionType = getActiveConnectionType();
    
    // Send via the appropriate channel with clear logging
    if (connectionType == KeyboardConnectionType::BLE) {
        Serial.println("KeyboardService: Sending via BLE");
        bleService.sendString(text);
    } else {
        Serial.println("KeyboardService: Sending via USB");
        usbService.sendString(text);
    }
}

void KeyboardService::sendChunkedString(const std::string& data, size_t chunkSize, unsigned long delayBetweenChunks) {
    // If BLE is enabled and connected, prefer it
    if (bleEnabled && bleService.isConnected()) {
        bleService.sendChunkedString(data, chunkSize, delayBetweenChunks);
    } 
    // Fall back to USB if BLE is not available or not connected
    else {
        usbService.sendChunkedString(data, chunkSize, delayBetweenChunks);
    }
}

KeyboardConnectionType KeyboardService::getActiveConnectionType() const {
    // Check if BLE is enabled AND connected
    if (bleEnabled) {
        Serial.println("KeyboardService: BLE is enabled, checking connection status...");
        
        // First, try to check server connection directly
        bool directServerCheck = false;
        try {
            directServerCheck = bleService.checkServerConnection();
            if (directServerCheck) {
                Serial.println("KeyboardService: Direct server check shows BLE is connected!");
                return KeyboardConnectionType::BLE;
            }
        } catch (...) {
            Serial.println("KeyboardService: Error during direct server check");
        }
        
        // If direct check failed, try simple isConnected method
        bool isConnected = false;
        try {
            isConnected = bleService.isConnected();
            if (isConnected) {
                Serial.println("KeyboardService: BLE is connected!");
                return KeyboardConnectionType::BLE;
            }
        } catch (...) {
            Serial.println("KeyboardService: Error checking BLE connection status");
        }
        
        Serial.println("KeyboardService: BLE is NOT connected");
    } else {
        Serial.println("KeyboardService: BLE is disabled");
    }
    
    // Check if USB is ready
    if (usbService.isReady()) {
        Serial.println("KeyboardService: USB is ready and will be used");
        return KeyboardConnectionType::USB;
    }
    
    // Neither is available
    Serial.println("KeyboardService: No keyboard connection available");
    return KeyboardConnectionType::NONE;
}

bool KeyboardService::isReady() const {
    // Ready if USB is ready or BLE is enabled, connected, and ready
    return usbService.isReady() || (bleEnabled && bleService.isReady());
}

void KeyboardService::clearBleBondingData() {
    // Forward the call to BLE service to clear bonding data
    bleService.clearBondingData();
}

bool KeyboardService::hasBondedBleDevices() const {
    return bleService.hasBondedDevices();
}
