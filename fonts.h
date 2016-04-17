#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

enum {
  FONT_HEIGHT,
  FONT_SPACE,

  FONT_DATA,
};

typedef struct {
  uint8_t height;
  uint8_t space;
  uint8_t color;
  const uint8_t *data;
} Font;

extern const uint8_t font_matrix_08[];
extern const uint8_t font_matrix_16[];
extern const uint8_t font_dosapp103_16[];

#endif /* FONTS_H */
