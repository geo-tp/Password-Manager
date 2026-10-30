#include "VerticalSelector.h"
#include <algorithm>
#include <cctype>

VerticalSelector::VerticalSelector(IView& display, IInput& input, InactivityManager& inactivityManager)
    : display(display), input(input), inactivityManager(inactivityManager) {}


int VerticalSelector::select(
        const std::string& title, 
        const std::vector<std::string>& options, 
        bool subMenu, 
        bool searchBar, 
        const std::vector<std::string>& options2,
        const std::vector<std::string>& shortcuts, 
        bool visibleMention,
        bool handleInactivity,
        bool* deleteRequested)
{
    if (deleteRequested) {
        *deleteRequested = false;
    }
    int currentIndex = 0;
    int lastIndex = -1;
    int lastQuerySize = 0;
    char key = KEY_NONE;
    std::string searchQuery;
    std::vector<size_t> filteredIndices = filterOptionIndices(options, searchQuery);
    inactivityManager.reset();

    while (true) {
        // Inactivity
        if (handleInactivity) {
            inactivityManager.update();
            if (inactivityManager.getVaultIsLocked()) {return -1;}
        }

        // Display the current options and selection
        if (lastIndex != currentIndex || lastQuerySize != searchQuery.size()) {
            std::vector<std::string> filteredOptions;
            std::vector<std::string> filteredOptions2;
            std::vector<std::string> filteredShortcuts;
            for (auto index : filteredIndices) {
                filteredOptions.push_back(options[index]);
                if (!options2.empty()) { filteredOptions2.push_back(options2[index]); }
                if (!shortcuts.empty()) { filteredShortcuts.push_back(shortcuts[index]); }
            }
            display.topBar(searchQuery.empty() ? title : searchQuery, subMenu, searchBar);
            display.verticalSelection(filteredOptions, currentIndex, 4, filteredOptions2, filteredShortcuts, visibleMention);
            lastIndex = currentIndex;
            lastQuerySize = searchQuery.size();
        }

        // Capture user input
        key = input.handler();

        if (handleInactivity && key != KEY_NONE) {
            inactivityManager.reset();
        }

        // Verify shortcuts
        if (!shortcuts.empty()) {
            int shortcutIndex = checkShortcut(shortcuts, key);
            if (shortcutIndex != -1) {
                return shortcutIndex;
            }
        }

        switch (key) {
            case KEY_ARROW_UP:
                if (!filteredIndices.empty()) {
                    currentIndex = (currentIndex > 0) ? currentIndex - 1 : filteredIndices.size() - 1;
                }
                break;
            case KEY_ARROW_DOWN:
                if (!filteredIndices.empty()) {
                    currentIndex = (currentIndex < filteredIndices.size() - 1) ? currentIndex + 1 : 0;
                }
                break;
            case KEY_OK:
                if (!filteredIndices.empty()) { return filteredIndices[currentIndex]; }
                break;
            case KEY_ESC_CUSTOM:
            case KEY_ARROW_LEFT: // Back
                return -1;
            case KEY_DEL:
                if (searchBar && !searchQuery.empty()) {
                    searchQuery.pop_back();
                    filteredIndices = filterOptionIndices(options, searchQuery);
                    currentIndex = 0;
                } else if (deleteRequested && !filteredIndices.empty()) {
                    *deleteRequested = true;
                    return filteredIndices[currentIndex];
                }
                break;
            default:
                if (searchBar && std::isalnum(key)) {
                    searchQuery += key;
                    filteredIndices = filterOptionIndices(options, searchQuery);
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

std::vector<size_t> VerticalSelector::filterOptionIndices(const std::vector<std::string>& options, const std::string& query) {
    std::vector<size_t> filtered;
    std::string lowerQuery = toLowerCase(query);
    for (size_t i = 0; i < options.size(); ++i) {
        if (toLowerCase(options[i]).find(lowerQuery) != std::string::npos) {
            filtered.push_back(i);
        }
    }
    return filtered;
}

int VerticalSelector::checkShortcut(const std::vector<std::string>& shortcuts, char key) {
    for (size_t i = 0; i < shortcuts.size(); ++i) {
        if (!shortcuts[i].empty() && std::tolower(key) == std::tolower(shortcuts[i][0])) {
            return i; // Retourne l'index du raccourci correspondant
        }
    }
    return -1; // Aucun raccourci correspondant trouvé
}

