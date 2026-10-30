#ifndef MOCK_VIEW_H
#define MOCK_VIEW_H

#include "../src/Views/IView.h"
#include <vector>
#include <string>

class MockView : public IView {
public:
    MockView() {}

    void initialize() override {
        initialized = true;
    }

    void welcome(uint8_t defaultBrightness) override {
        welcomeCalled = true;
    }   

    void setBrightness(uint16_t brightness) override {
        this->brightness = brightness;
    }

    uint8_t getBrightness() override {
        return this->brightness;
    }

    void topBar(const std::string& title, bool submenu, bool searchBar) override {
        lastTitle = title;
        this->submenu = submenu;
        this->searchBar = searchBar;
        topBarCalled = true;
    }

    void horizontalSelection(const std::vector<std::string>& options, uint16_t selectedIndex, const std::string& description1 = "", const std::string& description2 = "", const std::vector<std::string>& icons = {}) override {
        displayedOptions = options;
        lastSelectedIndex = selectedIndex;
        horizontalSelectionCalled = true;
    }
    
    void verticalSelection(
        const std::vector<std::string>& options, 
        uint16_t selectedIndex, 
        size_t visibleRows, 
        const std::vector<std::string>& optionLabels, 
        const std::vector<std::string>& shortcuts,
        bool visibleMention) override {

        displayedOptions = options;
        displayedOptionLabels = optionLabels;
        lastSelectedIndex = selectedIndex;
        this->visibleRows = visibleRows;
        verticalSelectionCalled = true;
    }

    void value(std::string label, std::string val) {
        valueCalled = true;
    }

    void subMessage(std::string message, int delayMs) {
        message = message;
        subMessageCalled = true;
    }

    void confirmationPrompt(std::string label) {
        confirmationPromptCalled = true;
    }

    void stringPrompt(std::string label, std::string value, bool backButton, size_t minLength) {
        stringPromptCalled = true;
    }

    void debug(const std::string& message) override {
        debugMessage = message;
        debugCalled = true;
    }

    // Public attributes for testing
    bool initialized = false;
    uint16_t brightness = 0;
    std::string lastTitle;
    bool submenu = false;
    bool searchBar = false;
    std::vector<std::string> displayedOptions;
    std::vector<std::string> displayedOptionLabels;
    uint16_t lastSelectedIndex = -1;
    size_t visibleRows = 0;
    std::string message;
    std::string debugMessage;
    bool welcomeCalled = false;
    bool topBarCalled = false;
    bool verticalSelectionCalled = false;
    bool horizontalSelectionCalled = false;
    bool valueCalled = false;
    bool subMessageCalled = false;
    bool debugCalled = false;
    bool confirmationPromptCalled = false;
    bool stringPromptCalled = false;
};

#endif // MOCK_VIEW_H
