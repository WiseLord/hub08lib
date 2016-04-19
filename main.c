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
    matrixSetFont(font_matrix_08, 1);
    matrixClear(ROW_BOTH);
    matrixSetCol(0, ROW_TOP);
    matrixLoadOutString("Top");
    matrixSetCol(9, ROW_BOTTOM);
    matrixLoadOutString("Low");
    matrixSetFont(font_matrix_16, 1);
    matrixSetCol(30, ROW_BOTH);
    matrixLoadOutString("Big");
    matrixShow(ROW_BOTH);
    _delay_ms(2000);

    matrixSetFont(font_matrix_16, 1);
    matrixLoadScrollString(" Scroll");
    matrixLoadScrollString(" large font");
    matrixScroll(SCROLL_START, ROW_BOTH);
    matrixSetCol(0, ROW_TOP);
    matrixLoadOutString("Large  ");
    _delay_ms(6000);
    matrixShow(ROW_BOTH);
    _delay_ms(3000);

    matrixClear(ROW_BOTH);
    matrixSetFont(font_matrix_08, 1);
    matrixLoadScrollString("Scroll");
    matrixLoadScrollString(" top line");
    matrixScroll(SCROLL_START, ROW_TOP);
    matrixSetCol(0, ROW_BOTTOM);
    matrixLoadOutString("Small");
    matrixLoadOutString(" font");
    _delay_ms(2000);
    matrixShow(ROW_BOTTOM);
    _delay_ms(3000);

    matrixSetFont(font_matrix_08, 1);
    matrixLoadScrollString("Scroll");
    matrixLoadScrollString(" bottom line");
    matrixScroll(SCROLL_START, ROW_BOTTOM);
    matrixSetCol(0, ROW_TOP);
    matrixLoadOutString("Small");
    matrixLoadOutString(" font");
    _delay_ms(2000);
    matrixShow(ROW_TOP);
    _delay_ms(3000);
  }

  return 0;
}
