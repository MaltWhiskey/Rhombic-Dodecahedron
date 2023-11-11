#ifndef TRAILS_H
#define TRAILS_H

#include "Animation.h"
#include "FastLED.h"

class Trails : public Animation {
 private:
  // amount of time this animation keeps running
  Timer timer_duration = 20.0f;
  // amount of seconds before moving
  Timer timer_interval = 0.01f;
  // amount of fading each frame
  uint8_t fade_out_amount = 5;
  // max amount of wisps
  static const uint16_t WISPS = 10;
  Display::Wisp Wisps[WISPS];

 private:
  // different animation modes
  boolean mode_fade_out = true;

 public:
  void init(float duration, boolean fade_out = false) {
    task = task_state_t::RUNNING;
    timer_duration = duration;
    mode_fade_out = fade_out;
    for (int x = 0; x < config.lights.lights; x++) {
      CRGB color = CHSV(config.lights.light[x].hue >> 8,
                        config.lights.light[x].sat, 255);
      Wisps[x].init(x & 1, 0, 0, 0, color);
    }
  }

  void speed(float interval, float out_amount) {
    timer_interval = interval;
    fade_out_amount = out_amount;
  }
  void end() {
    mode_fade_out = true;
    task = task_state_t::ENDING;
  }
  void draw(float dt) {
    Display::fade(fade_out_amount);
    if (timer_duration.update()) {
      task = task_state_t::ENDING;
    }
    boolean fade_active = false;
    if (task == task_state_t::ENDING) {
      if (mode_fade_out) {
        fade_active = true;
      } else {
        task = task_state_t::INACTIVE;
      }
    }
    // only move and draw once per interval
    if (timer_interval.update()) {
      uint8_t blacks = 0;
      for (int x = 0; x < config.lights.lights; x++) {
        CRGB color = Wisps[x].color;
        if (fade_active) {
          color = Display::leds[Wisps[x].led()];
        }
        if ((color.red | color.green | color.blue) == 0) {
          blacks++;
        }
        Wisps[x].move();
        Display::leds[Wisps[x].led()] = color;
      }
      if ((task == task_state_t::ENDING) & (blacks == config.lights.lights)) {
        task = task_state_t::INACTIVE;
      }
    }
  }
};
#endif