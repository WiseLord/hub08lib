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

	hub08Fill(0x00);
	while (1) {
		hub08Fill(0x00);
		hub08SetXY(0, 0);
		hub08WriteString("Тест", OUT_MODE_FIXED);
		_delay_ms(500);
		hub08WriteString("Проверка скроллирования", OUT_MODE_SCROLL);
		_delay_ms(500);
	}

	return 0;
}

