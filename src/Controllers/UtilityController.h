#ifndef UTILITY_CONTROLLER_H
#define UTILITY_CONTROLLER_H

#include <Services/KeyboardService.h>
#include <Services/LedService.h>
#include <Services/NvsService.h>
#include <Services/SdService.h>
#include <Selectors/HorizontalSelector.h>
#include <Selectors/VerticalSelector.h>
#include <Selectors/FieldEditorSelector.h>
#include <Selectors/StringPromptSelector.h>
#include <Selectors/ConfirmationSelector.h>
#include <Transformers/TimeTransformer.h>
#include <Views/IView.h>
#include <Inputs/IInput.h>
#include <Enums/ActionEnum.h>
#include <Enums/KeyboardLayoutEnum.h>
#include <vector>
#include <string>

class UtilityController {
public:
    UtilityController(IView& display,
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
                    TimeTransformer& timeTransformer);


    bool handleUsbTyping(std::string sendString);
    bool handleKeyboardInitialization();
    bool handleGeneralSettings();
    void handleLoadNvs();
    void handleWelcome();
    void handleInactivity();

private:
    IView& display;
    IInput& input;
    KeyboardService& keyboardService;
    LedService& ledService;
    NvsService& nvsService;
    SdService& sdService;

    TimeTransformer& timeTransformer;

    HorizontalSelector& horizontalSelector;
    VerticalSelector& verticalSelector;
    FieldEditorSelector& fieldEditorSelector;
    StringPromptSelector& stringPromptSelector;
    ConfirmationSelector& confirmationSelector;
    
    GlobalState& globalState = GlobalState::getInstance();
};

#endif // UTILITY_CONTROLLER_H
