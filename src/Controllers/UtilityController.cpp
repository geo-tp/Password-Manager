#include "UtilityController.h"

UtilityController::UtilityController(IView& display,
                                     IInput& input,
                                     HorizontalSelector& horizontalSelector,
                                     VerticalSelector& verticalSelector,
                                     FieldEditorSelector& fieldEditorSelector,
                                     StringPromptSelector& stringPromptSelector,
                                     ConfirmationSelector& confirmationSelector,
                                     KeyboardService& keyboardService,
                                     LedService& ledService,
                                     NvsService& nvsService,
                                     SdService& sdService,
                                     TimeTransformer& timeTransformer)
    : display(display),
      input(input),
      horizontalSelector(horizontalSelector),
      verticalSelector(verticalSelector),
      fieldEditorSelector(fieldEditorSelector),
      stringPromptSelector(stringPromptSelector),
      confirmationSelector(confirmationSelector),
      keyboardService(keyboardService),
      ledService(ledService),
      nvsService(nvsService),
      sdService(sdService),
      timeTransformer(timeTransformer) {}

void UtilityController::handleWelcome() {
    auto brightness = globalState.getSelectedScreenBrightness();
    display.welcome(brightness);
    input.waitPress();
    display.setBrightness(brightness);
}

bool UtilityController::handleUsbTyping(std::string sendString) {
    ledService.showLed();
    
    // Get detailed BLE status for debugging
    bool isEnabled = keyboardService.isBleEnabled();
    bool isConnected = keyboardService.isBleConnected();
    
    // Show detailed status message
    if (isEnabled && isConnected) {
        display.subMessage("Sending via BLE", 0);
    } else if (isEnabled && !isConnected) {
        display.subMessage("BLE enabled but not connected, using USB", 0);
    } else {
        display.subMessage("Sending via USB", 0);
    }
    
    // Add a small delay to make the message visible
    delay(1000);
    
    // Send the string
    keyboardService.sendString(sendString);
    ledService.clearLed();
    return true;
}

bool UtilityController::handleKeyboardInitialization() {
    auto selectedKeyboardLayout = globalState.getSelectedKeyboardLayout();
    const uint8_t* finalLayout = KeyboardLayoutMapper::toLayout(selectedKeyboardLayout);
    auto nvsKeyboardLayoutField = globalState.getNvsKeyboardLayout();
    int selectedIndex;

    if (selectedKeyboardLayout.empty()) {
        selectedKeyboardLayout = nvsService.getString(nvsKeyboardLayoutField);
    }

    if (selectedKeyboardLayout.empty()) {
        auto layouts = KeyboardLayoutMapper::getAllLayoutNames();
        selectedIndex = horizontalSelector.select("Choose Keyboard", layouts, "Region Layout", "Press OK to select");
        if (selectedIndex == -1) {
            return false;
        }
        selectedKeyboardLayout = layouts[selectedIndex];
        nvsService.saveString(nvsKeyboardLayoutField, selectedKeyboardLayout);
        globalState.setSelectedKeyboardLayout(selectedKeyboardLayout);
        finalLayout = KeyboardLayoutMapper::toLayout(selectedKeyboardLayout);
    }
    
    // Set the layout for the keyboard service
    keyboardService.setLayout(finalLayout);
    
    // Check if BLE is enabled from NVS settings
    std::string bleEnabled = nvsService.getString(globalState.getNvsBleEnabled());
    if (!bleEnabled.empty()) {
        bool isEnabled = (bleEnabled == "true" || bleEnabled == "1");
        globalState.setBleEnabled(isEnabled);
        keyboardService.setBleEnabled(isEnabled);
    }
    
    // Initialize the keyboard service (handles both USB and BLE if enabled)
    keyboardService.begin();
    return true;
}

void UtilityController::handleLoadNvs() {
    // Keyboard layout
    std::string savedLayout = nvsService.getString(globalState.getNvsKeyboardLayout());
    if (!savedLayout.empty()) {
        globalState.setSelectedKeyboardLayout(savedLayout);
    }

    // Brightness
    std::string savedBrightness = nvsService.getString(globalState.getNvsScreenBrightness());
    if (!savedBrightness.empty()) {
        uint8_t brightness = std::stoi(savedBrightness);
        globalState.setSelectedScreenBrightness(brightness);
        display.setBrightness(brightness);
    }

    // Screen off
    std::string savedScreenTimeout = nvsService.getString(globalState.getNvsInactivityScreenTimeout());
    if (!savedScreenTimeout.empty()) {
        uint32_t screenTimeout = timeTransformer.toMilliseconds(savedScreenTimeout);
        if (screenTimeout > 0) {
            globalState.setInactivityScreenTimeout(screenTimeout);
        }
    }

    // Vault lock
    std::string savedLockTimeout = nvsService.getString(globalState.getNvsInactivityLockTimeout());
    if (!savedLockTimeout.empty()) {
        uint32_t lockTimeout = timeTransformer.toMilliseconds(savedLockTimeout);
        if (lockTimeout > 0) {
            globalState.setInactivityLockTimeout(lockTimeout);
        }
    }
    
    // BLE enabled
    std::string bleEnabled = nvsService.getString(globalState.getNvsBleEnabled());
    if (!bleEnabled.empty()) {
        bool isEnabled = (bleEnabled == "true" || bleEnabled == "1");
        globalState.setBleEnabled(isEnabled);
    }
    
    // BLE device name
    std::string bleDeviceName = nvsService.getString(globalState.getNvsBleDeviceName());
    if (!bleDeviceName.empty()) {
        globalState.setBleDeviceName(bleDeviceName);
    }
}

bool UtilityController::handleGeneralSettings() {
    std::vector<std::string> timeLabels = timeTransformer.getAllTimeLabels();
    std::vector<uint32_t> timeValues = timeTransformer.getAllTimeValues();
    std::vector<std::string> brightnessValues = {"20", "60", "100", "140", "160", "200", "240"};
    std::vector<std::string> settingLabels = {" Keyboard ", "Brightness", "Screen off", "Vault lock", "BLE", "Reset BLE"};
    
    auto layouts = KeyboardLayoutMapper::getAllLayoutNames();
    auto selectedLayout = globalState.getSelectedKeyboardLayout().empty() ? layouts[2] : globalState.getSelectedKeyboardLayout();
    auto selectedScreenOffTime = timeTransformer.toLabel(globalState.getInactivityScreenTimeout());
    auto selectedLockCloseTime = timeTransformer.toLabel(globalState.getInactivityLockTimeout());
    std::vector<std::string> settings = {
        selectedLayout,
        std::to_string(globalState.getSelectedScreenBrightness()),
        selectedScreenOffTime, 
        selectedLockCloseTime + " ", 
        globalState.getBleEnabled() ? "Enabled" : "Disabled",
        "Clear Pairing"
    };

    while (true) {
        auto verticalIndex = verticalSelector.select("Settings", settings, true, false, settingLabels, {});
        size_t selectedIndex;
        if (verticalIndex == -1) {
            return false;
        }

        auto selectedSetting =  settingLabels[verticalIndex];

        if (selectedSetting == " Keyboard ") {
            selectedIndex = horizontalSelector.select("Choose Keyboard", layouts, "Region Layout", "Press OK to select", {}, false);
            globalState.setSelectedKeyboardLayout(layouts[selectedIndex]);
            nvsService.saveString(globalState.getNvsKeyboardLayout(), layouts[selectedIndex]);
            settings[verticalIndex] = layouts[selectedIndex];

        } else if (selectedSetting == "Brightness") {
            selectedIndex = horizontalSelector.select("Screen Brightness", brightnessValues, "Choose brightness", "Press OK to select", {}, false);
            uint8_t brightness = std::stoi(brightnessValues[selectedIndex]);
            globalState.setSelectedScreenBrightness(brightness);
            nvsService.saveString(globalState.getNvsScreenBrightness(), brightnessValues[selectedIndex]);
            display.setBrightness(brightness);
            settings[verticalIndex] = brightnessValues[selectedIndex];

        } else if (selectedSetting == "Screen off")  {
            selectedIndex = horizontalSelector.select("Screen Off", timeLabels, "Turn off inactivity", "Press OK to select", {}, false);
            globalState.setInactivityScreenTimeout(timeValues[selectedIndex]);
            nvsService.saveString(globalState.getNvsInactivityScreenTimeout(), timeLabels[selectedIndex]);
            settings[verticalIndex] = timeLabels[selectedIndex] + "  "; // hack to avoid same values in the vector

        } else if (selectedSetting == "Vault lock") {
            selectedIndex = horizontalSelector.select("Vault Lock", timeLabels, "Lock vault inactivity", "Press OK to select", {}, false);
            globalState.setInactivityLockTimeout(timeValues[selectedIndex]);
            nvsService.saveString(globalState.getNvsInactivityLockTimeout(), timeLabels[selectedIndex]);
            settings[verticalIndex] = timeLabels[selectedIndex] + " ";
        
        } else if (selectedSetting == "BLE") {
            selectedIndex = horizontalSelector.select("BLE", {"Enabled", "Disabled"}, "Enable/Disable BLE", "Press OK to select", {}, false);
            bool isEnabled = (selectedIndex == 0);
            globalState.setBleEnabled(isEnabled);
            nvsService.saveString(globalState.getNvsBleEnabled(), isEnabled ? "true" : "false");
            keyboardService.setBleEnabled(isEnabled);
            settings[verticalIndex] = isEnabled ? "Enabled" : "Disabled";
        } else if (selectedSetting == "Reset BLE") {
            bool confirmation = confirmationSelector.select("Reset BLE", "Clear all paired devices?");
            if (confirmation) {
                // Clear BLE bonding data
                keyboardService.clearBleBondingData();
                display.topBar("BLE Reset", false, false);
                display.subMessage("Pairing data cleared", 2000);
            }
        }
    }
}

void UtilityController::handleInactivity() {
    ledService.showLed();
    input.waitPress();
    ledService.clearLed();
    display.setBrightness(globalState.getSelectedScreenBrightness());
    if (!globalState.getLoadedVaultPath().empty()) {
        display.topBar("Inactivity", false, false);
        display.subMessage("Vault has been locked", 3000);
        globalState.setLoadedVaultPath("");
        globalState.setLoadedVaultPassword("");
        globalState.setVaultIsLocked(false);
    }
}
