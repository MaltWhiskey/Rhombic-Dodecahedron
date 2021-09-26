#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <stdint.h>

#include "ArduinoJson.h"
/*-----------------------------------------------------------------------------
 * Evil global Config parameters
 *
 * Animation init or draw routines need to apply config parameters to
 * dynamically set runtime parameters. Init only gets called when an animation
 * starts or restarts draw gets called every animation frame so choose wisely
 * where to apply.
 *---------------------------------------------------------------------------*/
struct Config {
  struct {
    char ssid[32] = "-^..^-";
    char password[64] = "qazwsxedc";
    char hostname[64] = "Infinity";
    char broker[64] = "192.168.178.24";
    char device[64] = "Homey";
    char topic[64] = "homie/homey/dodecahedron/onoff";
    char huebridge[64] = "192.168.178.22";
    char hueapi[128] = "/api/Mn0hooRxdFRet5wIpdNaNDY2y8tJHtRV6PWT2IZK/lights/";
  } network;
  struct {
    float timer_duration = 15.0f;
    float timer_interval = 0.001f;
    float fade_in_speed = 1.0f;
    float fade_out_speed = 2.0f;
  } twinkels;
  struct {
    float timer_duration = 20.0f;
    float timer_interval = 0.022f;
    uint8_t fade_out_amount = 5;
  } trails;
  struct {
    const uint8_t lights = 6;
    struct {
      char name[4];
      boolean on;
      uint16_t hue;
      uint8_t sat;
      uint8_t bri;
    } light[6] = {{"37", false, 0, 0, 0}, {"39", false, 0, 0, 0},
                  {"38", false, 0, 0, 0}, {"40", false, 0, 0, 0},
                  {"41", false, 0, 0, 0}, {"4", false, 0, 0, 0}};
  } lights;
};
// All cpp files that include this link to a single config struct
extern struct Config config;
#endif