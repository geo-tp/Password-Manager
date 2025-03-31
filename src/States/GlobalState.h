#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H

#include <cstdint>
#include <string>

class GlobalState {
private:
    // Device Pinout
    uint8_t ledPin = 21;
    uint8_t sdCardCSPin = 12;
    uint8_t sdCardCLKPin = 40;
    uint8_t sdCardMISOPin = 39;
    uint8_t sdCardMOSIPin = 14;
    size_t fileCountLimit = 512;
    size_t fileCacheLimit = 64;

    // Char and entry limit
    size_t maxInputCharCount = 24;
    size_t maxInputCharPasswordCount = 128;
    size_t maxSavedPasswords = 100;

    // Encryption size
    size_t saltSize = 16;
    size_t checksumSize = 32;

    // Configuration NVS key names
    std::string nvsNamespace = "vault_manager";
    std::string nvsLastUsedVaultPath = "lastUsedVault";
    std::string nvsKeyboardLayout = "keyboardLayout";
    std::string nvsScreenBrightness = "screenBright";
    std::string nvsInactivityScreenTimeout = "screenOffTime";
    std::string nvsInactivityLockTimeout = "vaultLockTime";
    std::string nvsBleEnabled = "bleEnabled";
    std::string nvsBleDeviceName = "bleDeviceName";

    // User config
    std::string selectedKeyboardLayout = "";
    uint8_t selectedScreenBrightness = 140;
    std::string defaultVaultPath = "/vaults";
    bool bleEnabled = false;
    std::string bleDeviceName = "Cardputer Keyboard";

    // Last Vault
    std::string loadedVaultPath = "";
    std::string loadedVaultPassword = "";
    bool vaultIsLocked = false;

    // Last Entry username
    std::string lastUsedUsername = "";

    // Inactivity Timing
    uint32_t inactivityBrightnessTimeout = 12000; // 12 sec
    uint32_t inactivityScreenTimeout = 1 * 60 * 1000; // 1 min
    uint32_t inactivityLockTimeout = 10 * 60 * 1000; // 10 mins

    // Private constructor
    GlobalState() = default;

public:
    // Erase Public constructor
    GlobalState(const GlobalState&) = delete;
    GlobalState& operator=(const GlobalState&) = delete;

    // Get the unique instance
    static GlobalState& getInstance() {
        static GlobalState instance;
        return instance;
    }

    // Accesseurs pour les configurations de périphériques
    uint8_t getLedPin() const { return ledPin; }
    uint8_t getSdCardCSPin() const { return sdCardCSPin; }
    uint8_t getSdCardCLKPin() const { return sdCardCLKPin; }
    uint8_t getSdCardMISOPin() const { return sdCardMISOPin; }
    uint8_t getSdCardMOSIPin() const { return sdCardMOSIPin; }
    size_t getFileCountLimit() const { return fileCountLimit; }
    size_t getFileCacheLimit() const { return fileCountLimit; }

    // Mutateurs pour les configurations de périphériques
    void setSdCardCSPin(uint8_t pin) { sdCardCSPin = pin; }
    void setSdCardCLKPin(uint8_t pin) { sdCardCLKPin = pin; }
    void setSdCardMISOPin(uint8_t pin) { sdCardMISOPin = pin; }
    void setSdCardMOSIPin(uint8_t pin) { sdCardMOSIPin = pin; }
    void setFileCountLimit(size_t limit) { fileCountLimit = limit; }
    void setFileCacheLimit(size_t limit) { fileCountLimit = limit; }

    // Accesseurs pour les limites de saisies utilisateur
    size_t getMaxInputCharCount() const { return maxInputCharCount; }
    size_t getMaxInputCharPasswordCount() const { return maxInputCharPasswordCount; }
    size_t getMaxSavedPasswordCount() const { return maxSavedPasswords; }

    // Mutateurs pour les limites de saisies utilisateur
    void setMaxInputCharCount(size_t limit) { maxInputCharCount = limit; }
    void setMaxInputCharPasswordCount(size_t limit) { maxInputCharPasswordCount = limit; }
    void setMaxSavedPasswordCount(size_t limit) { maxSavedPasswords = limit; }

    // Accesseurs pour les tailles de sel et de checksum
    size_t getSaltSize() const { return saltSize; }
    size_t getChecksumSize() const { return checksumSize; }

    // Mutateurs pour les tailles de sel et de checksum
    void setSaltSize(size_t size) { saltSize = size; }
    void setChecksumSize(size_t size) { checksumSize = size; }

    // Accesseurs pour la configuration NVS
    const std::string& getNvsNamespace() const { return nvsNamespace; }
    const std::string& getNvsLastUsedVaultPath() const { return nvsLastUsedVaultPath; }
    const std::string& getNvsKeyboardLayout() const { return nvsKeyboardLayout; }
    const std::string& getNvsScreenBrightness() const { return nvsScreenBrightness; }
    const std::string& getNvsInactivityScreenTimeout() const { return nvsInactivityScreenTimeout; }
    const std::string& getNvsInactivityLockTimeout() const { return nvsInactivityLockTimeout; }
    const std::string& getNvsBleEnabled() const { return nvsBleEnabled; }
    const std::string& getNvsBleDeviceName() const { return nvsBleDeviceName; }

    // Accesseurs pour config
    const std::string& getSelectedKeyboardLayout() const { return selectedKeyboardLayout; }
    const uint8_t getSelectedScreenBrightness() const { return selectedScreenBrightness; }
    const std::string& getDefaultVaultPath() const { return defaultVaultPath; }
    bool getBleEnabled() const { return bleEnabled; }
    const std::string& getBleDeviceName() const { return bleDeviceName; }

    // Mutateurs pour la configuration NVS
    void setNvsNamespace(const std::string& ns) { nvsNamespace = ns; }
    void setNvsLastUsedVaultPath(const std::string& key) { nvsLastUsedVaultPath = key; }
    void setNvsKeyboardLayout(const std::string& key) { nvsKeyboardLayout = key; }
    void setNvsScreenBrightness(const std::string& key) { nvsScreenBrightness = key; }
    void setNvsInactivityScreenTimeout(const std::string& key) { nvsInactivityScreenTimeout = key; }
    void setNvsInactivityLockTimeout(const std::string& key) { nvsInactivityLockTimeout = key; }
    void setNvsBleEnabled(const std::string& key) { nvsBleEnabled = key; }
    void setNvsBleDeviceName(const std::string& key) { nvsBleDeviceName = key; }

    // Mutateurs config
    void setSelectedKeyboardLayout(const std::string& key) { selectedKeyboardLayout = key; }
    void setSelectedScreenBrightness(uint8_t br) { selectedScreenBrightness = br; }
    void setDefaultVaultPath(const std::string& p) { defaultVaultPath = p; }
    void setBleEnabled(bool enabled) { bleEnabled = enabled; }
    void setBleDeviceName(const std::string& name) { bleDeviceName = name; }

    // Accesseurs pour les informations du dernier coffre chargé
    const std::string& getLoadedVaultPath() const { return loadedVaultPath; }
    const std::string& getLoadedVaultPassword() const { return loadedVaultPassword; }
    bool getVaultIsLocked() const { return vaultIsLocked; }

    // Mutateurs pour les informations du dernier coffre chargé
    void setLoadedVaultPath(const std::string& path) { loadedVaultPath = path; }
    void setLoadedVaultPassword(const std::string& password) { loadedVaultPassword = password; }
    void setVaultIsLocked(bool locked) { vaultIsLocked = locked; }

    // Accesseurs pour les temps d'inactivité
    uint32_t getInactivityBrightnessTimeout() const { return inactivityBrightnessTimeout; }
    uint32_t getInactivityScreenTimeout() const { return inactivityScreenTimeout; }
    uint32_t getInactivityLockTimeout() const { return inactivityLockTimeout; }

    // Mutateurs pour inactivity
    void setInactivityBrightnessTimeout(size_t timeout) { inactivityBrightnessTimeout = timeout; }
    void setInactivityScreenTimeout(size_t timeout) { inactivityScreenTimeout = timeout; }
    void setInactivityLockTimeout(size_t timeout) { inactivityLockTimeout = timeout; }

    // Accesseurs pour le dernier identifiant entry
    const std::string& getLastUsedUsername() const { return lastUsedUsername; }
    
    // Mutateurs pour le dernier identifiant entry
    void setLastUsedUsername(const std::string& username) { lastUsedUsername = username; }
};

#endif // GLOBAL_STATE_H
