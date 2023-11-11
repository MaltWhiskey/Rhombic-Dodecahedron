#ifndef PALETTES_H
#define PALETTES_H
#include <FastLED.h>
#include <stdint.h>
/*----------------------------------------------------------------------------------------------
 * Palettes CLASS
 *--------------------------------------------------------------------------------------------*/
class Palettes {
 private:
  uint8_t m_palette_index = 0;
  uint8_t m_palette_count = 0;

 public:
  Palettes();
  CRGBPalette16 get_next_palette();
};
#endif