#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "pins.h"
#include "hub08.h"

int main(void)
{
	hub08Init();
	sei();

	const uint8_t *font = font_matrix_16;

	hub08Brighness(8);
	hub08SetFont(font, 1);

	int16_t i, j;

	while (1) {
		j = 0;
		for (i = 0x20; i < 0x100; i++) {
			if (++j >= 5) {
				j = 0;
				hub08Fill(0x00);
				hub08SetXY(0, 0);
			}
			hub08WriteChar(i);
			hub08WriteChar(0x7F);
			if (pgm_read_byte(&font_matrix_16[i - 0x20 + 1]) > 0x01) {
				_delay_ms(300);
			}
		}
	}

	return 0;
}

