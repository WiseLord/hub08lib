#include "hub08.h"

#include <avr/interrupt.h>
#include "pins.h"

static uint8_t fb[HUB08_FB_SIZE];					// Frame buffer
static int8_t line;									// Current line

static void hub08SelectLine(void)
{
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

static void hub08LoadLineData()
{
	int8_t matr;

	// Switch to new line
	if (--line < 0)
		line = 15;

	// Prepare data for new line
	for (matr = 7; matr >= 0; matr--) {
		SPDR = fb[line * (HUB08_WIDTH / 8) + matr];	// It takes time to calculate index,
		//while(!(SPSR & (1<<SPIF)));				// so we can skip reading SPI status
	}

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

	TIMSK0 |= (1<<TOIE0);							// Enable timer overflow interrupt
	TIMSK0 |= (1<<OCIE0A);							// Enable timer compare interrupt
	TCCR0B |= (0<<CS02) | (1<<CS01) | (0<<CS00);	// Set timer prescaller to 8 (16M/8/256 = 7812.5Hz)
	OCR0A = 255;

	/* Configure Hardware SPI */
	SPCR = (1<<SPE) | (1<<MSTR);
	SPSR = (1<<SPI2X);

	return;
}

ISR (TIMER0_OVF_vect)
{
	// Latch current line to buffers
	PORT(HUB08_LAT) |= HUB08_LAT_LINE;
	PORT(HUB08_LAT) &= ~HUB08_LAT_LINE;

	PORT(HUB08_OE) &= ~HUB08_OE_LINE;				// Switch on current line

	// Prepare new data if there is enough time for this operation
	if (OCR0A >= 128)
		hub08LoadLineData();

	return;
}

ISR (TIMER0_COMPA_vect)
{
	PORT(HUB08_OE) |= HUB08_OE_LINE;				// Switch off current line

	// Prepare new data if there is enough time for this operation
	if (OCR0A < 128)
		hub08LoadLineData();

	// Switch to new line

	hub08SelectLine();
	return;
}

void hub08Fill(uint8_t data)
{
	uint8_t i;

	for (i = 0; i < 128; i++)
		fb[i] = data;

	return;
}

void hub08Brighness(uint8_t level)
{
	uint8_t brStep = 256 / HUB08_MAX_BRIGNTNESS;

	if (level > HUB08_MAX_BRIGNTNESS)
		level = HUB08_MAX_BRIGNTNESS;

	if (level) {
		OCR0A = (level - 1) * brStep - 1 + brStep;
		TIMSK0 |= (1<<TOIE0);						/* Enable timer overflow interrupt */
		TIMSK0 |= (1<<OCIE0A);						// Enable timer compare interrupt
	} else {
		TIMSK0 &= ~(1<<TOIE0);						/* Disable timer overflow interrupt */
		TIMSK0 &= ~(1<<OCIE0A);						// Disable timer compare interrupt
		PORT(HUB08_OE) |= HUB08_OE_LINE;			// Switch off current line
	}

	return;
}

void hub08Pixel(uint8_t x, uint8_t y, uint8_t color)
{
	uint8_t *pos = &fb[y * 8 + (7 - x / 8)];
	uint8_t bit = (0x80 >> (x % 8));

	if (x >= HUB08_WIDTH)
		return;
	if (y >= HUB08_HEIGHT)
		return;

	if (color)
		*pos |= bit;
	else
		*pos &= ~bit;

	return;
}
