#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "pins.h"
#include "matrix.h"


int main(void)
{
  matrixInit();
  sei();
  matrixSetFont(font_matrix_16, 1);
  matrixLoadOutString("Large  ");
  matrixShow(ROW_BOTH, EFFECT_SCROLL_UP);

  DDR(BUZZER) |= BUZZER_LINE;
  PORT(BUZZER) &= ~BUZZER_LINE;
  _delay_ms(50);
  PORT(BUZZER) |= BUZZER_LINE;

  matrixClear(ROW_BOTH, EFFECT_NONE);
  matrixSetFont(font_matrix_16, 1);

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
