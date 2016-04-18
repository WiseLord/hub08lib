#ifndef MATRIX_H
#define MATRIX_H

#include "hub08.h"

#define MATRIX_WIDTH                HUB08_WIDTH
#define MATRIX_HEIGHT               HUB08_HEIGHT
#define MATRIX_FB_SIZE              HUB08_FB_SIZE
#define MATRIX_MAX_BR               HUB08_MAX_BR
#define MATRIX_CHAR_MAX_WIDTH       16
#define MATRIX_STRING_MAX_LENGTH    256

enum {
  OUT_MODE_FIXED,
  OUT_MODE_SCROLL,

  OUT_MODE_END
};

enum {
  MATRIX_TOP,
  MATRIX_BOTTOM,
  MATRIX_BOTH,
  MATRIX_BOTH_BOTTOM
};

enum {
  SCROLL_START,
  SCROLL_DRAW,
  SCROLL_TAIL,
  SCROLL_STOP
};


void matrixInit(void);
void matrixSetBr(uint8_t level);

void matrixSetFont(const uint8_t *font, uint8_t color);
void matrixClear(uint8_t rows);
void matrixDrawPixel(uint8_t x, uint8_t y, uint8_t color);
void matrixDrawColumn(uint8_t x, uint16_t data);

void matrixShift(uint16_t data);

void matrixLoadString(char *str);
void matrixScroll(uint8_t mode, uint8_t rows);

#endif // MATRIX_H
