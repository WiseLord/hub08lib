#include "matrix.h"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

Font font;

static char strBuf[MATRIX_STRING_MAX_LENGTH];
static char *strPtr = strBuf;
static uint16_t charBuf[MATRIX_CHAR_MAX_WIDTH];

static volatile uint8_t scroll = SCROLL_STOP;

static uint8_t matrixReadChar(uint8_t code)
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
    charBuf[i] = data;
  }

  return swd + font.space;
}

ISR (TIMER3_OVF_vect)
{
  static uint8_t swd; // symbol width
  static uint8_t col; // current symbol column

  if (scroll == SCROLL_START) {
    col = 0;
    if (*strPtr) {
      swd = matrixReadChar(*strPtr);
      scroll = SCROLL_DRAW;
    } else {
      scroll = SCROLL_STOP;
    }
  }

  if (scroll == SCROLL_DRAW) {
    matrixShift(charBuf[col]);
    if (++col >= swd) {
      strPtr++;
      scroll = SCROLL_START;
    }
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


void matrixClear(void)
{
  hub08Clear();

  return;
}

void matrixDrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
  if (x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT)
    return;

  hub08DrawPixel(x, y, color);

  return;
}

void matrixDrawColumn(uint8_t x, uint16_t data)
{
  int8_t k;

  for (k = 0; k < 16; k++)
    matrixDrawPixel(x, k, (data & (1 << k)) != 0);

  return;
}

void matrixShift(uint16_t data)
{
  hub08Shift(data);

  return;
}

void matrixLoadString(char *str)
{
  scroll = SCROLL_STOP;
  while (*str)
    *strPtr++ = *str++;
  *strPtr = 0;

  return;
}

void matrixScroll(uint8_t mode)
{
  strPtr = strBuf;
  scroll = mode;

  return;
}
