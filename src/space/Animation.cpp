#include "Animation.h"

#include "FastLED.h"
#include "Flux.h"
#include "Trails.h"
#include "Twinkels.h"
/*------------------------------------------------------------------------------
 * ANIMATION STATIC DEFINITIONS
 *----------------------------------------------------------------------------*/
Noise Animation::noise = Noise();
Timer Animation::animation_timer = Timer();
uint16_t Animation::animation_sequence = 0;
/*------------------------------------------------------------------------------
 * GLOBAL DEFINITIONS
 *----------------------------------------------------------------------------*/
Twinkels twinkels;
Trails trails;
Flux flux;
Animation *Animations[] = {&twinkels, &trails, &flux};
/*----------------------------------------------------------------------------*/
void Animation::begin() { Display::begin(); }

// Render an animation frame from all active animations
void Animation::animate() {
  // Update the animation timer to determine frame deltatime
  animation_timer.update();
  // Draw all active animations from the animation pool
  uint8_t active_count = 0;
  for (uint8_t i = 0; i < sizeof(Animations) / sizeof(Animation *); i++) {
    Animation &animation = *Animations[i];
    if (animation.task != task_state_t::INACTIVE) {
      animation.draw(animation_timer.dt());
    }
    if (animation.task != task_state_t::INACTIVE) {
      active_count++;
    }
  }
  if (active_count == 0) next();
  // Commit current animation frame to the display
  Display::update();
}

// Get fps, if animate has been called t > 0
float Animation::fps() {
  if (animation_timer.dt() > 0) {
    return 1 / animation_timer.dt();
  }
  return 0;
}

void SEQ_TWINKEL_WHITE_00(void) {
  twinkels.clear();
  twinkels.init(config.twinkels.timer_duration, true, true);
  twinkels.speed(config.twinkels.fade_in_speed, config.twinkels.fade_out_speed);
  twinkels.color(CRGB(255, 150, 30));
}

void SEQ_TWINKEL_HUE_00(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.fade_in_speed, config.twinkels.fade_out_speed);
  twinkels.color(0);
}
void SEQ_TWINKEL_HUE_01(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.fade_in_speed, config.twinkels.fade_out_speed);
  twinkels.color(1);
}
void SEQ_TWINKEL_HUE_02(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.fade_in_speed, config.twinkels.fade_out_speed);
  twinkels.color(2);
}
void SEQ_TWINKEL_HUE_03(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.fade_in_speed, config.twinkels.fade_out_speed);
  twinkels.color(3);
}
void SEQ_TWINKEL_HUE_04(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.fade_in_speed, config.twinkels.fade_out_speed);
  twinkels.color(4);
}
void SEQ_TWINKEL_HUE_05(void) {
  twinkels.init(config.twinkels.timer_duration, true);
  twinkels.speed(config.twinkels.fade_in_speed, config.twinkels.fade_out_speed);
  twinkels.color(5);
}
void SEQ_TWINKEL_MULTI_00(void) {
  twinkels.init(config.twinkels.timer_duration);
  twinkels.speed(config.twinkels.fade_in_speed, config.twinkels.fade_out_speed);
}
void SEQ_TWINKEL_MULTI_01(void) {
  twinkels.init(config.twinkels.timer_duration, false, false, false, true);
  twinkels.speed(config.twinkels.fade_in_speed / 2,
                 config.twinkels.fade_out_speed / 2);
}
void SEQ_TRAILS_HUE_00(void) {
  trails.init(config.trails.timer_duration, true);
  trails.speed(config.trails.timer_interval, config.trails.fade_out_amount);
}

void SEQ_FLUX_00(void) {
  flux.init(config.flux.timer_duration, config.flux.x_movement,
            config.flux.y_movement, config.flux.z_movement);
}

void SEQ_TWINKEL_MQTT_00(void) {
  twinkels.init(100, true, false, false, false, true);
  twinkels.speed(config.twinkels.fade_in_speed, config.twinkels.fade_out_speed);
}

// Animation sequencer jumptable implementation
void Animation::next() {
  static void (*jump_table[])() =  //
      {&SEQ_TWINKEL_WHITE_00, &SEQ_TWINKEL_MQTT_00, &SEQ_TWINKEL_HUE_00,
       &SEQ_TWINKEL_HUE_01,   &SEQ_TWINKEL_HUE_02,  &SEQ_TWINKEL_HUE_03,
       &SEQ_TWINKEL_HUE_04,   &SEQ_TWINKEL_HUE_05,  &SEQ_TWINKEL_MULTI_00,
       &SEQ_TWINKEL_MULTI_01, &SEQ_TRAILS_HUE_00,   &SEQ_FLUX_00};
  if (animation_sequence >= sizeof(jump_table) / sizeof(void *)) {
    animation_sequence = 0;
  }
  jump_table[animation_sequence++]();
}