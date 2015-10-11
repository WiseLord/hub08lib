#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pins.h"
#include "gdfb.h"

int main(void)
{
	gdInit();
	sei();

	gdSetBrightness(16);
	gdLoadFont(font_ks0066_ru_16, 1);

	while (1) {
		gdSetXY(0, 0);
		gdWriteString("ХЛЫЩИ");
	}

	return 0;
}

