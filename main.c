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

  const uint8_t *font = font_matrix_16;

  matrixSetBr(16);
  matrixSetFont(font, 1);

  while (1) {
    matrixClear(MATRIX_BOTH);
    matrixLoadString("Test");
    matrixLoadString(" scrolling");
    matrixLoadString(" screen");
    matrixScroll(SCROLL_START, MATRIX_TOP);
    _delay_ms(8000);
  }

  return 0;
}
