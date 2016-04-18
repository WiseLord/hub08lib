#ifndef MATRIX_H
#define MATRIX_H

#include "hub08.h"

#define MATRIX_WIDTH                HUB08_WIDTH
#define MATRIX_HEIGHT               HUB08_HEIGHT
#define MATRIX_FB_SIZE              HUB08_FB_SIZE
#define MATRIX_MAX_BR               HUB08_MAX_BR
#define MATRIX_CHAR_MAX_WIDTH       13
#define MATRIX_STRING_MAX_LENGTH    256

typedef enum {
  ROW_TOP,
  ROW_BOTTOM,
  ROW_BOTH,
  ROW_BOTH_BOTTOM
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

void matrixInit(void);
void matrixSetBr(uint8_t level);

void matrixSetFont(const uint8_t *font, uint8_t color);
void matrixClear(MatrixRow row);
void matrixDrawPixel(uint8_t x, uint8_t y, uint8_t color);
void matrixDrawColumn(uint8_t x, uint16_t data, MatrixRow row);

void matrixShift(uint16_t data);

void matrixLoadScrollString(char *str);
void matrixScroll(MatrixScroll mode, MatrixRow row);

MatrixScroll matrixGetScroll(void);

void matrixSetCol(uint8_t col);
void matrixLoadOutString(char *str);
void matrixShow(MatrixRow row);

#endif // MATRIX_H
