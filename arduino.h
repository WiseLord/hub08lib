#ifndef ARDUINO_H
#define ARDUINO_H

#define ARDUINO_LEONARDO

#ifdef ARDUINO_LEONARDO

#define ARDUINO_A0			F
#define ARDUINO_A0_LINE		(1<<7)
#define ARDUINO_A1			F
#define ARDUINO_A1_LINE		(1<<6)
#define ARDUINO_A2			F
#define ARDUINO_A2_LINE		(1<<5)
#define ARDUINO_A3			F
#define ARDUINO_A3_LINE		(1<<4)
#define ARDUINO_A4			F
#define ARDUINO_A4_LINE		(1<<1)
#define ARDUINO_A5			F
#define ARDUINO_A5_LINE		(1<<0)

#define ARDUINO_D0			D
#define ARDUINO_D0_LINE		(1<<2)
#define ARDUINO_D1			D
#define ARDUINO_D1_LINE		(1<<3)
#define ARDUINO_D2			D
#define ARDUINO_D2_LINE		(1<<1)
#define ARDUINO_D3			D
#define ARDUINO_D3_LINE		(1<<0)
#define ARDUINO_D4			D
#define ARDUINO_D4_LINE		(1<<4)
#define ARDUINO_D5			C
#define ARDUINO_D5_LINE		(1<<6)
#define ARDUINO_D6			D
#define ARDUINO_D6_LINE		(1<<7)
#define ARDUINO_D7			E
#define ARDUINO_D7_LINE		(1<<6)

#define ARDUINO_D8			B
#define ARDUINO_D8_LINE		(1<<4)
#define ARDUINO_D9			B
#define ARDUINO_D9_LINE		(1<<5)
#define ARDUINO_D10			B
#define ARDUINO_D10_LINE	(1<<6)
#define ARDUINO_D11			B
#define ARDUINO_D11_LINE	(1<<7)
#define ARDUINO_D12			D
#define ARDUINO_D12_LINE	(1<<6)
#define ARDUINO_D13			C
#define ARDUINO_D13_LINE	(1<<7)

#define ARDUINO_MOSI		B
#define ARDUINO_MOSI_LINE	(1<<2)
#define ARDUINO_MISO		B
#define ARDUINO_MISO_LINE	(1<<3)
#define ARDUINO_SCLK		B
#define ARDUINO_SCLK_LINE	(1<<1)
#define ARDUINO_SS			B
#define ARDUINO_SS_LINE		(1<<0)

#endif

#endif /* ARDUINO_H */
