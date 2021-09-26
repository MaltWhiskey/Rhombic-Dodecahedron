#include "Animation.h"

#include "FastLED.h"
#include "Trails.h"
#include "Twinkels.h"
/*------------------------------------------------------------------------------
 * ANIMATION STATIC DEFINITIONS
 *----------------------------------------------------------------------------*/
Display &Animation::display = Display::instance();
Noise Animation::noise = Noise();
Timer Animation::s_cTimer = Timer();
uint8_t Animation::s_iAnimations = 0;
Animation *Animation::s_pAnimations[100];
uint16_t Animation::s_iSequence = 0;
/*------------------------------------------------------------------------------
 * ANIMATION GLOBAL DEFINITIONS
 *----------------------------------------------------------------------------*/
Twinkels twinkels;
Trails trails;
/*----------------------------------------------------------------------------*/
void Animation::begin() { display.begin(); }

// Render an animation frame from all active animations
void Animation::animate() {
  // Update the animation timer to determine frame deltatime
  s_cTimer.update();
  // Draw all active animations from the animation pool
  uint8_t active_count = 0;
  for (uint8_t i = 0; i < s_iAnimations; i++) {
    Animation &animation = *s_pAnimations[i];
    if (animation.task != task_state_t::INACTIVE) animation.draw(s_cTimer.dt());
    if (animation.task != task_state_t::INACTIVE) active_count++;
  }
  if (active_count == 0) next();
  // Commit current animation frame to the display
  display.update();
}

// Get fps, if animate has been called t > 0
float Animation::fps() {
  if (s_cTimer.dt() > 0) {
    return 1 / s_cTimer.dt();
  }
  return 0;
}

void SEQ_TWINKEL_WHITE_00(void) {
  twinkels.clear();
  twinkels.init(config.twinkels.timer_duration, true, true);
  twinkels.speed(config.twinkels.timer_interval, config.twinkels.fade_in_speed,
                 config.twinkels.fade_out_speed);
  twinkels.color(CRGB(255, 150, 30));
}

void SEQ_TWINKEL_HUE_00(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.timer_interval, config.twinkels.fade_in_speed,
                 config.twinkels.fade_out_speed);
  twinkels.color(0);
}
void SEQ_TWINKEL_HUE_01(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.timer_interval, config.twinkels.fade_in_speed,
                 config.twinkels.fade_out_speed);
  twinkels.color(1);
}
void SEQ_TWINKEL_HUE_02(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.timer_interval, config.twinkels.fade_in_speed,
                 config.twinkels.fade_out_speed);
  twinkels.color(2);
}
void SEQ_TWINKEL_HUE_03(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.timer_interval, config.twinkels.fade_in_speed,
                 config.twinkels.fade_out_speed);
  twinkels.color(3);
}
void SEQ_TWINKEL_HUE_04(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.timer_interval, config.twinkels.fade_in_speed,
                 config.twinkels.fade_out_speed);
  twinkels.color(4);
}
void SEQ_TWINKEL_HUE_05(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.timer_interval, config.twinkels.fade_in_speed,
                 config.twinkels.fade_out_speed);
  twinkels.color(5);
}
void SEQ_TWINKEL_MULTI_00(void) {
  twinkels.init(config.twinkels.timer_duration);
  twinkels.speed(config.twinkels.timer_interval, config.twinkels.fade_in_speed,
                 config.twinkels.fade_out_speed);
}
void SEQ_TWINKEL_MULTI_01(void) {
  twinkels.init(config.twinkels.timer_duration, false, false, false, true);
  twinkels.speed(config.twinkels.timer_interval,
                 config.twinkels.fade_in_speed / 5,
                 config.twinkels.fade_out_speed / 2);
}
void SEQ_TRAILS_HUE_00(void) {
  trails.init(config.trails.timer_duration, true);
  trails.speed(config.trails.timer_interval, config.trails.fade_out_amount);
}

// Animation sequencer jumptable implementation
void Animation::next() {
  static void (*jump_table[])() =  //
      {&SEQ_TWINKEL_WHITE_00, &SEQ_TWINKEL_HUE_00,   &SEQ_TWINKEL_HUE_01,
       &SEQ_TWINKEL_HUE_02,   &SEQ_TWINKEL_HUE_03,   &SEQ_TWINKEL_HUE_04,
       &SEQ_TWINKEL_HUE_05,   &SEQ_TWINKEL_MULTI_00, &SEQ_TWINKEL_MULTI_01,
       &SEQ_TRAILS_HUE_00};
  if (s_iSequence >= sizeof(jump_table) / sizeof(void *)) {
    s_iSequence = 0;
  }
  jump_table[s_iSequence++]();
}