#include "matrix.h"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

Font font;

static char scrollStrBuf[MATRIX_STRING_MAX_LENGTH];
static char *scrollStrPtr = scrollStrBuf;
static uint16_t scrollCharBuf[MATRIX_CHAR_MAX_WIDTH];

static volatile MatrixScroll scroll = SCROLL_STOP;
static MatrixRow scrollRow = ROW_TOP;

static uint16_t outBuf[HUB08_WIDTH];
static uint8_t outCol = 0;

static uint8_t matrixReadChar(uint8_t code, MatrixOutbuf buf)
{
  uint8_t i;
  int j;

  uint8_t pgmData = 0;
  uint16_t oft = 0;   // Current symbol offset in array
  uint8_t swd = 0;    // Current symbol width

  uint16_t data = 0;

  code -= ' ';

  // Calculate symbol offset in array and find symbol width
  for (i = 0; i < code; i++) {
    swd = pgm_read_byte(font.data + i);
    oft += swd;
  }
  swd = pgm_read_byte(font.data + code);

  oft *= (font.height + 7) / 8;
  oft += (256 - ' ');

  for (i = 0; i < swd + font.space; i++) {
    if (i < swd) {
      data = 0;
      for (j = (font.height - 1) / 8; j >= 0; j--) {
        data <<= 8;
        pgmData = pgm_read_byte(font.data + oft + (swd * j) + i);
        data |= pgmData;
      }
    } else {
      data = 0;
    }
    if (!font.color)
      data = ~data;
    if (buf == BUF_SCROLL) {
      scrollCharBuf[i] = data;
    } else {
      if (outCol < HUB08_WIDTH)
        outBuf[outCol++] = data;
    }
  }

  return swd + font.space;
}

ISR(TIMER3_OVF_vect, ISR_NOBLOCK)
{
  static uint8_t swd;   // symbol width
  static uint8_t col;   // current symbol column
  static uint8_t tail;  // tail spaces counter
  static uint16_t data; // data to be scrolled

  switch (scroll) {
  case SCROLL_DRAW:
    matrixShift(data);
    if (++col >= swd) {
      scrollStrPtr++;
      scroll = SCROLL_START;
    }
    break;
  case SCROLL_TAIL:
    matrixShift(data);
    if (tail++ >= MATRIX_WIDTH)
      matrixScroll(SCROLL_STOP, ROW_BOTH);
    break;
  default:
    break;
  }

  if (scroll == SCROLL_START) {
    col = 0;
    tail = 0;
    if (*scrollStrPtr) {
      swd = matrixReadChar(*scrollStrPtr, BUF_SCROLL);
      scroll = SCROLL_DRAW;
    } else {
      scroll = SCROLL_TAIL;
    }
  }

  if (scroll == SCROLL_DRAW) {
    data = scrollCharBuf[col];
  } else {
    data = font.color ? 0x00 : 0xFFFF;
  }

  return;
}

void matrixInit(void)
{
  hub08Init();
  TIMSK3 = (1 << TOIE3);
  TCCR3B |= (0 << CS32) | (1 << CS31) | (0 << CS30);
}

void matrixSetBr(uint8_t level)
{
  hub08SetBr(level);
}

void matrixSetFont(const uint8_t *fnt, uint8_t color)
{
  font.height = pgm_read_byte(&fnt[FONT_HEIGHT]);
  font.space = pgm_read_byte(&fnt[FONT_SPACE]);
  font.data = fnt + FONT_DATA;
  font.color = color;
}


void matrixClear(MatrixRow row)
{
  uint8_t i;

  for (i = 0; i < MATRIX_WIDTH; i++)
    outBuf[i] = 0;

  matrixShow(row);

  return;
}

void matrixDrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
  if (x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT)
    return;

  hub08DrawPixel(x, y, color);

  return;
}

void matrixDrawColumn(uint8_t x, uint16_t data, MatrixRow row)
{
  int8_t k;
  uint8_t start = 0;
  uint8_t stop = 16;

  if (row == ROW_TOP) {
    stop = 8;
  } else if (row == ROW_BOTTOM) {
    data <<= 8;
    start = 8;
  }

  for (k = start; k < stop; k++)
    matrixDrawPixel(x, k, (data & (1 << k)) != 0);

  return;
}

void matrixShift(uint16_t data)
{
  MatrixRow row = scrollRow;

  if ((scrollRow & 0x03) == ROW_BOTTOM)
    data <<= 8;

  if (font.height > 8 || scrollRow >= ROW_BOTH)
    row = ROW_BOTH;

  hub08Shift(data, row);

  return;
}

void matrixLoadScrollString(char *str)
{
  while (*str)
    *scrollStrPtr++ = *str++;
  *scrollStrPtr = 0;

  return;
}

void matrixScroll(MatrixScroll mode, MatrixRow row)
{
  scrollRow = row;
  scrollStrPtr = scrollStrBuf;
  scroll = mode;

  return;
}

MatrixScroll matrixGetScroll(void)
{
  return scroll;
}

void matrixLoadOutString(char *str)
{
  while (*str)
    matrixReadChar(*str++, BUF_FIXED);

  return;
}

void matrixSetCol(uint8_t col)
{
  outCol = col;
}


void matrixShow(MatrixRow row)
{
  uint8_t i;

  if (row + scrollRow != ROW_BOTH) // Different small rows
    matrixScroll(SCROLL_STOP, ROW_BOTH);

  for (i = 0; i < MATRIX_WIDTH; i++) {
    matrixDrawColumn(i, outBuf[i], row);
  }

  return;
}
