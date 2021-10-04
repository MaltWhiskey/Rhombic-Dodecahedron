#ifndef FLOW_H
#define FLOW_H

#include "Animation.h"
DEFINE_GRADIENT_PALETTE(lava_gp){
    0,   0,   0,   0,    //
    23,  18,  0,   0,    //
    48,  113, 0,   0,    //
    54,  142, 3,   1,    //
    60,  175, 17,  1,    //
    73,  213, 44,  2,    //
    87,  255, 82,  4,    //
    94,  255, 115, 4,    //
    101, 255, 156, 4,    //
    109, 255, 203, 4,    //
    117, 255, 255, 4,    //
    122, 255, 255, 71,   //
    127, 255, 255, 255,  //
    132, 255, 255, 71,   //
    137, 255, 255, 4,    //
    145, 255, 203, 4,    //
    153, 255, 156, 4,    //
    160, 255, 115, 4,    //
    167, 255, 82,  4,    //
    181, 213, 44,  2,    //
    188, 175, 17,  1,    //
    194, 142, 3,   1,    //
    200, 113, 0,   0,    //
    225, 18,  0,   0,    //
    255, 0,   0,   0,    //
};

DEFINE_GRADIENT_PALETTE(Sunset_Real_gp){
    0,   120, 0,   0,    //
    11,  179, 22,  0,    //
    25,  255, 104, 0,    //
    42,  167, 22,  18,   //
    62,  100, 0,   103,  //
    99,  16,  0,   130,  //
    127, 0,   0,   160,  //
    155, 16,  0,   130,  //
    192, 100, 0,   103,  //
    212, 167, 22,  18,   //
    229, 255, 104, 0,    //
    244, 179, 22,  0,    //
    255, 120, 0,   0,    //
};

class Flow : public Animation {
 private:
  // amount of time this animation keeps running
  Timer timer_duration = 60.0f;
  // Color options
  CRGBPalette16 palette = lava_gp;
  uint8_t hue;

 public:
  void init(float duration) {
    task = task_state_t::RUNNING;
    timer_duration = duration;
    Display::remap(0, Vector3());
    Display::remap(1, Vector3());
  }

  void draw(float dt) {
    if (timer_duration.update()) {
      task = task_state_t::INACTIVE;
    }
    for (uint16_t x = 0; x < Display::PIXELS; x++) {
      uint16_t index = (200 * Display::map[x].x) + (300 * Display::map[x].y) +
                       (500 * Display::map[x].z);
      Display::leds[x] = ColorFromPalette(palette, index + hue, 255);
    }
    hue++;
  }
};
#endif