#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pins.h"
#include "hub08.h"

int main(void)
{
	hub08Init();
	sei();

	hub08Brighness(5);

	uint8_t i, j;

	while (1) {
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 16; j++) {
				hub08Pixel(i, j, 1);
			}
			_delay_ms(10);
		}
		for (i = 0; i < 64; i++) {
			for (j = 0; j < 16; j++) {
				hub08Pixel(i, j, 0);
			}
			_delay_ms(10);
		}

	}

	return 0;
}

