#include "display.h"
/*------------------------------------------------------------------------------
 * DISPLAY CLASS
 *----------------------------------------------------------------------------*/
void Display::begin() {
  FastLED.addLeds<WS2813, A1_PIN, GRB>(leds, 0 * STRIP, STRIP);
  FastLED.addLeds<WS2813, A2_PIN, GRB>(leds, 1 * STRIP, STRIP);
  FastLED.addLeds<WS2813, A3_PIN, GRB>(leds, 2 * STRIP, STRIP);

  FastLED.addLeds<WS2813, A7_PIN, GRB>(leds, 3 * STRIP, STRIP);
  FastLED.addLeds<WS2813, A5_PIN, GRB>(leds, 4 * STRIP, STRIP);
  FastLED.addLeds<WS2813, A6_PIN, GRB>(leds, 5 * STRIP, STRIP);
  FastLED.setBrightness(255);
  FastLED.setDither(DISABLE_DITHER);
}
void Display::update() { FastLED.show(); }
void Display::fade(uint8_t i) { fadeToBlackBy(leds, PIXELS, i); }
