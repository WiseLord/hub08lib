#ifndef PINS_H
#define PINS_H

#include <avr/io.h>
#include "arduino.h"

#define CONCAT(x,y)         x ## y

#define DDR(x)              CONCAT(DDR,x)
#define PORT(x)             CONCAT(PORT,x)
#define PIN(x)              CONCAT(PIN,x)

#define HUB08_LA            ARDUINO_D4
#define HUB08_LA_LINE       ARDUINO_D4_LINE
#define HUB08_LB            ARDUINO_D5
#define HUB08_LB_LINE       ARDUINO_D5_LINE
#define HUB08_LC            ARDUINO_D6
#define HUB08_LC_LINE       ARDUINO_D6_LINE
#define HUB08_LD            ARDUINO_D7
#define HUB08_LD_LINE       ARDUINO_D7_LINE

#define HUB08_OE            ARDUINO_D8
#define HUB08_OE_LINE       ARDUINO_D8_LINE
#define HUB08_LAT           ARDUINO_D10
#define HUB08_LAT_LINE      ARDUINO_D10_LINE

// This should be connected to hardware SPI pins
#define HUB08_R1            ARDUINO_MOSI
#define HUB08_R1_LINE       ARDUINO_MOSI_LINE
#define HUB08_CLK           ARDUINO_SCLK
#define HUB08_CLK_LINE      ARDUINO_SCLK_LINE

#define BUTTON_1            ARDUINO_A0
#define BUTTON_1_LINE       ARDUINO_A0_LINE
#define BUTTON_2            ARDUINO_A1
#define BUTTON_2_LINE       ARDUINO_A1_LINE
#define BUTTON_3            ARDUINO_A2
#define BUTTON_3_LINE       ARDUINO_A2_LINE

#define BUZZER              ARDUINO_A5
#define BUZZER_LINE         ARDUINO_A5_LINE

#define I2C_SCL             ARDUINO_D3
#define I2C_SCL_LINE        ARDUINO_D3_LINE
#define I2C_SDA             ARDUINO_D2
#define I2C_SDA_LINE        ARDUINO_D2_LINE

#endif /* PINS_H */
