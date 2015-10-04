#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pins.h"
#include "hub08.h"

int main(void)
{
	uint8_t line;
	int16_t k;

	hub08Init();

	while (1) {
		// 1000 cycled outputs
		for (k = 0; k < 1000; k++)
			for (line = 0; line < HUB08_HEIGHT; line++)
				hub08ShowLine(line);

		// 1 second delay
		PORT(HUB08_OE) |= HUB08_OE_LINE;
		_delay_ms(1000);
		PORT(HUB08_OE) &= ~HUB08_OE_LINE;
	}

	return 0;
}

