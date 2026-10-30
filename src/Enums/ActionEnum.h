#ifndef ACTION_ENUM_H
#define ACTION_ENUM_H
#include <unordered_map>
#include <vector>
#include <string>

enum class ActionEnum {
    None = -1,

    // File-related actions
    OpenVault,
    CreateVault,
    CloseVault,
    LoadSdVault,
    LoadNvsVault,

    // Entry-related actions
    SelectEntry,
    CreateEntry,
    DeleteEntry,

    // Field-related actions
    SelectField,
    UpdateField,

    // App-level actions
    SendToUsb,
    SendToUsbAndEnter,
    ShowHelp,
    UpdateSettings
};

class ActionEnumMapper {
public:
    static std::string toString(ActionEnum action) {
        static const std::unordered_map<ActionEnum, std::string> actionToStringMap = {
            {ActionEnum::None, "None"},
            {ActionEnum::OpenVault, "Open Vault"},
            {ActionEnum::CreateVault, "Create Vault"},
            {ActionEnum::CloseVault, "Close Vault"},
            {ActionEnum::LoadSdVault, "SD Vault"},
            {ActionEnum::LoadNvsVault, "NVS Vault"},
            {ActionEnum::SelectEntry, "My Passwords"},
            {ActionEnum::CreateEntry, "New Password"},
            {ActionEnum::DeleteEntry, "Del Password"},
            {ActionEnum::SelectField, "Select Field"},
            {ActionEnum::UpdateField, "Update Field"},
            {ActionEnum::SendToUsb, "Send to USB"},
            {ActionEnum::SendToUsbAndEnter, "Send to USB + Enter"},
            {ActionEnum::ShowHelp, "Show Help"},
            {ActionEnum::UpdateSettings, "Settings"}
        };

        auto it = actionToStringMap.find(action);
        return it != actionToStringMap.end() ? it->second : "Unknown Action";
    }

    static std::vector<std::string> getActionNames(const std::vector<ActionEnum>& actions) {
        std::vector<std::string> actionNames;
        for (const auto& action : actions) {
            actionNames.push_back(toString(action));
        }
        return actionNames;
    }
};

#endif // ACTION_ENUM_H
