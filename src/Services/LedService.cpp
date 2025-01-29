#include "LedService.h"

void LedService::blink() {
    showLed();
    delay(30);
    clearLed();
}

void LedService::showLed() {
    const int ledPin = globalState.getLedPin(); // TODO: no way to pass this to FastLED atm
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, 1); // we use define LED_PIN Builtin for now
    leds[0] = CRGB::OrangeRed;
    FastLED.show();
}

void LedService::clearLed() {
    FastLED.clear(true);
}