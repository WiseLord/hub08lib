#include "matrix.h"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "fonts.h"

Font font;

static char scrollStrBuf[MATRIX_STRING_MAX_LENGTH];
static char *scrollStrPtr = scrollStrBuf;
static uint8_t scrollCharBuf[MATRIX_CHAR_MAX_WIDTH][MATRIX_HEIGHT / 8];

static volatile MatrixScroll scroll = SCROLL_STOP;
static MatrixRow scrollRow = ROW_TOP;

static uint8_t scrBuf[MATRIX_WIDTH][MATRIX_HEIGHT / 8];
static uint8_t outBuf[MATRIX_WIDTH][MATRIX_HEIGHT / 8];
static uint16_t *scrBig = (uint16_t *)scrBuf;
static uint16_t *outBig = (uint16_t *)outBuf;
static uint8_t outCol = 0;
static uint8_t outRow = 0;

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
        scrollCharBuf[i][j + (scrollRow == ROW_BOTTOM)] = data;
      else if (outCol < MATRIX_WIDTH)
        outBuf[outCol][j + (outRow == ROW_BOTTOM)] = data;
    }
    outCol++;
  }

  return swd + font.space;
}

ISR(TIMER1_OVF_vect, ISR_NOBLOCK)
{
  static uint8_t swd;                     // symbol width
  static uint8_t col;                     // current symbol column
  static uint8_t tail;                    // tail spaces counter
  static uint8_t data[MATRIX_HEIGHT / 8]; // data to be scrolled
  uint8_t i;

  TCNT1 = 32768;

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
#if defined(HUB08)
  hub08Init();
#elif defined(MAX7219)
  max7219Init();
#endif
  TIMSK1 = (1 << TOIE1);
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
}

void matrixSetBr(uint8_t level)
{
#if defined(HUB08)
  hub08SetBr(level);
#elif defined(MAX7219)
  max7219SendCmd(MAX7219_INTENSITY, level);
#endif
}

void matrixSetFont(const uint8_t *fnt, uint8_t color)
{
  font.height = pgm_read_byte(&fnt[FONT_HEIGHT]);
  font.space = pgm_read_byte(&fnt[FONT_SPACE]);
  font.data = fnt + FONT_DATA;
  font.color = color;
}

void matrixClear(MatrixRow row, MatrixEffect effect)
{
  uint8_t i;
  uint8_t *ptr = outBuf[0];

  for (i = 0; i < MATRIX_FB_SIZE; i++)
    *ptr++ = 0x00;

  matrixShow(row, effect);

  return;
}

void matrixDrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
  if (x >= MATRIX_WIDTH || y >= MATRIX_HEIGHT)
    return;

  if (color)
    scrBig[x] |= (1<<y);
  else
    scrBig[x] &= ~(1<<y);

#if defined(HUB08)
  hub08DrawPixel(x, y, color);
#endif

  return;
}

void matrixDrawColumn(uint8_t x, uint8_t *data, MatrixRow row)
{
  int8_t r, y;

  for (r = 0; r < MATRIX_HEIGHT / 8; r++) {
    if (row & (1 << r))
      for (y = 0; y < 8; y++)
        matrixDrawPixel(x, r * 8 + y, data[r] & (1 << y));
  }
#if defined(MAX7219)
  max7219SendDataBuf(scrBig);
#endif

  return;
}

void matrixShift(uint8_t *data)
{
  uint8_t i, r;
#if defined(HUB08)
  hub08Shift(data, scrollRow);
#endif

  for (r = 0; r < MATRIX_HEIGHT / 8; r++) {
    if (scrollRow & (1 << r)) {
      for (i = 0; i < MATRIX_WIDTH - 1; i++) {
        scrBuf[i][r] = scrBuf[i + 1][r];
      }
      scrBuf[MATRIX_WIDTH - 1][r] = data[r];
    }
  }
#if defined(MAX7219)
  max7219SendDataBuf(scrBig);
#endif

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

void matrixSetCol(uint8_t col, uint8_t row)
{
  outCol = col;
  outRow = row;

  return;
}

void matrixShow(MatrixRow row, MatrixEffect effect)
{
  uint8_t i, r, b;

  if (row == scrollRow || row + scrollRow > ROW_BOTH)
    matrixScroll(SCROLL_STOP, ROW_BOTH);

  switch (row) {
  case ROW_TOP:
  case ROW_BOTTOM:
    r = row - ROW_TOP;
    switch (effect) {
    case EFFECT_SCROLL_UP:
      for (b = 0; b < 8; b++) {
        for (i = 0; i < MATRIX_WIDTH; i++) {
          scrBuf[i][r] >>= 1;
          if (outBuf[i][r] & (0x01 << b))
            scrBuf[i][r] |= 0x80;
          matrixDrawColumn(i, scrBuf[i], row);
        }
        _delay_ms(10);
      }
      break;
    case EFFECT_SCROLL_DOWN:
      for (b = 0; b < 8; b++) {
        for (i = 0; i < MATRIX_WIDTH; i++) {
          scrBuf[i][r] <<= 1;
          if (outBuf[i][r] & (0x80 >> b))
            scrBuf[i][r] |= 0x01;
          matrixDrawColumn(i, scrBuf[i], row);
        }
        _delay_ms(10);
      }
      break;
    default:
      for (i = 0; i < MATRIX_WIDTH; i++) {
        scrBuf[i][r] = outBuf[i][r];
        matrixDrawColumn(i, scrBuf[i], row);
      }
      break;
    }
    break;
  case ROW_BOTH:
    switch (effect) {
    case EFFECT_SCROLL_UP:
      for (b = 0; b < 16; b++) {
        for (i = 0; i < MATRIX_WIDTH; i++) {
          scrBig[i] >>= 1;
          if (outBig[i] & (0x0001 << b))
            scrBig[i] |= 0x8000;
          matrixDrawColumn(i, scrBuf[i], row);
        }
        _delay_ms(5);
      }
      break;
    case EFFECT_SCROLL_DOWN:
      for (b = 0; b < 16; b++) {
        for (i = 0; i < MATRIX_WIDTH; i++) {
          scrBig[i] <<= 1;
          if (outBig[i] & (0x8000 >> b))
            scrBig[i] |= 0x0001;
          matrixDrawColumn(i, scrBuf[i], row);
        }
        _delay_ms(5);
      }
      break;
    default:
      for (i = 0; i < MATRIX_WIDTH; i++) {
        for (r = 0; r < MATRIX_HEIGHT / 8; r++)
          scrBuf[i][r] = outBuf[i][r];
        matrixDrawColumn(i, scrBuf[i], row);
      }
      break;
    }
    break;
  default:
    break;
  }

  return;
}
