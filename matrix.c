#include "matrix.h"

#include <avr/pgmspace.h>
#include <util/delay.h>

static const uint8_t *_fontData;
static uint8_t _fontHeight;
static uint8_t _fontColor;

static uint8_t _x, _y;
static uint16_t charBuf[11];                        // Big char buffer

static uint8_t *fb;                                 // Frame buffer

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
    swd = pgm_read_byte(_fontData + i);
    oft += swd;
  }
  swd = pgm_read_byte(_fontData + code);

  oft *= (_fontHeight + 7) / 8;
  oft += (256 - ' ');

  for (i = 0; i < swd; i++) {
    data = 0;
    for (j = (_fontHeight - 1) / 8; j >= 0; j--) {
      data <<= 8;
      pgmData = pgm_read_byte(_fontData + oft + (swd * j) + i);
      if (!_fontColor)
        pgmData = ~pgmData;
      data |= pgmData;
    }
    charBuf[i] = data;
  }

  return swd;
}

void matrixInit()
{
  fb = hub08Init();
}

void matrixBrightness(uint8_t level)
{
  hub08Brighness(level);
}

void matrixSetFont(const uint8_t *font, uint8_t color)
{
  _fontHeight = pgm_read_byte(&font[FONT_HEIGHT]);
  _fontData = font + FONT_DATA;
  _fontColor = color;
}

void matrixSetPos(uint8_t x, uint8_t y)
{
  _x = x;
  _y = y;

  return;
}


void matrixFIll(uint8_t data)
{
  uint8_t i;

  for (i = 0; i < 128; i++)
    fb[i] = data;

  return;
}

void matrixDrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
  uint8_t *pos = &fb[y * 8 + (7 - x / 8)];
  uint8_t bit = (0x80 >> (x % 8));

  if (x >= MATRIX_WIDTH)
    return;
  if (y >= MATRIX_HEIGHT)
    return;

  if (color)
    *pos |= bit;
  else
    *pos &= ~bit;

  return;
}

void matrixDrawColumn(uint8_t x, uint16_t data)
{
  int8_t k;

  for (k = 0; k < 16; k++)
    matrixDrawPixel(x, _y + k, (data & (1 << k)) != 0);

  return;
}


void matrixWriteChar(uint8_t code, uint8_t mode)
{
  uint8_t i;
  uint8_t swd = matrixReadChar(code);

  if (mode == OUT_MODE_SCROLL) {
    for (i = 0; i < swd; i++) {
      matrixScroll(charBuf[i]);
      _delay_ms(15);
    }
  } else {
    for (i = 0; i < swd; i++)
      matrixDrawColumn(_x + i, charBuf[i]);
    matrixSetPos(_x + swd, _y);
  }

  return;
}

void matrixWriteString(char *string, uint8_t mode)
{
  if (*string)
    matrixWriteChar(*string++, mode);
  while (*string) {
    matrixWriteChar(0x7F, mode);
    matrixWriteChar(*string++, mode);
  }

  return;
}

void matrixScroll(uint16_t data)
{
  int8_t i, j;
  uint8_t *buf;

  buf = &fb[MATRIX_FB_SIZE - 1];
  for (j = 15; j >= 0; j--) {
    for (i = 7; i >= 0; i--) {
      *buf <<= 1;
      if (i ? * (buf - 1) & 0x80 : data & (1 << j))
        *buf |= 0x01;
      buf--;
    }
  }

  return;
}

