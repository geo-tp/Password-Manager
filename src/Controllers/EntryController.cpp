#include "EntryController.h"

EntryController::EntryController(IView& display,
                                 IInput& input,
                                 HorizontalSelector& horizontalSelector,
                                 VerticalSelector& verticalSelector,
                                 FieldActionSelector& fieldActionSelector,
                                 ConfirmationSelector& confirmationSelector,
                                 StringPromptSelector& stringPromptSelector,
                                 EntryService& entryService,
                                 CryptoService& cryptoService,
                                 KeyboardService& keyboardService,
                                 LedService& ledService,
                                 NvsService& nvsService,
                                 ModelTransformer& modelTransformer)
    : display(display),
      input(input),
      horizontalSelector(horizontalSelector),
      verticalSelector(verticalSelector),
      fieldActionSelector(fieldActionSelector),
      confirmationSelector(confirmationSelector),
      stringPromptSelector(stringPromptSelector),
      entryService(entryService),
      cryptoService(cryptoService),
      keyboardService(keyboardService),
      ledService(ledService),
      nvsService(nvsService),
      modelTransformer(modelTransformer) {}

ActionEnum EntryController::actionFieldSelected(Field field) {
    auto action = fieldActionSelector.select(field.getLabel(), field.getValue());

    return action;
}

Entry EntryController::handleEntrySelection() {
    auto entries = entryService.getAllEntries();
    std::reverse(entries.begin(), entries.end()); // last created in first
    auto labels = modelTransformer.toStrings(entries);
    auto selectedIndex = verticalSelector.select(entryService.getVaultName(), labels, true, true);

    if (selectedIndex == -1) {
        return entryService.getEmptyEntry();
    }

    return entries[selectedIndex];
}

Field EntryController::handleFieldSelection(Entry& selectedEntry) {
    auto fieldValues = modelTransformer.toStrings(selectedEntry);
    std::vector<std::string> fieldLabels = {"User", "Pass", "Note"};
    std::vector<std::string> shortcuts = {"U", "P", "N"};
    auto selectedIndex = verticalSelector.select(selectedEntry.getServiceName(), fieldValues, true, false, fieldLabels, shortcuts, true);

    if (selectedIndex == -1) {
        return Field();
    }

    return Field(fieldLabels[selectedIndex], fieldValues[selectedIndex], shortcuts[selectedIndex]);
}

bool EntryController::handleEntryCreation() {
    // Check max saved password
    auto entryCount = entryService.getAllEntries().size();
    auto entryLimit = globalState.getMaxSavedPasswordCount();
    if (entryCount >= entryLimit) {
        display.topBar("Passwords Limit", false, false);
        display.subMessage("Can't save more passwords", 2000);
        return false;
    }

    auto servicerName = stringPromptSelector.select("Create a new Entry", "Enter service name", "", true, false, true);
    if (servicerName.empty()) {
        return false;
    }

    // Get the last used username
    auto lastUsername = globalState.getLastUsedUsername();
    if (lastUsername.empty()) {
        auto entries = entryService.getAllEntries();

        if (!entries.empty()) {
            std::reverse(entries.begin(), entries.end()); // last created in first
            lastUsername = entries[0].getUsername();
        }
    }

    auto randomPassword = cryptoService.generateRandomString(20);
    auto username = stringPromptSelector.select("Username or Email", "Enter username", lastUsername, false, true, false, 3, true);
    display.subMessage("Loading...", 300); // avoid rendering too fast after OK input
    auto password = stringPromptSelector.select("Account Password", "Enter password", randomPassword, false, true, false, 3, true);
    auto notes = stringPromptSelector.select("Notes (Optionnal)", "Enter notes (OK to pass)", "", false, true, false, 0);
    notes = notes.empty() ? "No notes" : notes;

    auto entry = Entry(servicerName, username, password, notes);
    entryService.addEntry(entry);
    globalState.setLastUsedUsername(username);
    display.topBar("Password added", false, false);
    display.subMessage("Successfully created", 1000);
    return true;
}

bool EntryController::handleEntryUpdate(Entry& entry, Field& field) {
    auto value = stringPromptSelector.select(field.getLabel(), "Modify the value", field.getValue(), false, true);
    if (value == field.getValue()) {
        return false;
    }

    field.setValue(value);
    entryService.updateField(entry, field);
    display.subMessage("Field updated", 500);
    if (field.getLabel() == "User") {
        globalState.setLastUsedUsername(field.getValue());
    }

    return true;
}

bool EntryController::handleEntryDeletion() {
    size_t selectedIndex;
    bool entryUpdated = false;

    while (selectedIndex != 1) {
        auto entries = entryService.getAllEntries();
        std::reverse(entries.begin(), entries.end()); // last created in first
        auto labels = modelTransformer.toStrings(entries);
        std::vector<std::string> delLabels(labels.size(), "Del");
        auto selectedIndex = verticalSelector.select("Select to remove", labels, true, true, delLabels);
        if (selectedIndex == -1) {break;}
        delay(200); // debounce in case of double input
        auto confirmation = confirmationSelector.select("Erase Password", "Delete " + labels[selectedIndex] + " ?");
        if (confirmation) {
            entryService.deleteEntry(entries[selectedIndex]);
            entryUpdated = true;
        }
    }

    if (entryUpdated) {
        display.subMessage("Saving...", 0);
    }
    
    return entryUpdated;
}
