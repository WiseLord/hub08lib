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

  matrixBrightness(8);
  matrixSetFont(font, 1);

  matrixFIll(0x00);
  while (1) {
    matrixFIll(0x00);
    matrixSetPos(0, 0);
    matrixWriteString("Тест", OUT_MODE_FIXED);
    _delay_ms(500);
    matrixWriteString("Проверка скроллирования", OUT_MODE_SCROLL);
    _delay_ms(500);
  }

  return 0;
}
