#include "display.h"
/******************************************************************************
 *          How to order and travel around a rhombic dodecahedron             *
 * ****************************************************************************
 *                            A     A     A     A
 *                            |     |     |     |
 *                            B     C     D     B
 *                           /|\   /|\   /|\   /|
 *                         G/ | \E/ | \F/ | \G/ |
 *                         | /H\ | /I\ | /J\ | /H
 *                         |/   \|/   \|/   \|/
 *                         M     K     L     M
 *                         |     |     |     |
 *                         N     N     N     N
 *****************************************************************************/
Display::Edge Display::Edges[DODECAHEDRA][EDGES] = {
    {{{A, B}, {0, 42}},    {{B, H}, {42, 84}},          // DIN1
     {{H, K}, {85, 127}},  {{K, I}, {127, 169}},        //
     {{E, K}, {170, 212}}, {{K, N}, {212, 254}},        //
     {{N, L}, {255, 297}}, {{L, F}, {297, 339}},        //
     {{A, C}, {340, 382}}, {{C, I}, {382, 424}},        // DIN2
     {{I, L}, {425, 467}}, {{L, J}, {467, 509}},        //
     {{N, M}, {510, 552}}, {{M, G}, {552, 594}},        //
     {{G, D}, {595, 637}}, {{D, F}, {637, 679}},        //
     {{A, D}, {680, 722}}, {{D, J}, {722, 764}},        // DIN3
     {{J, M}, {765, 807}}, {{M, H}, {807, 849}},        //
     {{G, B}, {850, 892}}, {{B, E}, {892, 934}},        //
     {{E, C}, {935, 977}}, {{C, F}, {977, 1019}}},      //
    {{{A, B}, {1020, 1062}}, {{B, H}, {1062, 1104}},    // DIN4
     {{H, K}, {1105, 1147}}, {{K, I}, {1147, 1189}},    //
     {{E, K}, {1190, 1232}}, {{K, N}, {1232, 1274}},    //
     {{N, L}, {1275, 1317}}, {{L, F}, {1317, 1359}},    //
     {{A, C}, {1360, 1402}}, {{C, I}, {1402, 1444}},    // DIN5
     {{I, L}, {1445, 1487}}, {{L, J}, {1487, 1529}},    //
     {{N, M}, {1530, 1572}}, {{M, G}, {1572, 1614}},    //
     {{G, D}, {1615, 1657}}, {{D, F}, {1657, 1699}},    //
     {{A, D}, {1700, 1742}}, {{D, J}, {1742, 1784}},    // DIN6
     {{J, M}, {1785, 1827}}, {{M, H}, {1827, 1869}},    //
     {{G, B}, {1870, 1912}}, {{B, E}, {1912, 1954}},    //
     {{E, C}, {1955, 1997}}, {{C, F}, {1997, 2039}}}};  //

Display::Connection Display::Graph[VERTICES] = {{3, {B, C, D}},     // A
                                                {4, {A, G, H, E}},  // B
                                                {4, {A, E, I, F}},  // C
                                                {4, {A, F, J, G}},  // D
                                                {3, {B, K, C}},     // E
                                                {3, {C, L, D}},     // F
                                                {3, {D, M, B}},     // G
                                                {3, {M, B, K}},     // H
                                                {3, {K, C, L}},     // I
                                                {3, {L, D, M}},     // J
                                                {4, {H, E, I, N}},  // K
                                                {4, {I, F, J, N}},  // L
                                                {4, {J, G, H, N}},  // M
                                                {3, {K, L, M}}};    // N

// Cartesian coordinates with node A on top and lid to the front
Vector3 Display::Coordinates[VERTICES] = {
    Vector3(0, sqrt(3) / 2, 0),                         // A
    Vector3(0, sqrt(3) / 3, sqrt(6) / 3),               // B
    Vector3(-sqrt(2) / 2, sqrt(3) / 3, -sqrt(6) / 6),   // C
    Vector3(sqrt(2) / 2, sqrt(3) / 3, -sqrt(6) / 6),    // D
    Vector3(-sqrt(2) / 2, sqrt(3) / 6, sqrt(6) / 6),    // E
    Vector3(0, sqrt(3) / 6, -sqrt(6) / 3),              // F
    Vector3(sqrt(2) / 2, sqrt(3) / 6, sqrt(6) / 6),     // G
    Vector3(0, -sqrt(3) / 6, sqrt(6) / 3),              // H
    Vector3(-sqrt(2) / 2, -sqrt(3) / 6, -sqrt(6) / 6),  // I
    Vector3(sqrt(2) / 2, -sqrt(3) / 6, -sqrt(6) / 6),   // J
    Vector3(-sqrt(2) / 2, -sqrt(3) / 3, sqrt(6) / 6),   // K
    Vector3(0, -sqrt(3) / 3, -sqrt(6) / 3),             // L
    Vector3(sqrt(2) / 2, -sqrt(3) / 3, sqrt(6) / 6),    // M
    Vector3(0, -sqrt(3) / 2, 0),                        // N
};

// Static memory for all pixels
CRGB Display::leds[PIXELS];
// Static coordinates for all pixels
Vector3 Display::map[PIXELS];

void Display::begin() {
  FastLED.addLeds<WS2813, A1_PIN, GRB>(leds, 0 * STRIP, STRIP);
  FastLED.addLeds<WS2813, A2_PIN, GRB>(leds, 1 * STRIP, STRIP);
  FastLED.addLeds<WS2813, A3_PIN, GRB>(leds, 2 * STRIP, STRIP);

  FastLED.addLeds<WS2813, A7_PIN, GRB>(leds, 3 * STRIP, STRIP);
  FastLED.addLeds<WS2813, A5_PIN, GRB>(leds, 4 * STRIP, STRIP);
  FastLED.addLeds<WS2813, A6_PIN, GRB>(leds, 5 * STRIP, STRIP);
  FastLED.setBrightness(255);
  FastLED.setDither(DISABLE_DITHER);
}
void Display::update() { FastLED.show(); }
void Display::fade(uint8_t i) { fadeToBlackBy(leds, PIXELS, i); }

void Display::remap(uint8_t solid, Vector3 r) {
  Vector3 offset = Vector3(1, 1, 1);
  for (uint8_t edge = 0; edge < Display::EDGES; edge++) {
    uint16_t lower = Edges[solid][edge].led[0];
    uint16_t upper = Edges[solid][edge].led[1];
    Vector3 v0 = (Coordinates[Edges[solid][edge].node[0]] + offset) / 2;
    Vector3 v1 = (Coordinates[Edges[solid][edge].node[1]] + offset) / 2;
    Vector3 delta = (v1 - v0) / (upper - lower);
    v0 += (offset * solid);
    // include last led since first and last led are also on other edges
    for (uint16_t led = lower; led <= upper; led++) {
      map[led] = v0;
      v0 += delta;
    }
  }
}

void Display::Wisp::init(uint8_t solid_, uint8_t edge_, uint8_t direction_,
                         uint8_t position_, CRGB color_) {
  solid = solid_ & 1;
  edge = edge_ % EDGES;
  direction = direction_ & 1;
  position = position_;
  color = color_;
}

uint16_t Display::Wisp::led() {
  uint16_t led = Edges[solid][edge].led[direction];
  if (direction == 0) {
    led += position;
  } else {
    led -= position;
  }
  return led;
}

void Display::Wisp::move() {
  if (++position > (Edges[solid][edge].led[1] - Edges[solid][edge].led[0])) {
    // Arrived at new node.
    uint8_t new_node = Edges[solid][edge].node[1 - direction];
    // start position on new node
    position = 0;
    // Arrived from old node
    uint8_t old_node = Edges[solid][edge].node[direction];
    // Going to random next node but not back to old node
    uint8_t next_node = old_node;
    while (next_node == old_node)
      next_node = Graph[new_node].nodes[random(0, Graph[new_node].faces)];
    // find an edge going from new node to next node or visa versa
    for (int i = 0; i < EDGES; i++) {
      if ((Edges[solid][i].node[0] == new_node) &&
          (Edges[solid][i].node[1] == next_node)) {
        edge = i;
        direction = 0;
        break;
      }
      if ((Edges[solid][i].node[0] == next_node) &&
          (Edges[solid][i].node[1] == new_node)) {
        edge = i;
        direction = 1;
        break;
      }
    }
  }
}
