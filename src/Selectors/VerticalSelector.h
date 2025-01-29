#ifndef VERTICAL_SELECTOR_H
#define VERTICAL_SELECTOR_H

#include <Arduino.h>
#include <string>
#include <vector>
#include <Views/IView.h>
#include <Inputs/IInput.h>
#include <States/GlobalState.h>
#include <Managers/InactivityManager.h>

class VerticalSelector {
public:
    VerticalSelector(IView& display, IInput& input, InactivityManager& inactivityManager);
    int select(const std::string& title, const std::vector<std::string>& options, bool subMenu = false, bool searchBar = false,  const std::vector<std::string>& options2={},  const std::vector<std::string>& shortcuts={}, bool visibleMention=false, bool handleInactivity=true);

private:
    IView& display;
    IInput& input;
    InactivityManager& inactivityManager;

    std::string toLowerCase(const std::string& input);
    std::vector<std::string> filterOptions(const std::vector<std::string>& options, const std::string& query);
    GlobalState& globalState = GlobalState::getInstance();
};

#endif // VERTICAL_SELECTOR_H
