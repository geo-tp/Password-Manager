#include "CardputerInput.h"

namespace inputs {


char CardputerInput::handler() {

    // Update keyboard state
    M5Cardputer.update();

    // Bouton GO
    if (M5Cardputer.BtnA.isPressed()) {
        delay(150); // debounce
        return KEY_ESC_CUSTOM;
    }
    
    if (M5Cardputer.Keyboard.isChange()) {

        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

            if (status.enter) { // short OK or one-second long OK
                auto pressedAt = millis();
                while (true) {
                    M5Cardputer.update();
                    if (!M5Cardputer.Keyboard.keysState().enter) {
                        return KEY_OK;
                    }
                    if (millis() - pressedAt >= 1000) {
                        return KEY_OK_LONG;
                    }
                    delay(10);
                }
            }
            if (status.del) { 
                return KEY_DEL;
            }
            
            if(M5Cardputer.Keyboard.isKeyPressed(KEY_ARROW_LEFT)) { // go back to previous menu
                return KEY_ARROW_LEFT;
            }

            if(M5Cardputer.Keyboard.isKeyPressed(KEY_ARROW_RIGHT)) { // go to next menu
                return KEY_ARROW_RIGHT;
            }

            for (auto c : status.word) {
                // Issue with %, the only key that requires 2 inputs to display
                if (c == '%') {
                    return '5'; 
                }
                return c; // retourner le premier char saisi
            }
        }
    }
    delay(10); // debounce
    return KEY_NONE;
}

void CardputerInput::waitPress() {
  while (true) {
    M5Cardputer.update();
    if (M5Cardputer.BtnA.isPressed() || M5Cardputer.Keyboard.isPressed()) {
      return;
    }
    delay(10);
  }
}

}