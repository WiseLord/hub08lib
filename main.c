#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pins.h"
#include "hub08.h"

int main(void)
{
	hub08Init();
	sei();

	hub08Brighness(32);

	int8_t i, j;

	while (1) {
		hub08Fill(0xFF);

		for (j = 1; j <= HUB08_MAX_BRIGNTNESS; j++) {
			hub08Brighness(j);
			_delay_ms(50);
		}
		for (j = HUB08_MAX_BRIGNTNESS; j >= 1; j--) {
			hub08Brighness(j);
			_delay_ms(100);
		}

		hub08Fill(0x00);
		_delay_ms(500);
		hub08Brighness(32);

		for (j = 63; j >= 0; j--) {
			for (i = 15; i >= 0; i--)
				hub08Pixel(j, i, 1);
			_delay_ms(20);
			for (i = 0; i < 16; i++)
				hub08Pixel(j, i, 0);
		}
		_delay_ms(500);

	}

	return 0;
}

