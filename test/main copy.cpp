

  void wisp(Wisp& w) {
    w.position++;
    if (w.position > Edges[w.edge].led[1] - Edges[w.edge].led[0]) {
      w.position = 0;
      // Arrived at current node.
      uint8_t current_node = Edges[w.edge].node[1 - w.from];
      // Arrived from previous node
      uint8_t previous_node = Edges[w.edge].node[w.from];
      // Going from current node to future node
      uint8_t future_node = previous_node;
      while (future_node == previous_node) {
        future_node =
            Graph[current_node].nodes[random(0, Graph[current_node].faces)];
      }
      // find an edge going from current to future or visa versa
      for (int i = 0; i < edges; i++) {
        if ((Edges[i].node[0] == current_node) &&
            (Edges[i].node[1] == future_node)) {
          w.edge = i;
          w.from = 0;
          break;
        }
        if ((Edges[i].node[0] == future_node) &&
            (Edges[i].node[1] == current_node)) {
          w.edge = i;
          w.from = 1;
          break;
        }
      }
    }
    // Draw the current led
    uint16_t led = Edges[w.edge].led[w.from];
    if (w.from == 0) {
      led += w.position;
    } else {
      led -= w.position;
    }
    leds[led] = w.color;
    leds[led + 1020] = w.color;
  }
};


Dodecahedron::Wisp wisps[100];
void createwisps();

void createwisps() {
  for (int i = 0; i < 50; i++) {
    wisps[i].edge = random(0, Dodecahedron::edges);
    wisps[i].position = random(0, 42);
    wisps[i].from = random(0, 2);
    wisps[i].color = CRGB(random(255), random(255), random(255));
  };
  wisps[0].color = CRGB(255, 150, 30);
  wisps[1].color = CRGB(255, 150, 30);
}

void loop() {
  EVERY_N_MILLISECONDS(5000) {
    // See status of 5 HUE lights
    uint8_t lights_on = 0;
    lights_on += queryHue(wisps[0].color, "37");
    lights_on += queryHue(wisps[1].color, "38");
    lights_on += queryHue(wisps[2].color, "39");
    lights_on += queryHue(wisps[3].color, "40");
    lights_on += queryHue(wisps[4].color, "41");
    if (lights_on == 0) {
      digitalWrite(RELAY_PIN, LOW);
    } else {
      digitalWrite(RELAY_PIN, HIGH);
    }
  }
  EVERY_N_MILLISECONDS(30) {
    for (int i = 0; i < 5; i++) {
      dodecahedron.wisp(wisps[i]);
    }
  }
  fadeToBlackBy(leds, PIXELS * 6, 1);
  FastLED.show();
}

