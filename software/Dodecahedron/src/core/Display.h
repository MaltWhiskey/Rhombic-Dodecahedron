#ifndef DISPLAY_H
#define DISPLAY_H
#define FASTLED_ESP32_I2S true
#include <Arduino.h>
#include <FastLED.h>

#include "power/Math3D.h"

class Display {
 public:
  static const uint8_t DODECAHEDRA = 2;
  static const uint8_t EDGES = 24;
  static const uint8_t VERTICES = 14;
  static const uint16_t STRIP = 340;
  static const uint16_t PIXELS = 6 * STRIP;
  static CRGB leds[PIXELS];
  static Vector3 coordinates[PIXELS];

 private:
  static const uint8_t A = 0;
  static const uint8_t B = 1;
  static const uint8_t C = 2;
  static const uint8_t D = 3;
  static const uint8_t E = 4;
  static const uint8_t F = 5;
  static const uint8_t G = 6;
  static const uint8_t H = 7;
  static const uint8_t I = 8;
  static const uint8_t J = 9;
  static const uint8_t K = 10;
  static const uint8_t L = 11;
  static const uint8_t M = 12;
  static const uint8_t N = 13;

  static const uint8_t DIN1 = 32;
  static const uint8_t DIN2 = 12;
  static const uint8_t DIN3 = 27;
  static const uint8_t DIN4 = 17;
  static const uint8_t DIN5 = 16;
  static const uint8_t DIN6 = 26;

  // An edge had 2 nodes mapping to 2 leds
  struct Edge {
    uint8_t node[2];
    uint16_t led[2];
  };
  // A node is connected to 3 or 4 other nodes
  struct Path {
    uint8_t faces;
    uint8_t node[4];
  };

  // Led edge mapping on each dodecahedra
  static Edge Edges[DODECAHEDRA][EDGES];
  // Paths that can be taken from each node
  static Path Paths[VERTICES];
  // Cartesian coordinates of each node
  static Vector3 Nodes[VERTICES];

 public:
  static void begin();
  static void update();
  static void fade(uint8_t i);
  static void calibrate(uint8_t solid, float a);

 public:
  // Wisp is a position on an edge on a solid, moving in a direction
  class Wisp {
   private:
    // Edge = Edges[solid][edge]
    uint8_t solid = 0;
    uint8_t edge = 0;
    // direction 0 = node[0] -> node[1]
    // direction 1 = node[1] -> node[0]
    uint8_t direction = 0;
    // Relative position on the edge with respect to direction
    uint8_t position = 0;

   public:
    // current color, can be changed
    CRGB color = CRGB(0, 0, 0);

   public:
    void init(uint8_t solid_, uint8_t edge_, uint8_t direction_,
              uint8_t position_, CRGB color_);
    void move();
    uint16_t led();
  };
};
#endif