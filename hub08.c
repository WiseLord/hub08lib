#include "hub08.h"

#include "pins.h"

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

	return;
}

void hub08SetLine(uint8_t line)
{
	// Switch off old line
	PORT(HUB08_OE) |= HUB08_OE_LINE;

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

	// Switch on new line
	PORT(HUB08_OE) &= ~HUB08_OE_LINE;

	return;
}
