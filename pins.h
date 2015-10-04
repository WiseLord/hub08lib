#ifndef PINS_H
#define PINS_H

#include <avr/io.h>
#include "arduino.h"

#define CONCAT(x,y)			x ## y

#define DDR(x)				CONCAT(DDR,x)
#define PORT(x)				CONCAT(PORT,x)
#define PIN(x)				CONCAT(PIN,x)

#define HUB08_LA			ARDUINO_D4
#define HUB08_LA_LINE		ARDUINO_D4_LINE
#define HUB08_LB			ARDUINO_D5
#define HUB08_LB_LINE		ARDUINO_D5_LINE
#define HUB08_LC			ARDUINO_D6
#define HUB08_LC_LINE		ARDUINO_D6_LINE
#define HUB08_LD			ARDUINO_D7
#define HUB08_LD_LINE		ARDUINO_D7_LINE

#define HUB08_OE			ARDUINO_D8
#define HUB08_OE_LINE		ARDUINO_D8_LINE
#define HUB08_R1			ARDUINO_D9
#define HUB08_R1_LINE		ARDUINO_D9_LINE
#define HUB08_LAT			ARDUINO_D10
#define HUB08_LAT_LINE		ARDUINO_D10_LINE
#define HUB08_CLK			ARDUINO_D11
#define HUB08_CLK_LINE		ARDUINO_D11_LINE

#endif /* PINS_H */

