#ifndef DEPENDENCY_PROVIDER_H
#define DEPENDENCY_PROVIDER_H

#include "Views/IView.h"
#include "Inputs/IInput.h"
#include "Services/CryptoService.h"
#include "Services/EntryService.h"
#include "Services/CategoryService.h"
#include "Services/SdService.h"
#include "Services/NvsService.h"
#include "Services/KeyboardService.h"
#include "Services/LedService.h"
#include "Transformers/JsonTransformer.h"
#include "Transformers/ModelTransformer.h"
#include "Transformers/TimeTransformer.h"
#include "Selectors/VerticalSelector.h"
#include "Selectors/HorizontalSelector.h"
#include "Selectors/FieldEditorSelector.h"
#include "Selectors/StringPromptSelector.h"
#include "Selectors/ConfirmationSelector.h"
#include "Selectors/FieldActionSelector.h"
#include "Repositories/EntryRepository.h"
#include "Repositories/CategoryRepository.h"
#include "Controllers/VaultController.h"
#include "Controllers/EntryController.h"
#include "Controllers/UtilityController.h"
#include "Managers/InactivityManager.h"

class DependencyProvider {
public:
    DependencyProvider(IView& view, IInput& input);
    void setup();

    // Accessors for core components
    IView& getView();
    IInput& getInput();

    // Repositories
    EntryRepository& getEntryRepository();
    CategoryRepository& getCategoryRepository();

    // Services
    CryptoService& getCryptoService();
    EntryService& getEntryService();
    CategoryService& getCategoryService();
    SdService& getSdService();
    NvsService& getNvsService();
    KeyboardService& getKeyboardService();
    LedService& getLedService();

    // Transformers
    JsonTransformer& getJsonTransformer();
    ModelTransformer& getModelTransformer();
    TimeTransformer& getTimeTransformer();

    // Selectors
    VerticalSelector& getVerticalSelector();
    HorizontalSelector& getHorizontalSelector();
    FieldEditorSelector& getFieldEditorSelector();
    FieldActionSelector& getFieldActionSelector();
    StringPromptSelector& getStringPromptSelector();
    ConfirmationSelector& getConfirmationSelector();

    // Controllers
    VaultController& getVaultController();
    EntryController& getEntryController();
    UtilityController& getUtilityController();

    // Managers
    InactivityManager& getInactivityManager();

private:
    IView& view;
    IInput& input;

    // Repositories
    EntryRepository entryRepository;
    CategoryRepository categoryRepository;

    // Services
    CryptoService cryptoService;
    EntryService entryService;
    CategoryService categoryService;
    SdService sdService;
    NvsService nvsService;
    KeyboardService keyboardService;
    LedService ledService;

    // Transformers
    JsonTransformer jsonTransformer;
    ModelTransformer modelTransformer;
    TimeTransformer timeTransformer;

    // Selectors
    VerticalSelector verticalSelector;
    HorizontalSelector horizontalSelector;
    FieldEditorSelector fieldEditorSelector;
    FieldActionSelector fieldActionSelector;
    StringPromptSelector stringPromptSelector;
    ConfirmationSelector confirmationSelector;

    // Controllers
    VaultController vaultController;
    EntryController entryController;
    UtilityController utilityController;

    // Managers
    InactivityManager inactivityManager;

};

#endif // DEPENDENCY_PROVIDER_H
