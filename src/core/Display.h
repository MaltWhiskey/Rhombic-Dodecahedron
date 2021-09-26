#ifndef DISPLAY_H
#define DISPLAY_H
#define FASTLED_ESP32_I2S true
#include <Arduino.h>
#include <FastLED.h>

class Display {
 private:
  static const uint8_t A1_PIN = 32;
  static const uint8_t A2_PIN = 12;
  static const uint8_t A3_PIN = 27;
  static const uint8_t A5_PIN = 16;
  static const uint8_t A6_PIN = 17;
  static const uint8_t A7_PIN = 26;

 public:
  static const uint16_t STRIP = 340;
  static const uint16_t PIXELS = 6 * STRIP;
  // values passed to display controller
  CRGB leds[PIXELS] = {};

  // Gets a display reference to enable calling public methods
  static Display &instance() {
    // Create exactly one Display object with the private contructor
    static Display display;
    // Return the one and only display object
    return display;
  };

  void begin();
  void update();
  void fade(uint8_t i);

 private:
  // Not allowed to make an object of the class (singleton pattern)
  Display(){};
  // Copy constructor is also disabled (singleton pattern)
  Display(Display const &);
  // Assigment operator is also disabled (singleton pattern)
  void operator=(Display const &);
};
#endif