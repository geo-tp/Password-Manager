#include "CardputerView.h"

namespace views {

M5GFX* CardputerView::Display = nullptr;

void CardputerView::initialize() {
    Display = &M5Cardputer.Display;
    Display->setRotation(1);
    Display->setTextColor(TEXT_COLOR);
    Display->fillScreen(BACKGROUND_COLOR);
    M5Cardputer.Display.setTextDatum(middle_center);
    M5Cardputer.Display.setFont(&fonts::Font0);
}

void CardputerView::topBar(const std::string& title, bool submenu, bool searchBar) {
    uint8_t marginX = 4;
    uint8_t marginY = 14;
    int offsetX; // for text align
    size_t limiter; // char limitation
    float sizeText; // pixels offset depending on text size

    clearTopBar();

    if (submenu) {
        drawSubMenuReturn(marginX+3, marginY); // for return <
        limiter = 20; // limit string size
        sizeText = 5.1; // pixels offset for each char
    } else {
        Display->setTextSize(TEXT_LARGE);
        limiter = 16;
        sizeText = 6.95;
    }
    
    if (searchBar) {
        Display->setTextColor(TEXT_COLOR);

        // Empty search query
        const std::string searchQuery = title.empty() ? "Type to search" : title.substr(0, limiter);
        drawSearchIcon(Display->width() - 20, marginY-2, 10, PRIMARY_COLOR);
        offsetX = getCenterOffset(searchQuery, Display->width());
        Display->setCursor(offsetX, marginY);
        Display->printf(searchQuery.c_str());
    } else {
        Display->setTextColor(TEXT_COLOR);
        Display->setTextSize(TEXT_BIG);
        offsetX = getCenterOffset(title, Display->width());
        Display->setCursor(offsetX, marginY);
        Display->printf(title.c_str());
    }
}

void CardputerView::horizontalSelection(
    const std::vector<std::string>& options,
    uint16_t selectedIndex,
    const std::string& description1,
    const std::string& description2,
    const std::vector<std::string>& icons) {

    if (icons.empty()) {
        horizontalSelectionWithoutIcons(options, selectedIndex, description1, description2);
    } else {
        horizontalSelectionWithIcons(options, selectedIndex, icons);
    }
}

void CardputerView::horizontalSelectionWithIcons(
    const std::vector<std::string>& options,
    uint16_t selectedIndex,
    const std::vector<std::string>& icons) {

    // Clear
    clearMainView(5);

    // Box frame
    // Display->drawRoundRect(1, 1, Display->width() - 1, Display->height() - 1, 5, RECT_COLOR_DARK);

    // Icon
    if (icons[selectedIndex] == "Create Vault") {
        drawFileIcon();
    } else if (icons[selectedIndex] == "Load Vault") {
        drawVaultIcon();
    } else if (icons[selectedIndex] == "Load File") {
        drawSdCardIcon();
    } else if (icons[selectedIndex] == "Settings") {
        drawSettingsIcon();
    } else if (icons[selectedIndex] == "New Password") {
        drawPlusIcon();
    } else if (icons[selectedIndex] == "Del Password") {
        drawMinusIcon();
    } else if (icons[selectedIndex] == "My Passwords") {
        drawLockIcon();
    }

    // Name box
    Display->fillRoundRect(38, 93, Display->width() - 77, 35, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
    Display->drawRoundRect(38, 93, Display->width() - 77, 35, DEFAULT_ROUND_RECT, PRIMARY_COLOR);

    // Name
    Display->setTextSize(TEXT_BIG);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(getCenterOffset(options[selectedIndex]), 110);
    Display->printf(options[selectedIndex].c_str());

    // Arrows for navigation
    Display->setTextColor(RECT_COLOR_DARK);
    Display->setCursor(17, 50);
    Display->printf("<");
    Display->setCursor(Display->width() - 30, 50);
    Display->printf(">");

    // Reset text color to default
    Display->setTextColor(PRIMARY_COLOR);
    Display->setTextSize(TEXT_MEDIUM);
}

void CardputerView::horizontalSelectionWithoutIcons(
    const std::vector<std::string>& options,
    uint16_t selectedIndex,
    const std::string& description1,
    const std::string& description2) {

    // Clear
    clearMainView(5);

    // Box frame
    Display->drawRoundRect(10, 35, Display->width() - 20, 90, DEFAULT_ROUND_RECT, PRIMARY_COLOR);

    // Infos
    Display->setTextSize(TEXT_WIDE);
    Display->setTextColor(PRIMARY_COLOR);
    Display->setCursor(getCenterOffset(description1), 48);
    Display->printf(description1.c_str());
    Display->setCursor(getCenterOffset(description2), 113);
    Display->printf(description2.c_str());

    // Name box
    Display->fillRoundRect(40, 63, Display->width() - 80, 35, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
    Display->drawRoundRect(40, 63, Display->width() - 80, 35, DEFAULT_ROUND_RECT, RECT_COLOR_LIGHT);

    // Name
    Display->setTextSize(TEXT_WIDE);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(getCenterOffset(options[selectedIndex]), 80);
    Display->printf(options[selectedIndex].c_str());

    // Arrows for navigation
    Display->setCursor(20, 78);
    Display->printf("<");
    Display->setCursor(Display->width() - 27, 78);
    Display->printf(">");

    // Reset text color to default
    Display->setTextColor(PRIMARY_COLOR);
    Display->setTextSize(TEXT_MEDIUM);
}

void CardputerView::verticalSelection(
    const std::vector<std::string>& options,
    uint16_t selectedIndex,
    size_t visibleRows,
    const std::vector<std::string>& optionLabels,
    const std::vector<std::string>& shortcuts,
    bool visibleMention) {

    if (options.empty()) {
        clearMainView();
        Display->setTextSize(TEXT_BIG);
        Display->fillRoundRect(50, 62, 137, 30, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
        Display->drawRoundRect(50, 62, 137, 30, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
        Display->drawCenterString("No results", 120, 70);
        return;
    }

    if (!optionLabels.empty()) {
        verticalSelectionWithLabelsAndShortcuts(options, selectedIndex, visibleRows, optionLabels, shortcuts, visibleMention);
    } else {
        verticalSelectionSimple(options, selectedIndex, visibleRows);
    }
}

void CardputerView::verticalSelectionSimple(
    const std::vector<std::string>& options,
    uint16_t selectedIndex,
    size_t visibleRows) {

    size_t totalOptions = options.size();
    size_t currentStartRow = selectedIndex / visibleRows * visibleRows;

    clearMainView();

    for (size_t i = 0; i < visibleRows; ++i) {
        size_t index = currentStartRow + i;
        if (index >= totalOptions) break;

        bool isSelected = (index == selectedIndex);
        int x = DEFAULT_MARGIN;
        int y = TOP_BAR_HEIGHT + (i * 26);

        drawRect(isSelected, DEFAULT_MARGIN, y, Display->width() - 13, 22, 0);

        Display->setCursor(x + 10, y + 12);
        Display->setTextSize(TEXT_LARGE);
        Display->setTextColor(TEXT_COLOR);

        Display->printf(truncateString(options[index], 20).c_str());
    }
}

void CardputerView::verticalSelectionWithLabelsAndShortcuts(
    const std::vector<std::string>& options,
    uint16_t selectedIndex,
    size_t visibleRows,
    const std::vector<std::string>& optionLabels,
    const std::vector<std::string>& shortcuts,
    bool visibleMention
    ) {

    size_t totalOptions = options.size();
    size_t currentStartRow = selectedIndex / visibleRows * visibleRows;

    clearMainView();

    for (size_t i = 0; i < visibleRows; ++i) {
        size_t index = currentStartRow + i;
        if (index >= totalOptions) break;

        bool isSelected = (index == selectedIndex);
        int x = DEFAULT_MARGIN;
        int y = TOP_BAR_HEIGHT + (i * 26);

        drawRect(isSelected, DEFAULT_MARGIN, y, Display->width() - 13, 22, 0);

        // Label
        Display->setTextSize(TEXT_SMALL);
        auto labelWidth = Display->textWidth(optionLabels[index].c_str());
        Display->fillRoundRect(x, y, labelWidth+15, 22, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
        Display->drawRoundRect(x, y, labelWidth+16, 22, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
        Display->setCursor(x + 8, y + 12);
        Display->setTextColor(PRIMARY_COLOR);
        Display->printf(optionLabels[index].c_str());

        // Option principale
        auto truncatedOption = truncateString(options[index], 18);
        truncatedOption = optionLabels[index] == "Pass" ? std::string(truncatedOption.size(), '*') : truncatedOption;
        Display->setCursor(labelWidth+28, y + 12);
        Display->setTextSize(TEXT_WIDE);
        Display->setTextColor(TEXT_COLOR);
        Display->printf(truncatedOption.c_str());

        // Raccourci
        if (!shortcuts.empty()) {
            Display->fillRoundRect(Display->width() - 44, y - 1, 39, 22, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
            Display->drawRoundRect(Display->width() - 45, y, 40, 22, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
            Display->setTextColor(TEXT_COLOR);
            Display->setTextSize(TEXT_WIDE);
            Display->setCursor(Display->width() - 28, y+12);
            Display->printf(shortcuts[index].c_str());
        }

    }
    
    // Keyboard mention
    if (visibleMention) {
        Display->setTextSize(TEXT_WIDE);
        Display->setTextColor(PRIMARY_COLOR);
        std::string mention = "Plug in USB as keyboard";
        Display->setCursor(getCenterOffset(mention), 122);
        Display->printf(mention.c_str());
    }
}

void CardputerView::subMessage(std::string message, int delayMs) {
    // Clear
    clearMainView(5);

    // Box frame
    Display->drawRoundRect(10, 35, Display->width() - 20, 90, DEFAULT_ROUND_RECT, PRIMARY_COLOR);

    // Description
    Display->setTextSize(TEXT_WIDE);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(getCenterOffset(message), 80);
    Display->printf(message.c_str());
    Display->setTextSize(TEXT_MEDIUM);

    if (delayMs) {
        delay(delayMs);
    }
}

void CardputerView::stringPrompt(std::string label, std::string value, bool backButton, size_t minLength) {
    // Clear
    clearMainView(5);

    // Box frame
    Display->drawRoundRect(10, 35, Display->width() - 20, 90, DEFAULT_ROUND_RECT, PRIMARY_COLOR);

    // Description
    Display->setTextSize(TEXT_MEDIUM);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(getCenterOffset(label), 48);
    Display->printf(label.c_str());

    // Check the length of the input and truncate if necessary
    std::string truncatedInput;
    if (value.length() > 20) {
        truncatedInput = value.substr(value.length() - 20); // Get the last characters
    } else {
        truncatedInput = value;
    }

    // input
    Display->setTextSize(TEXT_MEDIUM_LARGE);
    Display->fillRoundRect(42, 62, 155, 25, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
    Display->setCursor(51, 73);
    Display->printf(truncatedInput.c_str());

    size_t xPos = 110; 
    if (backButton) {
        // < button
        Display->fillRoundRect(53, 95, 40, 20, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
        Display->drawRoundRect(53, 95, 40, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
        Display->setCursor(70, 105);
        Display->printf("<");
        xPos = 135;
    }

    // Button ok
    if (value.length() < minLength) {
        Display->fillRoundRect(xPos-30, 95, 80, 20, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
        Display->drawRoundRect(xPos-30, 95, 80, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    } else {
        Display->fillRoundRect(xPos-30, 95, 80, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    }

    Display->setTextSize(TEXT_MEDIUM);
    Display->setCursor(xPos+5, 106);
    Display->printf("OK");
}

void CardputerView::confirmationPrompt(std::string label) {
    // Clear
    clearMainView(5);

    // Box frame
    Display->drawRoundRect(10, 35, Display->width() - 20, 90, DEFAULT_ROUND_RECT, PRIMARY_COLOR);

    // Description
    Display->setTextSize(TEXT_MEDIUM);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(getCenterOffset(label), 62);
    Display->printf(label.c_str());
    Display->setTextSize(TEXT_MEDIUM);

    // < button
    Display->fillRoundRect(65, 85, 40, 20, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
    Display->drawRoundRect(65, 85, 40, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    Display->setCursor(82, 96);
    Display->printf("<");

    // ok button
    Display->fillRoundRect(128, 85, 40, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    Display->setCursor(143, 96);
    Display->printf("OK");
}

void CardputerView::value(std::string label, std::string value) {
    // Clear the main view area
    clearMainView(5);

    // Initial text size
    uint8_t textSize = TEXT_BIG;

    // Adjust text size to fit the screen width
    adjustTextSizeToFit(value, Display->width() - 20, textSize);

    // Determine position
    auto x = getCenterOffset(value);
    auto y = 40;
    auto biggerThanScreen = Display->textWidth(value.c_str()) > Display->width();

    // Draw background if the text fits on the screen
    if (!biggerThanScreen) {
        y = 60;
        Display->fillRoundRect(x - 13, y - 15, Display->textWidth(value.c_str()) + 25, 32, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
        Display->drawRoundRect(x - 13, y - 15, Display->textWidth(value.c_str()) + 25, 32, DEFAULT_ROUND_RECT, RECT_COLOR_LIGHT);
    }

    Display->setCursor(x, y);
    Display->setTextColor(TEXT_COLOR);
    Display->printf(value.c_str());

    // Display "M" button
    Display->setTextSize(1.6);
    Display->fillRoundRect(38, 95, 20, 15, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(45, 101);
    Display->printf("m");

    Display->setCursor(66, 102);
    Display->setTextColor(PRIMARY_COLOR);
    Display->printf("to modify field");

    // Display "OK" button
    Display->fillRoundRect(33, 117, 30, 15, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(40, 125);
    Display->printf("ok");

    // Display->setTextSize(TEXT_TINY);
    Display->setCursor(72, 125);
    Display->setTextColor(PRIMARY_COLOR);
    Display->printf("to send via USB");

    Display->setTextColor(TEXT_COLOR);
}

void CardputerView::drawRect(bool selected, uint8_t margin, uint16_t startY, uint16_t sizeX, uint16_t sizeY, uint16_t stepY) {
        // Draw rect
        if (selected) {
            Display->fillRoundRect(margin, startY + stepY, sizeX, sizeY, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
            Display->setTextColor(TEXT_COLOR);
        } else {
            Display->fillRoundRect(margin, startY + stepY , sizeX, sizeY, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
            Display->drawRoundRect(margin, startY + stepY, sizeX, sizeY, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
            Display->setTextColor(TEXT_COLOR);
        }
}

void CardputerView::drawSubMenuReturn(uint8_t x, uint8_t y) {
    Display->setTextSize(TEXT_WIDE);
    Display->setTextColor(PRIMARY_COLOR);
    Display->setCursor(x, y);
    Display->printf("<");
}

void CardputerView::drawSearchIcon(int x, int y, int size, uint16_t color) {
    int radius = size / 2;
    int handleLength = size / 2;

    // Dessiner cercle
    Display->drawCircle(x, y, radius, color);
    // Dessiner poignée
    Display->drawLine(x + radius, y + radius, x + radius + handleLength, y + radius + handleLength, color);
}

void CardputerView::clearMainView(uint8_t offsetY) {
    Display->fillRect(0, TOP_BAR_HEIGHT-offsetY, Display->width(), Display->height(), BACKGROUND_COLOR);
}

void CardputerView::clearTopBar() {
    Display->fillRect(0, 0, Display->width(), TOP_BAR_HEIGHT, BACKGROUND_COLOR);
}

void CardputerView::debug(const std::string& message) {
    Display->setTextSize(TEXT_MEDIUM);
    Display->fillScreen(TFT_BLACK);
    Display->setCursor(100, 10);
    Display->printf("DEBUG");
    Display->setCursor(10, 50);
    Display->printf(message.c_str());
    delay(3000);
}

void CardputerView::setBrightness(uint16_t brightness) {
    Display->setBrightness(brightness);
}

uint8_t CardputerView::getBrightness() {
    return Display->getBrightness();
}

int CardputerView::getCenterOffset(const std::string& text, int screenWidth) {
    // Measure the width of the text in pixels using the current font and size
    int textWidth = Display->textWidth(text.c_str());

    // Calculate the centered X position
    return (screenWidth - textWidth) / 2;
}

void CardputerView::drawVaultIcon(int x, int y, uint16_t color) {
    // Taille ajustée de l'icône
    int width = 70;  // Largeur du coffre
    int height = 50; // Hauteur de la base
    int lidHeight = 20; // Hauteur du couvercle

    // Fond de l'icône
    Display->fillRect(x - 5, y - 5, width + 10, height + lidHeight + 10, BACKGROUND_COLOR);

    // Dessin du coffre
    // Base rectangulaire du coffre
    Display->fillRect(x, y + lidHeight, width, height, PRIMARY_COLOR);

    // Dessin du couvercle
    Display->fillRect(x, y, width, lidHeight, PRIMARY_COLOR);

    // Dessin de la séparation entre le couvercle et la base
    Display->drawLine(x, y + lidHeight, x + width, y + lidHeight, RECT_COLOR_DARK);

    // Ajout d'une poignée rectangulaire sur le couvercle
    int handleWidth = 30;
    int handleHeight = 5;
    Display->fillRect(x + (width / 2) - (handleWidth / 2), y + 5, handleWidth, handleHeight, RECT_COLOR_DARK);

    // Serrure centrale
    int lockWidth = 12;
    int lockHeight = 18;
    int lockX = x + (width / 2) - (lockWidth / 2);
    int lockY = y + lidHeight + (height / 2) - (lockHeight / 2);
    Display->fillRect(lockX, lockY, lockWidth, lockHeight, RECT_COLOR_DARK);

    // Dessin de l'intérieur de la serrure (cylindre)
    int lockInnerWidth = 4;
    int lockInnerHeight = 6;
    int lockInnerX = lockX + (lockWidth / 2) - (lockInnerWidth / 2);
    int lockInnerY = lockY + 6;
    Display->fillRect(lockInnerX, lockInnerY, lockInnerWidth, lockInnerHeight, PRIMARY_COLOR);

    // Renforcement visuel avec des contours
    Display->drawRect(x, y, width, lidHeight, RECT_COLOR_DARK); // Contour du couvercle
    Display->drawRect(x, y + lidHeight, width, height, RECT_COLOR_DARK); // Contour de la base

    // Ajout de renforts sur le coffre
    int strapWidth = 4;
    Display->fillRect(x + 10, y + lidHeight, strapWidth, height, RECT_COLOR_DARK); // Renfort gauche
    Display->fillRect(x + width - 10 - strapWidth, y + lidHeight, strapWidth, height, RECT_COLOR_DARK); // Renfort droit
}

void CardputerView::drawSettingsIcon(int x, int y) {
  Display->fillRect(x,y,80,80,BACKGROUND_COLOR);
  int i=0;
  for(i=0;i<6;i++) {
    Display->fillArc(40 + x, 40 + y, 30, 20, 15 + 60 * i, 45 + 60 * i, PRIMARY_COLOR);
  }
  Display->fillArc(40 + x, 40 + y, 22, 8, 0, 360, PRIMARY_COLOR);
}

void CardputerView::drawFileIcon(int x, int y) {
    Display->fillRect(x-10, y, 90, 80, BACKGROUND_COLOR);

    int iconW = 58;  // Largeur de l'icône
    int iconH = 72;  // Hauteur de l'icône
    int foldSize = iconH / 4;  // Taille du pli en haut à droite

    // Dessin du fichier
    Display->fillRect(x, y, iconW, iconH, RECT_COLOR_DARK);
    Display->drawRect(x, y, iconW, iconH, PRIMARY_COLOR);

    // Effacer les coins pour le pli
    Display->fillRect(x + iconW - foldSize, y - 1, foldSize, 2, BACKGROUND_COLOR);
    Display->fillRect(x + iconW - 1, y, 2, foldSize, BACKGROUND_COLOR);
    Display->fillTriangle(x + iconW, y, x + iconW - foldSize, y, x + iconW, y + foldSize, BACKGROUND_COLOR);

    // Dessin du pli
    Display->drawTriangle(
        x + iconW - foldSize, y,
        x + iconW - foldSize, y + foldSize - 1,
        x + iconW - 1, y + foldSize - 1,
        PRIMARY_COLOR
    );

    // Dessin du petit coffre au centre
    int chestWidth = iconW / 2;  // Largeur du coffre
    int chestHeight = iconH / 4;  // Hauteur du coffre
    int chestX = x + (iconW / 2) - (chestWidth / 2);  // Centrage horizontal
    int chestY = y + (iconH / 2) - (chestHeight / 2);  // Centrage vertical
    int lidHeight = chestHeight / 3;  // Hauteur du couvercle

    // Base du coffre
    Display->fillRect(chestX, chestY + lidHeight, chestWidth, chestHeight - lidHeight, PRIMARY_COLOR);
    Display->drawRect(chestX, chestY + lidHeight, chestWidth, chestHeight - lidHeight, RECT_COLOR_DARK);

    // Couvercle du coffre
    Display->fillRect(chestX, chestY, chestWidth, lidHeight, PRIMARY_COLOR);
    Display->drawRect(chestX, chestY, chestWidth, lidHeight, RECT_COLOR_DARK);

    // Serrure du coffre
    int lockWidth = chestWidth / 6;
    int lockHeight = chestHeight / 6;
    int lockX = chestX + (chestWidth / 2) - (lockWidth / 2);
    int lockY = chestY + lidHeight + (chestHeight / 2) - (lockHeight / 2);
    Display->fillRect(lockX, lockY-4, lockWidth, lockHeight, RECT_COLOR_DARK);
}

void CardputerView::drawSdCardIcon(int x, int y) {
    Display->fillRect(x-10, y, 90, 80, BACKGROUND_COLOR);

    // Taille de l'icône
    int cardWidth = 60;  // Largeur de la carte SD
    int cardHeight = 70; // Hauteur de la carte SD
    int notchWidth = 12; // Largeur de l'encoche
    int notchHeight = 15; // Hauteur de l'encoche

    // Dessiner le corps principal de la carte SD
    Display->drawRoundRect(x, y, cardWidth, cardHeight, DEFAULT_ROUND_RECT, PRIMARY_COLOR);

    // Ajout d'une ligne pour délimiter la zone des contacts
    // int contactZoneHeight = 10;
    // Display->drawLine(x + 5, y + cardHeight - contactZoneHeight - 1, x + cardWidth - 5, y + cardHeight - contactZoneHeight - 1, BACKGROUND_COLOR);

    // Dessiner les contacts métalliques
    int contactWidth = 6; // Largeur de chaque contact
    int contactSpacing = 4; // Espacement entre les contacts
    int numContacts = 5; // Nombre de contacts
    int contactStartX = x + 7;
    int contactStartY = y + cardHeight - 10 + 2;

    // Ajouter des détails au centre de la carte SD
    int innerRectX = x + 8;
    int innerRectY = y + 8;
    int innerRectWidth = cardWidth - 16;
    int innerRectHeight = cardHeight - 20;
    Display->fillRoundRect(innerRectX, innerRectY, innerRectWidth, innerRectHeight, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
    Display->drawRoundRect(innerRectX, innerRectY, innerRectWidth, innerRectHeight, DEFAULT_ROUND_RECT, PRIMARY_COLOR);

    // // Dessiner "SD" au milieu du carré central
    int textX = innerRectX + innerRectWidth / 2;
    int textY = innerRectY + 10 + innerRectHeight / 2;

    Display->setTextSize(2); // Taille du texte
    Display->setTextColor(PRIMARY_COLOR);
    Display->setCursor(textX - (Display->textWidth("SD") / 2), textY - (Display->fontHeight() / 2));
    Display->print("SD");


    for (int i = 0; i < numContacts; i++) {
        Display->fillRect(contactStartX + i * (contactWidth + contactSpacing), contactStartY, contactWidth, 6, PRIMARY_COLOR);
    }
}

void CardputerView::drawLockIcon(int x, int y, uint16_t color) {
    Display->fillRect(x-10, y, 90, 80, BACKGROUND_COLOR);
    int lockWidth = 60;     // Largeur du corps du cadenas
    int lockHeight = 45;    // Hauteur du corps du cadenas
    int shackleWidth = 40;  // Largeur de l'anse
    int shackleHeight = 20; // Hauteur de l'anse
    int shackleOffset = 10; // Décalage vertical de l'anse par rapport au corps

    // Fond de l'icône
    Display->fillRect(x - lockWidth / 2 - 2, y - shackleHeight - lockHeight - 4, 
                      lockWidth + 4, lockHeight + shackleHeight + 6, BACKGROUND_COLOR);

    // Dessiner l'anse
    int shackleStartX = x - shackleWidth / 2;
    int shackleStartY = y - lockHeight - shackleOffset;
    Display->fillArc(shackleStartX + shackleWidth / 2, shackleStartY + shackleHeight / 2,
                     shackleWidth / 2, shackleWidth / 2 - 4, 180, 360, color);

    // Dessiner le corps du cadenas
    int lockX = x - lockWidth / 2;
    int lockY = y - lockHeight;
    Display->fillRect(lockX, lockY, lockWidth, lockHeight, color);

    // Dessiner l'intérieur du cadenas
    int innerLockWidth = lockWidth - 4;
    int innerLockHeight = lockHeight - 4;
    Display->fillRect(lockX + 2, lockY + 2, innerLockWidth, innerLockHeight, RECT_COLOR_DARK);

    // Ajouter un cercle pour représenter la serrure
    int lockCenterX = lockX + lockWidth / 2;
    int lockCenterY = lockY + lockHeight / 2;
    int lockHoleRadius = 5;

    Display->drawCircle(lockCenterX, lockCenterY, lockHoleRadius, RECT_COLOR_DARK);
    Display->fillCircle(lockCenterX, lockCenterY, lockHoleRadius - 2, color);
}

void CardputerView::drawPlusIcon(int x, int y, uint16_t color) {
    Display->fillRect(x-30, y-90, 120, 90, BACKGROUND_COLOR);
    int size = 50;
    int thickness = size / 5; // Épaisseur des lignes
    int halfSize = size / 2;

    // Fond de l'icône
    Display->fillRect(x - size / 2 - 2, y - size / 2 - 2, size + 4, size + 4, BACKGROUND_COLOR);

    // Ligne horizontale
    Display->fillRect(x - halfSize, y - thickness / 2, size, thickness, RECT_COLOR_DARK);
    Display->drawRect(x - halfSize, y - thickness / 2, size, thickness, color);

    // Ligne verticale
    Display->fillRect(x - thickness / 2, y - halfSize, thickness, size, RECT_COLOR_DARK);
    Display->drawRect(x - thickness / 2, y - halfSize, thickness, size, color);
}

void CardputerView::drawMinusIcon(int x, int y, uint16_t color) {
    Display->fillRect(x-30, y-90, 120, 90, BACKGROUND_COLOR);
    int size = 50;
    int thickness = size / 5;
    int halfSize = size / 2;

    // Fond de l'icône
    Display->fillRect(x - size / 2 - 2, y - size / 2 - 2, size + 4, size + 4, BACKGROUND_COLOR);

    // Ligne horizontale
    Display->fillRect(x - halfSize, y - thickness / 2, size, thickness, RECT_COLOR_DARK);
    Display->drawRect(x - halfSize, y - thickness / 2, size, thickness, color);
}


std::string CardputerView::truncateString(const std::string& input, size_t maxLength) {
    const std::string ellipsis = "...";

    if (input.length() <= maxLength) {
        return input;
    }

    // Calcul char number each side
    size_t halfLength = (maxLength - ellipsis.length()) / 2;

    // Start of the fist string, end of the second
    std::string firstPart = input.substr(0, halfLength);
    std::string secondPart = input.substr(input.length() - halfLength);

    // Concat with "..."
    return firstPart + ellipsis + secondPart;
}

void CardputerView::adjustTextSizeToFit(const std::string& text, uint16_t maxWidth, uint8_t& textSize) {
    // Réduction de la taille du texte tant qu'il dépasse la largeur autorisée
    while (Display->textWidth(text.c_str()) > maxWidth && textSize > TEXT_SMALL) {
        textSize -= 0.01; // Réduction de la taille du texte
        Display->setTextSize(textSize);
    }
}

}
