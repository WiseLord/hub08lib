#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pins.h"
#include "gdfb.h"

int main(void)
{
	gdInit();
	sei();

	gdSetBrightness(8);
	gdLoadFont(font_matrix_08, 1);

	hub08Fill(0xFF);
	while (1) {
		gdSetXY(0, 0);
		gdWriteString("Понедельник");
	}

	return 0;
}

