#ifndef TWINKELS_H
#define TWINKELS_H

#include "Animation.h"
#include "FastLED.h"
class Twinkels : public Animation {
 private:
  // amount of time this animation keeps running
  Timer timer_duration = 20.0f;
  // amount of seconds it takes to fade a pixel to max
  float fade_in_speed = 1.0f;
  // amount of seconds it takes to fade a pixel to min
  float fade_out_speed = 3.0f;
  // source buffer for origional value
  CRGB buffer[Display::PIXELS] = {};
  // amount of time the pixel is activated
  float time[Display::PIXELS] = {};

 private:
  // different animation modes
  boolean mode_single_color = true;
  boolean mode_fade_out = true;
  boolean mode_custom_color = true;
  boolean mode_random_color = true;
  boolean mode_mqtt_color = true;
  // light for single_color mode
  uint8_t hue_light = 0;
  // color for single_color mode, custom_color mode
  CRGB custom_color = CRGB(0, 0, 0);

 public:
  void init(float duration, boolean single = false, boolean custom = false,
            boolean fade_out = false, boolean rnd = false,
            boolean mqtt = false) {
    task = task_state_t::RUNNING;
    timer_duration = duration;
    mode_single_color = single;
    mode_fade_out = fade_out;
    mode_custom_color = custom;
    mode_mqtt_color = mqtt;
    mode_random_color = rnd;
  }
  void speed(float in_speed, float out_speed) {
    fade_in_speed = in_speed;
    fade_out_speed = out_speed;
  }
  void color(CRGB c) { custom_color = c; }
  void color(uint8_t light) { hue_light = light; }
  void clear() {
    for (uint16_t x = 0; x < Display::PIXELS; x++) {
      time[x] = 0;
      buffer[x] = CRGB(0, 0, 0);
    }
  }

  void end() {
    mode_fade_out = true;
    task = task_state_t::ENDING;
  }

  void draw(float dt) {
    bool pixels_active = false;
    for (uint16_t x = 0; x < Display::PIXELS; x++) {
      if (buffer[x].r | buffer[x].g | buffer[x].b) {
        if (time[x] < fade_in_speed) {
          float t = time[x] / fade_in_speed;
          CRGB c = buffer[x];
          nscale8x3(c.r, c.g, c.b, (uint8_t)(255 * t));
          Display::leds[x] = c;
          time[x] += dt;
        } else if (time[x] < fade_in_speed + fade_out_speed) {
          float t = (time[x] - fade_in_speed) / fade_out_speed;
          CRGB c = buffer[x];
          nscale8x3(c.r, c.g, c.b, (uint8_t)(255 * (1 - t)));
          Display::leds[x] = c;
          time[x] += dt;
        } else {
          time[x] = 0;
          buffer[x] = CRGB(0, 0, 0);
          Display::leds[x] = CRGB(0, 0, 0);
        }
        pixels_active = true;
      }
    }
    // if this animation is finished start end mode
    if (timer_duration.update()) {
      task = task_state_t::ENDING;
    }
    if (task != task_state_t::ENDING) {
      const uint8_t div = config.twinkels.dim_divisor;
      const uint8_t dim = config.network.mqtt_values.dim;
      uint8_t counter = ceil(dim / (float)div);
      uint8_t chance = (counter * div) - dim;
      // Consider adding a new random twinkle
      for (int i = 0; i < counter; i++) {
        if ((i == counter - 1) && (random(0, div) < chance)) {
          break;
        }
        uint16_t x = random(0, Display::PIXELS);
        if (time[x] == 0) {
          if (mode_single_color) {
            if (mode_custom_color)
              buffer[x] = custom_color;
            else if (mode_mqtt_color)
              buffer[x] = config.network.mqtt_values.color;
            else
              buffer[x] = CHSV(config.lights.light[hue_light].hue >> 8,
                               config.lights.light[hue_light].sat, 255);

          } else if (mode_random_color) {
            buffer[x] = CRGB(random(0, 255), random(0, 255), random(0, 255));
          } else {
            uint8_t light = random(0, config.lights.lights);
            buffer[x] = CHSV(config.lights.light[light].hue >> 8,
                             config.lights.light[light].sat, 255);
          }
        }
      }
    } else if (mode_fade_out) {
      // deactivate when all pixels are black
      if (!pixels_active) {
        task = task_state_t::INACTIVE;
      }
    } else {
      // deactivate immediately
      task = task_state_t::INACTIVE;
    }
  }
};
#endif