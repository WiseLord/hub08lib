#include "matrix.h"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

Font font;

static char scrollStrBuf[MATRIX_STRING_MAX_LENGTH];
static char *scrollStrPtr = scrollStrBuf;
static uint8_t scrollCharBuf[MATRIX_CHAR_MAX_WIDTH][MATRIX_HEIGHT / 8];

static volatile MatrixScroll scroll = SCROLL_STOP;
static MatrixRow scrollRow = ROW_TOP;

static uint8_t outBuf[HUB08_WIDTH][MATRIX_HEIGHT / 8];
static uint8_t outCol = 0;

static uint8_t matrixReadChar(uint8_t code, MatrixOutbuf buf)
{
  uint8_t i;
  int j;

  uint16_t oft = 0;   // Current symbol offset in array
  uint8_t swd = 0;    // Current symbol width

  uint8_t data;

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
    for (j = (font.height - 1) / 8; j >= 0; j--) {
      if (i < swd)
        data = pgm_read_byte(font.data + oft + (swd * j) + i);
      else
        data = 0;
      if (!font.color)
        data = ~data;
      if (buf == BUF_SCROLL)
        scrollCharBuf[i][j] = data;
      else if (outCol < HUB08_WIDTH)
        outBuf[outCol][j] = data;
    }
    outCol++;
  }

  return swd + font.space;
}

ISR(TIMER3_OVF_vect, ISR_NOBLOCK)
{
  static uint8_t swd;                     // symbol width
  static uint8_t col;                     // current symbol column
  static uint8_t tail;                    // tail spaces counter
  static uint8_t data[MATRIX_HEIGHT / 8]; // data to be scrolled
  uint8_t i;

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

  for (i = 0; i < MATRIX_HEIGHT / 8; i++)
    if (scroll == SCROLL_DRAW)
      data[i] = scrollCharBuf[col][i];
    else
      data[i] = font.color ? 0x00 : 0xFF;

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
  uint8_t *ptr = outBuf[0];

  for (i = 0; i < MATRIX_FB_SIZE; i++)
    *ptr++ = 0;

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

void matrixDrawColumn(uint8_t x, uint8_t *data, MatrixRow row)
{
  int8_t r, y;

  if ((row & 0x03) == ROW_BOTTOM)
    data[1] = data[0];

  for (r = 0; r < MATRIX_HEIGHT / 8; r++) {
    if (row & (1 << r))
      for (y = 0; y < 8; y++)
        matrixDrawPixel(x, r * 8 + y, data[r] & (1 << y));
  }

  return;
}

void matrixShift(uint8_t *data)
{
  MatrixRow row = scrollRow;

  if ((row & 0x03) == ROW_BOTTOM)
    data[1] = data[0];

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

  for (i = 0; i < MATRIX_WIDTH; i++)
    matrixDrawColumn(i, outBuf[i], row);

  return;
}
