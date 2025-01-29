#ifndef VAULT_CONTROLLER_H
#define VAULT_CONTROLLER_H

#include "Views/IView.h"
#include "Inputs/IInput.h"
#include "Selectors/HorizontalSelector.h"
#include "Selectors/VerticalSelector.h"
#include <Selectors/StringPromptSelector.h>
#include <Selectors/ConfirmationSelector.h>
#include "Services/SdService.h"
#include "Services/NvsService.h"
#include "Services/CategoryService.h"
#include "Services/EntryService.h"
#include "Services/CryptoService.h"
#include "Enums/ActionEnum.h"
#include "Transformers/JsonTransformer.h"
#include "Transformers/ModelTransformer.h"
#include "States/GlobalState.h"
#include "Models/VaultFile.h"

class VaultController {
public:
    VaultController(IView& display, 
                    IInput& input, 
                    HorizontalSelector& horizontalSelector, 
                    VerticalSelector& verticalSelector,
                    ConfirmationSelector& confirmationSelector,
                    StringPromptSelector& stringPromptSelector,
                    SdService& sdService, 
                    NvsService& nvsService, 
                    CategoryService& categoryService, 
                    EntryService& entryService,
                    CryptoService& cryptoService,
                    JsonTransformer& jsonTransformer,
                    ModelTransformer& modelTransformer);

    ActionEnum actionNoVault();
    ActionEnum actionVaultSelected();
    bool handleVaultCreation();
    bool handleVaultLoading();
    bool handleVaultSave();

private:
    bool loadDataFromEncryptedFile(std::string path);
    bool loadSdVault();

    IView& display;
    IInput& input;
    HorizontalSelector& horizontalSelector;
    VerticalSelector& verticalSelector;
    ConfirmationSelector& confirmationSelector;
    StringPromptSelector& stringPromptSelector;
    SdService& sdService;
    NvsService& nvsService;
    CategoryService& categoryService;
    EntryService& entryService;
    CryptoService& cryptoService;
    JsonTransformer& jsonTransformer;
    ModelTransformer& modelTransformer;

    GlobalState& globalState = GlobalState::getInstance();
};

#endif // VAULT_CONTROLLER_H
