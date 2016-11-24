#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "pins.h"
#include "matrix.h"
#include "rtc.h"
#include "fonts.h"

static char *mkNumberString(int16_t value, uint8_t width, uint8_t prec, uint8_t lead)
{
  static char strbuf[8];

  uint8_t sign = lead;
  int8_t pos;

  if (value < 0) {
    sign = '-';
    value = -value;
  }

  // Clear buffer and go to it's tail
  for (pos = 0; pos < width + prec; pos++)
    strbuf[pos] = lead;
  strbuf[pos--] = '\0';

  // Fill buffer from right to left
  while (value > 0 || pos > width - 2) {
    if (prec && (width - pos - 1 == 0))
      strbuf[pos--] = '.';
    strbuf[pos] = value % 10 + 0x30;
    pos--;
    value /= 10;
  }

  if (pos >= 0)
    strbuf[pos] = sign;

  return strbuf;
}

int main(void)
{
  matrixInit();
  sei();

  matrixSetBr(4);

  matrixSetFont(font_matrix_16, 1);
  matrixLoadOutString("Large  ");
  matrixShow(ROW_BOTH, EFFECT_SCROLL_UP);

  DDR(BUZZER) |= BUZZER_LINE;
  PORT(BUZZER) &= ~BUZZER_LINE;
  _delay_ms(50);
  PORT(BUZZER) |= BUZZER_LINE;

  matrixClear(ROW_BOTH, EFFECT_NONE);

  while (1) {
    rtcReadTime();
    matrixSetCol(0, ROW_BOTH);
    matrixSetFont(font_matrix_16, 1);
    matrixLoadOutString(mkNumberString(rtc.hour, 2, 0, '0'));
    matrixLoadOutString(":");
    matrixLoadOutString(mkNumberString(rtc.min, 2, 0, '0'));
    matrixSetFont(font_matrix_08, 1);
    matrixSetCol(55, ROW_TOP);
    matrixLoadOutString(mkNumberString(rtc.sec, 2, 0, '0'));
    matrixSetCol(55, ROW_BOTTOM);
    matrixLoadOutString(mkNumberString(rtc.date, 2, 0, '0'));
    matrixShow(ROW_BOTH, EFFECT_NONE);
  }

  while (1) {
    matrixSetFont(font_matrix_08, 1);
    matrixClear(ROW_BOTH, EFFECT_SCROLL_DOWN);
    matrixSetCol(0, ROW_TOP);
    matrixLoadOutString("Top");
    matrixSetCol(9, ROW_BOTTOM);
    matrixLoadOutString("Low");
    matrixSetFont(font_matrix_16, 1);
    matrixSetCol(30, ROW_BOTH);
    matrixLoadOutString("Big");
    matrixShow(ROW_BOTH, EFFECT_SCROLL_UP);
    _delay_ms(1000);

    matrixSetFont(font_matrix_16, 1);
    matrixLoadScrollString(" Scroll");
    matrixLoadScrollString(" large font");
    matrixScroll(SCROLL_START, ROW_BOTH);
    matrixSetCol(0, ROW_TOP);
    matrixLoadOutString("Large  ");
    _delay_ms(3000);
    matrixShow(ROW_BOTH, EFFECT_SCROLL_UP);
    _delay_ms(1000);

    matrixClear(ROW_BOTH, EFFECT_SCROLL_DOWN);
    matrixSetFont(font_matrix_08, 1);
    matrixLoadScrollString("Scroll");
    matrixLoadScrollString(" top line");
    matrixScroll(SCROLL_START, ROW_TOP);
    matrixSetCol(0, ROW_BOTTOM);
    matrixLoadOutString("Small");
    matrixLoadOutString(" font");
    _delay_ms(1000);
    matrixShow(ROW_BOTTOM, EFFECT_SCROLL_UP);
    _delay_ms(1000);

    matrixSetFont(font_matrix_08, 1);
    matrixLoadScrollString("Scroll");
    matrixLoadScrollString(" bottom line");
    matrixScroll(SCROLL_START, ROW_BOTTOM);
    matrixSetCol(0, ROW_TOP);
    matrixLoadOutString("Small");
    matrixLoadOutString(" font");
    _delay_ms(1000);
    matrixShow(ROW_TOP, EFFECT_SCROLL_DOWN);
    _delay_ms(1000);
  }

  return 0;
}
