#ifndef PINS_H
#define PINS_H

#include <avr/io.h>
#include "arduino.h"

#define CONCAT(x,y)			x ## y

#define DDR(x)				CONCAT(DDR,x)
#define PORT(x)				CONCAT(PORT,x)
#define PIN(x)				CONCAT(PIN,x)

#define HUB08_LA			ARDUINO_A0
#define HUB08_LA_LINE		ARDUINO_A0_LINE
#define HUB08_LB			ARDUINO_A1
#define HUB08_LB_LINE		ARDUINO_A1_LINE
#define HUB08_LC			ARDUINO_A2
#define HUB08_LC_LINE		ARDUINO_A2_LINE
#define HUB08_LD			ARDUINO_A3
#define HUB08_LD_LINE		ARDUINO_A3_LINE

#define HUB08_CLK			ARDUINO_D8
#define HUB08_CLK_LINE		ARDUINO_D8_LINE
#define HUB08_LAT			ARDUINO_D9
#define HUB08_LAT_LINE		ARDUINO_D9_LINE
#define HUB08_R1			ARDUINO_D10
#define HUB08_R1_LINE		ARDUINO_D10_LINE
#define HUB08_OE			ARDUINO_D11
#define HUB08_OE_LINE		ARDUINO_D11_LINE

#endif /* PINS_H */

