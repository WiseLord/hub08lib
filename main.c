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

  matrixSetBr(16);

  while (1) {
    matrixClear(ROW_BOTH);
    matrixSetFont(font_matrix_16, 1);
    matrixLoadScrollString("Scroll");
    matrixLoadScrollString(" large font");
    matrixScroll(SCROLL_START, ROW_BOTH);
    matrixSetCol(0);
    matrixLoadOutString("Large");
    _delay_ms(5000);
    matrixShow(ROW_BOTH);
    _delay_ms(3000);

    matrixClear(ROW_BOTH);
    matrixSetFont(font_matrix_08, 1);
    matrixLoadScrollString("Scroll");
    matrixLoadScrollString(" top line");
    matrixScroll(SCROLL_START, ROW_TOP);
    matrixSetCol(0);
    matrixLoadOutString("Small");
    matrixLoadOutString(" font");
    _delay_ms(3000);
    matrixShow(ROW_BOTTOM);
    _delay_ms(3000);

    matrixSetFont(font_matrix_08, 1);
    matrixLoadScrollString("Scroll");
    matrixLoadScrollString(" bottom line");
    matrixScroll(SCROLL_START, ROW_BOTTOM);
    matrixSetCol(0);
    matrixLoadOutString("Small");
    matrixLoadOutString(" font");
    _delay_ms(3000);
    matrixShow(ROW_TOP);
    _delay_ms(3000);
  }

  return 0;
}
