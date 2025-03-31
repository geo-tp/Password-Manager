#include "DependencyProvider.h"

DependencyProvider::DependencyProvider(IView& view, IInput& input)
    : view(view),
      input(input),
      entryService(entryRepository),
      categoryService(categoryRepository),
      cryptoService(),
      sdService(),
      nvsService(),
      keyboardService(),
      ledService(),
      inactivityManager(view),
      verticalSelector(view, input, inactivityManager),
      horizontalSelector(view, input, inactivityManager),
      fieldEditorSelector(view, input),
      fieldActionSelector(view, input, inactivityManager),
      stringPromptSelector(view, input),
      confirmationSelector(view, input),
      jsonTransformer(),
      modelTransformer(),
      timeTransformer(),
      vaultController(view, input, horizontalSelector, verticalSelector, 
                      confirmationSelector, stringPromptSelector, sdService, 
                      nvsService, categoryService, entryService, cryptoService, 
                      jsonTransformer, modelTransformer),
      entryController(view, input, horizontalSelector, verticalSelector, fieldActionSelector,
                      confirmationSelector, stringPromptSelector, entryService, cryptoService, 
                      keyboardService, ledService, nvsService, modelTransformer),
      utilityController(view, input, horizontalSelector, verticalSelector, fieldEditorSelector, 
                        stringPromptSelector, confirmationSelector, keyboardService, ledService, nvsService,
                        sdService, timeTransformer)
      {}

void DependencyProvider::setup() {
    view.initialize();
}

// Accessors for core components
IView& DependencyProvider::getView() { return view; }
IInput& DependencyProvider::getInput() { return input; }

// Accessors for repositories
EntryRepository& DependencyProvider::getEntryRepository() { return entryRepository; }
CategoryRepository& DependencyProvider::getCategoryRepository() { return categoryRepository; }

// Accessors for services
CryptoService& DependencyProvider::getCryptoService() { return cryptoService; }
EntryService& DependencyProvider::getEntryService() { return entryService; }
CategoryService& DependencyProvider::getCategoryService() { return categoryService; }
SdService& DependencyProvider::getSdService() { return sdService; }
NvsService& DependencyProvider::getNvsService() { return nvsService; }
KeyboardService& DependencyProvider::getKeyboardService() { return keyboardService; }
LedService& DependencyProvider::getLedService() { return ledService; }

// Accessors for transformers
JsonTransformer& DependencyProvider::getJsonTransformer() { return jsonTransformer; }
ModelTransformer& DependencyProvider::getModelTransformer() { return modelTransformer; }
TimeTransformer& DependencyProvider::getTimeTransformer() { return timeTransformer; }


// Accessors for selectors
VerticalSelector& DependencyProvider::getVerticalSelector() { return verticalSelector; }
HorizontalSelector& DependencyProvider::getHorizontalSelector() { return horizontalSelector; }
FieldEditorSelector& DependencyProvider::getFieldEditorSelector() { return fieldEditorSelector; }
StringPromptSelector& DependencyProvider::getStringPromptSelector() { return stringPromptSelector; }
ConfirmationSelector& DependencyProvider::getConfirmationSelector() { return confirmationSelector; }
FieldActionSelector& DependencyProvider::getFieldActionSelector() { return fieldActionSelector; }

// Accessors for controllers
VaultController& DependencyProvider::getVaultController() { return vaultController; }
EntryController& DependencyProvider::getEntryController() { return entryController; }
UtilityController& DependencyProvider::getUtilityController() { return utilityController; }

// Accessors for managers
InactivityManager& DependencyProvider::getInactivityManager() { return inactivityManager; };