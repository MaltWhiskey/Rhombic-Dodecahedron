#ifndef FLUX_H
#define FLUX_H

#include "Animation.h"
#include "Palettes.h"

class Flux : public Animation {
 private:
  // amount of time this animation keeps running
  Timer timer_duration = 60.0f;
  // Color options
  Palettes palettes;
  CRGBPalette16 palette;
  // Conversion from coordinates to hues
  uint16_t hues[Display::PIXELS];
  // Rotating hue
  uint8_t hue = 0;
  // Brightness used for fading
  uint8_t brightness = 0;

 private:
  // different animation modes
  boolean mode_fade_out = true;

 public:
  void init(float duration, uint16_t mx, uint16_t my, uint16_t mz) {
    task = task_state_t::RUNNING;
    timer_duration = duration;
    for (uint16_t l = 0; l < Display::PIXELS; l++) {
      hues[l] = (mx * Display::coordinates[l].x) +
                (my * Display::coordinates[l].y) +
                (mz * Display::coordinates[l].z);
    }
    brightness = 0;
    palette = palettes.get_next_palette();
  }

  void end() {
    mode_fade_out = true;
    task = task_state_t::ENDING;
  }

  void draw(float dt) {
    if (timer_duration.update()) {
      task = task_state_t::ENDING;
    }
    if (task != task_state_t::ENDING) {
      if (brightness < config.network.mqtt_values.dim)
        brightness++;
      else if (brightness > config.network.mqtt_values.dim)
        brightness--;
    } else {
      if (brightness == 0) {
        task = task_state_t::INACTIVE;
      } else {
        brightness--;
      }
    }
    for (uint16_t x = 0; x < Display::PIXELS; x++) {
      Display::leds[x] = ColorFromPalette(palette, hues[x] + hue, brightness);
    }
    hue++;
  }
};
#endif