#include "VerticalSelector.h"
#include <algorithm>
#include <cctype>

VerticalSelector::VerticalSelector(IView& display, IInput& input, InactivityManager& inactivityManager)
    : display(display), input(input), inactivityManager(inactivityManager) {}


int VerticalSelector::select(const std::string& title, const std::vector<std::string>& options, bool subMenu, bool searchBar, const std::vector<std::string>& options2,  const std::vector<std::string>& shortcuts, bool visibleMention) {
    int currentIndex = 0;
    int lastIndex = -1;
    int lastQuerySize = 0;
    char key = KEY_NONE;
    std::string searchQuery;
    std::vector<std::string> filteredOptions = options;
    inactivityManager.reset();

    while (true) {
        inactivityManager.update();

        if (inactivityManager.getVaultIsLocked()) {
            return -1;
        }

        // Display the current options and selection
        if (lastIndex != currentIndex || lastQuerySize != searchQuery.size()) {
            display.topBar(searchQuery.empty() ? title : searchQuery, subMenu, searchBar);
            display.verticalSelection(filteredOptions, currentIndex, 4, options2, shortcuts, visibleMention);
            lastIndex = currentIndex;
            lastQuerySize = searchQuery.size();
        }

        // Capture user input
        key = input.handler();

        if (key != KEY_NONE) {
            inactivityManager.reset();
        }

        switch (key) {
            case KEY_ARROW_UP:
                currentIndex = (currentIndex > 0) ? currentIndex - 1 : filteredOptions.size() - 1;
                break;
            case KEY_ARROW_DOWN:
                currentIndex = (currentIndex < filteredOptions.size() - 1) ? currentIndex + 1 : 0;
                break;
            case KEY_OK:
                for (size_t i = 0; i < options.size(); ++i) {
                    if (options[i] == filteredOptions[currentIndex]) {
                        return i;
                    }
                }
                break;
            case KEY_ESC_CUSTOM:
            case KEY_ARROW_LEFT: // Back
                return -1;
            case KEY_DEL: // Backspace for text search
                if (searchBar && !searchQuery.empty()) {
                    searchQuery.pop_back();
                    filteredOptions = filterOptions(options, searchQuery);
                    currentIndex = 0;
                } else {
                    filteredOptions = options;
                }
                break;
            default:
                if (searchBar && std::isalnum(key)) {
                    searchQuery += key;
                    filteredOptions = filterOptions(options, searchQuery);
                    currentIndex = 0;
                }
                break;
        }
    }
}

std::string VerticalSelector::toLowerCase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::vector<std::string> VerticalSelector::filterOptions(const std::vector<std::string>& options, const std::string& query) {
    std::vector<std::string> filtered;
    std::string lowerQuery = toLowerCase(query);
    for (const auto& option : options) {
        if (toLowerCase(option).find(lowerQuery) != std::string::npos) {
            filtered.push_back(option);
        }
    }
    return filtered;
}
