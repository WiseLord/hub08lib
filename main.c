#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pins.h"
#include "hub08.h"

uint8_t row0[64];
uint8_t row1[64];

void line(uint8_t l)
{
	PORT(HUB08_OE) |= HUB08_OE_LINE;
	if (l & (1<<0)) PORT(HUB08_LA) |= HUB08_LA_LINE; else PORT(HUB08_LA) &= ~HUB08_LA_LINE;
	if (l & (1<<1)) PORT(HUB08_LB) |= HUB08_LB_LINE; else PORT(HUB08_LB) &= ~HUB08_LB_LINE;
	if (l & (1<<2)) PORT(HUB08_LC) |= HUB08_LC_LINE; else PORT(HUB08_LC) &= ~HUB08_LC_LINE;
	if (l & (1<<3)) PORT(HUB08_LD) |= HUB08_LD_LINE; else PORT(HUB08_LD) &= ~HUB08_LD_LINE;
	PORT(HUB08_OE) &= ~HUB08_OE_LINE;
}

int main(void)
{
	uint8_t i, j;
	int16_t k;

	hub08Init();

//	sei();

	for (i = 0; i < 64; i++) {
		row0[i] = i;
		row1[i] = i;
	}

	PORT(HUB08_OE) &= ~HUB08_OE_LINE;

	uint8_t *ptr;
	uint8_t bitmask;

	while (1) {
		for (k = 0; k < 1000; k++) {
			for (j = 0; j < 16; j++) {
				line(j);
				if (j < 8)
					ptr = row0;
				else
					ptr = row1;
				bitmask = (1<<(j % 8));
				for (i = 0; i < 64; i++) {
					if (*ptr++ & bitmask)
						PORT(HUB08_R1) |= HUB08_R1_LINE;
					else
						PORT(HUB08_R1) &= ~HUB08_R1_LINE;
					PORT(HUB08_CLK) |= HUB08_CLK_LINE;
					PORT(HUB08_CLK) &= ~HUB08_CLK_LINE;
				}
				PORT(HUB08_LAT) |= HUB08_LAT_LINE;
				PORT(HUB08_LAT) &= ~HUB08_LAT_LINE;
			}
		}
		_delay_ms(1000);
	}

	return 0;
}

