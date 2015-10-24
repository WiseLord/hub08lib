#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pins.h"
#include "hub08.h"

int main(void)
{
	hub08Init();
	sei();

	hub08Brighness(8);
//	gdLoadFont(font_matrix_08, 1);
	hub08SetFont(font_dosapp103_16, 1);

//	hub08Fill(0xFF);
	while (1) {
		hub08SetXY(0, 0);
		hub08WriteString("Hello!");
	}

	return 0;
}

