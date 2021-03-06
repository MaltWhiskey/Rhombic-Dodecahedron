#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "Animation.h"

class Test : public Animation {
 private:
  // amount of time this animation keeps running
  Timer timer_duration = 60.0f;

 private:
  // different animation modes
  boolean mode_fade_out = true;

 public:
  void init(float duration) {
    task = task_state_t::RUNNING;
    timer_duration = duration;
  }
  void end() {
    mode_fade_out = true;
    task = task_state_t::ENDING;
  }
  void draw(float dt) {
    if (timer_duration.update()) {
      task = task_state_t::INACTIVE;
    }
    delay(10);
    Display::leds[0].red++;
  }
};
#endif