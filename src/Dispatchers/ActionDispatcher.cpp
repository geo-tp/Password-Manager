#include "ActionDispatcher.h"

ActionDispatcher::ActionDispatcher(DependencyProvider& provider)
    : provider(provider), 
      context(ContextEnum::NoVault),
      selectedEntry(),
      selectedField() {}

void ActionDispatcher::setup() {
    provider.getUtilityController().handleLoadNvs();
    provider.getUtilityController().handleWelcome();
}

void ActionDispatcher::run() {
    while (true) {
        auto action = determineActionByContext();
        executeAction(action);
    }
}

ActionEnum ActionDispatcher::determineActionByContext() {
    auto action = ActionEnum::None;

    // Vault lock state check
    if(globalState.getVaultIsLocked()) { 
        context = ContextEnum::NoVault;
        provider.getUtilityController().handleInactivity();
    }

    switch (context) {
        case ContextEnum::NoVault:
            action = provider.getVaultController().actionNoVault();
            break;

        case ContextEnum::VaultSelected:
            action = provider.getVaultController().actionVaultSelected();
            if (action == ActionEnum::None) { context = ContextEnum::NoVault;}
            break;

        case ContextEnum::VaultLoaded:
            action = ActionEnum::SelectEntry;
            break;

        case ContextEnum::EntrySelected:
            action = ActionEnum::SelectField;
            break;

        case ContextEnum::FieldSelected:
            action = provider.getEntryController().actionFieldSelected(selectedField);
            if (action == ActionEnum::None) {context = ContextEnum::EntrySelected;}
            break;
    }

    return action;
}

void ActionDispatcher::executeAction(ActionEnum action) {
    bool confirmation;
    switch (action) {
        case ActionEnum::CreateVault:
            confirmation = provider.getVaultController().handleVaultCreation();
            if (confirmation) {
                context = ContextEnum::VaultSelected;
            }
            break;

        case ActionEnum::OpenVault:
            confirmation = provider.getVaultController().handleVaultLoading();
            if (confirmation) {
                context = ContextEnum::VaultSelected;
            }
            break;

        case ActionEnum::SelectEntry: {
            bool deleteRequested = false;
            selectedEntry = provider.getEntryController().handleEntrySelection(true, &deleteRequested);

            if (selectedEntry.empty()) {
                context = ContextEnum::VaultSelected;
            } else if (deleteRequested) {
                confirmation = provider.getEntryController().handleEntryDeletion(selectedEntry);
                if (confirmation) {
                    provider.getVaultController().handleVaultSave();
                }
                context = ContextEnum::VaultSelected;
            } else {
                context = ContextEnum::EntrySelected;
            }
            break;
        }

        case ActionEnum::CreateEntry:
            confirmation = provider.getEntryController().handleEntryCreation();
            if (confirmation) {
                provider.getVaultController().handleVaultSave();
            }
            break;

        case ActionEnum::DeleteEntry:
            confirmation = provider.getEntryController().handleEntryDeletion();
            if (confirmation) {
                provider.getVaultController().handleVaultSave();
            }
            break;

        case ActionEnum::SelectField:
            provider.getUtilityController().handleKeyboardInitialization();
            selectedField = provider.getEntryController().handleFieldSelection(selectedEntry);

            if (selectedField.empty()) {
                context = ContextEnum::VaultLoaded;
            } else {
                context = ContextEnum::FieldSelected;    
            }
            break;

        case ActionEnum::UpdateField:
            confirmation = provider.getEntryController().handleEntryUpdate(selectedEntry, selectedField);
            if (confirmation) {
                provider.getVaultController().handleVaultSave();
            }
            break;
        
        case ActionEnum::SendToUsb:
            provider.getUtilityController().handleUsbTyping(selectedField.getValue());
            break;

        case ActionEnum::SendToUsbAndEnter:
            provider.getUtilityController().handleUsbTyping(selectedField.getValue(), true);
            break;

        case ActionEnum::UpdateSettings:
            provider.getUtilityController().handleGeneralSettings();
            break;
    }
}