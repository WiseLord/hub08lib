#include "hub08.h"

#include <avr/interrupt.h>
#include "pins.h"

// Screen buffer
uint8_t row0[HUB08_WIDTH];
uint8_t row1[HUB08_WIDTH];

static void hub08SetLine(uint8_t line)
{
	// Switch to new line
	if (line & (1<<0))
		PORT(HUB08_LA) |= HUB08_LA_LINE;
	else
		PORT(HUB08_LA) &= ~HUB08_LA_LINE;
	if (line & (1<<1))
		PORT(HUB08_LB) |= HUB08_LB_LINE;
	else
		PORT(HUB08_LB) &= ~HUB08_LB_LINE;
	if (line & (1<<2))
		PORT(HUB08_LC) |= HUB08_LC_LINE;
	else
		PORT(HUB08_LC) &= ~HUB08_LC_LINE;
	if (line & (1<<3))
		PORT(HUB08_LD) |= HUB08_LD_LINE;
	else
		PORT(HUB08_LD) &= ~HUB08_LD_LINE;

	return;
}

void hub08Init()
{
	// Init all lines as outputs
	DDR(HUB08_LA) |= HUB08_LA_LINE;
	DDR(HUB08_LB) |= HUB08_LB_LINE;
	DDR(HUB08_LC) |= HUB08_LC_LINE;
	DDR(HUB08_LD) |= HUB08_LD_LINE;

	DDR(HUB08_CLK) |= HUB08_CLK_LINE;
	DDR(HUB08_LAT) |= HUB08_LAT_LINE;
	DDR(HUB08_R1) |= HUB08_R1_LINE;
	DDR(HUB08_OE) |= HUB08_OE_LINE;

	// Default controls state
	PORT(HUB08_LA) &= ~HUB08_LA_LINE;
	PORT(HUB08_LB) &= ~HUB08_LB_LINE;
	PORT(HUB08_LC) &= ~HUB08_LC_LINE;
	PORT(HUB08_LD) &= ~HUB08_LD_LINE;

	PORT(HUB08_CLK) &= ~HUB08_CLK_LINE;
	PORT(HUB08_LAT) &= ~HUB08_LAT_LINE;
	PORT(HUB08_R1) &= ~HUB08_R1_LINE;
	PORT(HUB08_OE) |= HUB08_OE_LINE;

	TIMSK0 |= (1<<TOIE0);							/* Enable timer overflow interrupt */
	TIMSK0 |= (1<<OCIE0A);							/* Enable timer compare interrupt */
	TCCR0B |= (0<<CS02) | (1<<CS01) | (0<<CS00);	/* Set timer prescaller to 8 (16M/2/256 = 7812.5Hz)*/
	OCR0A = 140;

	return;
}

__attribute__((optimize("O3"))) ISR(TIMER0_OVF_vect)
{
	static uint8_t line;
	uint8_t i;
	uint8_t *ptr;
	uint8_t bitmask;

	if (++line >= 16)
		line = 0;

	// Select buffer
	if (line < 8)
		ptr = row0;
	else
		ptr = row1;
	// Select bit for current line
	bitmask = (1<<(line % 8));
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

	// Select next line
	hub08SetLine(line);

	// Latch 64 bit for output
	PORT(HUB08_LAT) |= HUB08_LAT_LINE;
	PORT(HUB08_LAT) &= ~HUB08_LAT_LINE;

	// Switch on new line
	PORT(HUB08_OE) &= ~HUB08_OE_LINE;

	return;
}

ISR (TIMER0_COMPA_vect)
{
	// Switch off everything
	PORT(HUB08_OE) |= HUB08_OE_LINE;

	return;
}


void hub08Fill(uint8_t data)
{
	uint8_t i;

	for (i = 0; i < HUB08_WIDTH; i++) {
		row0[i] = data;
		row1[i] = data;
	}

	return;
}

void hub08Brighness(uint8_t level)
{
	if (level > HUB08_MAX_BRIGNTNESS)
		level = HUB08_MAX_BRIGNTNESS;
	if (level) {
		TIMSK0 |= (1<<TOIE0);							/* Enable timer overflow interrupt */
		OCR0A = 135 + (level * (level + 1)) / 2;
	} else {
		TIMSK0 &= ~(1<<TOIE0);							/* Disable timer overflow interrupt */
	}

	return;
}

void hub08Pixel(uint8_t x, uint8_t y, uint8_t color) {
	if (color) {
		if (y < 8) {
			row0[x] |= (1 << y);
		} else {
			row1[x] |= (1 << (y - 8));
		}
	} else {
		if (y < 8) {
			row0[x] &= ~(1 << y);
		} else {
			row1[x] &= ~(1 << (y - 8));
		}
	}

	return;
}
