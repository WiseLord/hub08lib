#ifndef MATRIX_H
#define MATRIX_H

#include "hub08.h"

#define MATRIX_WIDTH                HUB08_WIDTH
#define MATRIX_HEIGHT               HUB08_HEIGHT
#define MATRIX_FB_SIZE              HUB08_FB_SIZE
#define MATRIX_MAX_BR               HUB08_MAX_BR
#define MATRIX_CHAR_MAX_WIDTH       11

enum {
  OUT_MODE_FIXED,
  OUT_MODE_SCROLL,

  OUT_MODE_END
};

void matrixInit(void);
void matrixSetBr(uint8_t level);

void matrixSetFont(const uint8_t *font, uint8_t color);
void matrixSetPos(uint8_t x, uint8_t y);
void matrixFill(uint8_t data);
void matrixDrawPixel(uint8_t x, uint8_t y, uint8_t color);
void matrixDrawColumn(uint8_t x, uint16_t data);

void matrixWriteChar(uint8_t code, uint8_t mode);
void matrixWriteString(char *string, uint8_t mode);
void matrixScroll(uint16_t data);

#endif // MATRIX_H
