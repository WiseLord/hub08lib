#ifndef MATRIX_H
#define MATRIX_H

#include <inttypes.h>

#define MAX7219

#if defined(HUB08)
#include "hub08.h"
#elif defined(MAX7219)
#include "max7219.h"
#endif

#if defined(HUB08)
#define MATRIX_WIDTH                HUB08_WIDTH
#define MATRIX_HEIGHT               HUB08_HEIGHT
#define MATRIX_FB_SIZE              HUB08_FB_SIZE
#elif defined(MAX7219)
#define MATRIX_WIDTH                MAX7219_WIDTH
#define MATRIX_HEIGHT               MAX7219_HEIGHT
#define MATRIX_FB_SIZE              MAX7219_FB_SIZE
#endif
#define MATRIX_CHAR_MAX_WIDTH       13
#define MATRIX_STRING_MAX_LENGTH    256

typedef enum {
  ROW_NONE,
  ROW_TOP,
  ROW_BOTTOM,
  ROW_BOTH,
} MatrixRow;

typedef enum {
  SCROLL_START,
  SCROLL_DRAW,
  SCROLL_TAIL,
  SCROLL_STOP
} MatrixScroll;

typedef enum {
  BUF_SCROLL,
  BUF_FIXED
} MatrixOutbuf;

typedef enum {
  EFFECT_NONE,
  EFFECT_SCROLL_UP,
  EFFECT_SCROLL_DOWN,
} MatrixEffect;

void matrixInit(void);
void matrixSetBr(uint8_t level);

void matrixSetFont(const uint8_t *font, uint8_t color);
void matrixClear(MatrixRow row, MatrixEffect effect);
void matrixDrawPixel(uint8_t x, uint8_t y, uint8_t color);
void matrixDrawColumn(uint8_t x, uint8_t *data, MatrixRow row);

void matrixShift(uint8_t *data);

void matrixLoadScrollString(char *str);
void matrixScroll(MatrixScroll mode, MatrixRow row);

MatrixScroll matrixGetScroll(void);

void matrixSetCol(uint8_t col, uint8_t row);
void matrixLoadOutString(char *str);
void matrixShow(MatrixRow row, MatrixEffect effect);

#endif // MATRIX_H
