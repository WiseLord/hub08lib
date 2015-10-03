#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pins.h"
#include "hub08.h"

// Screen buffer
uint8_t row0[HUB08_WIDTH];
uint8_t row1[HUB08_WIDTH];

int main(void)
{
	uint8_t i, j;
	int16_t k;

	hub08Init();

	for (i = 0; i < HUB08_WIDTH; i++) {
		row0[i] = i;
		row1[i] = 255 - i;
	}

	uint8_t *ptr;
	uint8_t bitmask;

	while (1) {
		// 1000 cycled outputs
		for (k = 0; k < 1000; k++) {
			for (j = 0; j < HUB08_HEIGHT; j++) {
				// Select next line
				hub08SetLine(j);
				// Select buffer
				if (j < 8)
					ptr = row0;
				else
					ptr = row1;
				// Select bit for current line
				bitmask = (1<<(j % 8));
				for (i = 0; i < HUB08_WIDTH; i++) {
					// Set data
					if (*ptr++ & bitmask)
						PORT(HUB08_R1) |= HUB08_R1_LINE;
					else
						PORT(HUB08_R1) &= ~HUB08_R1_LINE;
					// Strob data
					PORT(HUB08_CLK) |= HUB08_CLK_LINE;
					PORT(HUB08_CLK) &= ~HUB08_CLK_LINE;
				}
				// Latch 64 bit for output
				PORT(HUB08_LAT) |= HUB08_LAT_LINE;
				PORT(HUB08_LAT) &= ~HUB08_LAT_LINE;

				// Clear string to avoid brightening
				PORT(HUB08_R1) &= ~HUB08_R1_LINE;
				for (i = 0; i < HUB08_WIDTH; i++) {
					// Strob data
					PORT(HUB08_CLK) |= HUB08_CLK_LINE;
					PORT(HUB08_CLK) &= ~HUB08_CLK_LINE;
				}
				// Latch 64 zero bits for output
				PORT(HUB08_LAT) |= HUB08_LAT_LINE;
				PORT(HUB08_LAT) &= ~HUB08_LAT_LINE;
			}
		}
		// 1 second delay
		PORT(HUB08_OE) |= HUB08_OE_LINE;
		_delay_ms(1000);
		PORT(HUB08_OE) &= ~HUB08_OE_LINE;
	}

	return 0;
}

