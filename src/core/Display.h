#ifndef DISPLAY_H
#define DISPLAY_H
#define FASTLED_ESP32_I2S true
#include <Arduino.h>
#include <FastLED.h>

#include "power/Math3D.h"

class Display {
 public:
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
  static const uint8_t DODECAHEDRA = 2;
  static const uint8_t EDGES = 24;
  static const uint8_t VERTICES = 14;
  static const uint16_t STRIP = 340;
  static const uint16_t PIXELS = 6 * STRIP;

 private:
  static const uint8_t A1_PIN = 32;
  static const uint8_t A2_PIN = 12;
  static const uint8_t A3_PIN = 27;
  static const uint8_t A5_PIN = 16;
  static const uint8_t A6_PIN = 17;
  static const uint8_t A7_PIN = 26;

 private:
  // An edge had 2 nodes and their location in the leds array
  struct Edge {
    uint8_t node[2];
    uint16_t led[2];
  };
  // Each node is connected to 3 or 4 other nodes
  struct Connection {
    uint8_t faces;
    uint8_t nodes[4];
  };

 private:
  // Leds on the 4 faced vertices are on 2 edges. The 3 faced vertices
  // have no leds on them.
  static Edge Edges[DODECAHEDRA][EDGES];
  // Connection graph, specifies the paths that can be taken from each node.
  static Connection Graph[VERTICES];
  // Cartesian coordinate mapping
  static Vector3 Coordinates[VERTICES];

 public:
  static CRGB leds[PIXELS];
  static Vector3 map[PIXELS];

 public:
  static void begin();
  static void update();
  static void fade(uint8_t i);
  static void remap(uint8_t solid, Vector3 v);

 public:
  // Wisp is a position on an edge on a solid, moving in a direction
  class Wisp {
   private:
    // On which Edges[solid][EDGES]
    uint8_t solid = 0;
    uint8_t edge = 0;
    // 0 = node[0] -> node[1]
    // 1 = node[1] -> node[0]
    uint8_t direction = 0;
    // Relative position on the edge, depends on direction
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