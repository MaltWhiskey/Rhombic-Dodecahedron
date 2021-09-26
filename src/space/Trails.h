#ifndef TRAILS_H
#define TRAILS_H

#include "Animation.h"
#include "FastLED.h"
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
static const uint8_t DODECAHEDRA = 2;
static const uint8_t EDGES = 24;
static const uint8_t VERTICES = 14;

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

// Leds on the 4 faced vertices are on 2 edges. The 3 faced vertices
// have no leds on them. Edges do not have to be equal length. Calibrate
// all edges visually.
Edge Edges[DODECAHEDRA][EDGES] = {
    {{{'A', 'B'}, {0, 42}},    {{'B', 'H'}, {42, 84}},          // DIN1
     {{'H', 'K'}, {85, 127}},  {{'K', 'I'}, {127, 169}},        //
     {{'E', 'K'}, {170, 212}}, {{'K', 'N'}, {212, 254}},        //
     {{'N', 'L'}, {255, 297}}, {{'L', 'F'}, {297, 339}},        //
     {{'A', 'C'}, {340, 382}}, {{'C', 'I'}, {382, 424}},        // DIN2
     {{'I', 'L'}, {425, 467}}, {{'L', 'J'}, {467, 509}},        //
     {{'N', 'M'}, {510, 552}}, {{'M', 'G'}, {552, 594}},        //
     {{'G', 'D'}, {595, 637}}, {{'D', 'F'}, {637, 679}},        //
     {{'A', 'D'}, {680, 722}}, {{'D', 'J'}, {722, 764}},        // DIN3
     {{'J', 'M'}, {765, 807}}, {{'M', 'H'}, {807, 849}},        //
     {{'G', 'B'}, {850, 892}}, {{'B', 'E'}, {892, 934}},        //
     {{'E', 'C'}, {935, 977}}, {{'C', 'F'}, {977, 1019}}},      //
    {{{'A', 'B'}, {1020, 1062}}, {{'B', 'H'}, {1062, 1104}},    // DIN4
     {{'H', 'K'}, {1105, 1147}}, {{'K', 'I'}, {1147, 1189}},    //
     {{'E', 'K'}, {1190, 1232}}, {{'K', 'N'}, {1232, 1274}},    //
     {{'N', 'L'}, {1275, 1317}}, {{'L', 'F'}, {1317, 1359}},    //
     {{'A', 'C'}, {1360, 1402}}, {{'C', 'I'}, {1402, 1444}},    // DIN5
     {{'I', 'L'}, {1445, 1487}}, {{'L', 'J'}, {1487, 1529}},    //
     {{'N', 'M'}, {1530, 1572}}, {{'M', 'G'}, {1572, 1614}},    //
     {{'G', 'D'}, {1615, 1657}}, {{'D', 'F'}, {1657, 1699}},    //
     {{'A', 'D'}, {1700, 1742}}, {{'D', 'J'}, {1742, 1784}},    // DIN6
     {{'J', 'M'}, {1785, 1827}}, {{'M', 'H'}, {1827, 1869}},    //
     {{'G', 'B'}, {1870, 1912}}, {{'B', 'E'}, {1912, 1954}},    //
     {{'E', 'C'}, {1955, 1997}}, {{'C', 'F'}, {1997, 2039}}}};  //

// Connection graph, specifies the paths that can be taken from each node.
Connection Graph[VERTICES] = {{3, {'B', 'C', 'D'}},       // A
                              {4, {'A', 'G', 'H', 'E'}},  // B
                              {4, {'A', 'E', 'I', 'F'}},  // C
                              {4, {'A', 'F', 'J', 'G'}},  // D
                              {3, {'B', 'K', 'C'}},       // E
                              {3, {'C', 'L', 'D'}},       // F
                              {3, {'D', 'M', 'B'}},       // G
                              {3, {'M', 'B', 'K'}},       // H
                              {3, {'K', 'C', 'L'}},       // I
                              {3, {'L', 'D', 'M'}},       // J
                              {4, {'H', 'E', 'I', 'N'}},  // K
                              {4, {'I', 'F', 'J', 'N'}},  // L
                              {4, {'J', 'G', 'H', 'N'}},  // M
                              {3, {'K', 'L', 'M'}}};      // N

class Wisp {
 private:
  // On which Edges[solid][EDGES]
  uint8_t solid;
  // On which Edges[solid][EDGES]
  uint8_t edge;
  // 0 = node[0] -> node[1]
  // 1 = node[1] -> node[0]
  uint8_t direction;
  // Relative position on the edge, depends on direction
  uint8_t position;
  // current color, can be changed
  CRGB color;

 public:
  // previous color;
  CRGB old_color;

 public:
  void move() {
    old_color = Display::instance().leds[led_position()];
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

  uint16_t led_position() {
    uint16_t led = Edges[solid][edge].led[direction];
    if (direction == 0) {
      led += position;
    } else {
      led -= position;
    }
    return led;
  }

  void draw(boolean fade = false) {
    uint16_t led = led_position();
    if (fade) {
      Display::instance().leds[led] = old_color;
    } else {
      Display::instance().leds[led] = color;
    }
  }

  void init(uint8_t s, uint8_t e, uint8_t p, uint8_t d, CRGB c) {
    solid = s & 1;
    edge = e % EDGES;
    position = p;
    direction = d & 1;
    color = c;
  }
};

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
  Wisp Wisps[WISPS] = {};

 private:
  // different animation modes
  boolean mode_fade_out = true;

 public:
  Trails() {
    for (int i = 0; i < EDGES; i++) {
      for (int d = 0; d < DODECAHEDRA; d++) {
        Edges[d][i].node[0] -= 'A';
        Edges[d][i].node[1] -= 'A';
      }
    }
    for (int i = 0; i < VERTICES; i++)
      for (int j = 0; j < Graph[i].faces; j++) {
        Graph[i].nodes[j] -= 'A';
      }
  }

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

  void draw(float dt) {
    // if this animation is finished start end mode
    if (timer_duration.update()) {
      task = task_state_t::ENDING;
    }
    display.fade(fade_out_amount);
    // only move and draw once per interval
    boolean fade_active = false;
    if (task == task_state_t::ENDING) {
      if (mode_fade_out) {
        fade_active = true;
      } else {
        task = task_state_t::INACTIVE;
      }
    }
    if (timer_interval.update()) {
      uint8_t blacks = 0;
      for (int x = 0; x < config.lights.lights; x++) {
        Wisps[x].move();
        Wisps[x].draw(fade_active);
        if ((Wisps[x].old_color.red | Wisps[x].old_color.green |
             Wisps[x].old_color.blue) == 0)
          blacks++;
      }
      if ((task == task_state_t::ENDING) & (blacks == config.lights.lights)) {
        task = task_state_t::INACTIVE;
      }
    }
  }
};
#endif