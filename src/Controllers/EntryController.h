#ifndef ENTRY_CONTROLLER_H
#define ENTRY_CONTROLLER_H

#include <Selectors/VerticalSelector.h>
#include <Selectors/HorizontalSelector.h>
#include <Selectors/FieldActionSelector.h>
#include <Selectors/StringPromptSelector.h>
#include <Selectors/ConfirmationSelector.h>
#include <Services/EntryService.h>
#include <Services/CryptoService.h>
#include <Services/KeyboardService.h>
#include <Services/LedService.h>
#include <Services/NvsService.h>
#include <Enums/ActionEnum.h>
#include <Enums/KeyboardLayoutEnum.h>
#include <Enums/IconEnum.h>
#include <Models/Entry.h>
#include <Models/Field.h>
#include <Transformers/ModelTransformer.h>
#include <States/GlobalState.h>
#include <vector>
#include <string>

class EntryController {
public:
    EntryController(IView& display,
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
                    ModelTransformer& modelTransformer);

    ActionEnum actionFieldSelected(Field field);

    Entry handleEntrySelection();
    Field handleFieldSelection(Entry& selectedEntry);

    bool handleEntryCreation();
    bool handleEntryUpdate(Entry& entry, Field& field);
    bool handleEntryDeletion();

private:
    IView& display;
    IInput& input;
    HorizontalSelector& horizontalSelector;
    VerticalSelector& verticalSelector;
    FieldActionSelector& fieldActionSelector;
    ConfirmationSelector& confirmationSelector;
    StringPromptSelector& stringPromptSelector;
    EntryService& entryService;
    CryptoService& cryptoService;
    KeyboardService& keyboardService;
    LedService& ledService;
    NvsService& nvsService;
    ModelTransformer& modelTransformer;
    
    GlobalState& globalState = GlobalState::getInstance();
};

#endif // ENTRY_CONTROLLER_H
