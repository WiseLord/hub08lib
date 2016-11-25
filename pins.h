#ifndef PINS_H
#define PINS_H

#include <avr/io.h>
#include "arduino.h"

#define CONCAT(x,y)         x ## y

#define DDR(x)              CONCAT(DDR,x)
#define PORT(x)             CONCAT(PORT,x)
#define PIN(x)              CONCAT(PIN,x)

#define HUB08_LA            ARDUINO_D5
#define HUB08_LA_LINE       ARDUINO_D5_LINE
#define HUB08_LB            ARDUINO_D4
#define HUB08_LB_LINE       ARDUINO_D4_LINE
#define HUB08_LC            ARDUINO_D3
#define HUB08_LC_LINE       ARDUINO_D3_LINE
#define HUB08_LD            ARDUINO_D2
#define HUB08_LD_LINE       ARDUINO_D2_LINE

#define HUB08_OE            ARDUINO_D10
#define HUB08_OE_LINE       ARDUINO_D10_LINE
#define HUB08_R1            ARDUINO_MOSI
#define HUB08_R1_LINE       ARDUINO_MOSI_LINE
#define HUB08_LAT           ARDUINO_D12
#define HUB08_LAT_LINE      ARDUINO_D12_LINE
#define HUB08_CLK           ARDUINO_SCLK
#define HUB08_CLK_LINE      ARDUINO_SCLK_LINE

#define BUTTON_1            ARDUINO_D9
#define BUTTON_1_LINE       ARDUINO_D9_LINE
#define BUTTON_2            ARDUINO_D8
#define BUTTON_2_LINE       ARDUINO_D8_LINE
#define BUTTON_3            ARDUINO_D7
#define BUTTON_3_LINE       ARDUINO_D7_LINE

#define BUZZER              ARDUINO_D6
#define BUZZER_LINE         ARDUINO_D6_LINE

#define I2C_SCL             ARDUINO_SCL
#define I2C_SCL_LINE        ARDUINO_SCL_LINE
#define I2C_SDA             ARDUINO_SDA
#define I2C_SDA_LINE        ARDUINO_SDA_LINE

#define MAX7219_LOAD        ARDUINO_D12
#define MAX7219_LOAD_LINE   ARDUINO_D12_LINE
#define MAX7219_CLK         ARDUINO_SCLK
#define MAX7219_CLK_LINE    ARDUINO_SCLK_LINE
#define MAX7219_DIN         ARDUINO_MOSI
#define MAX7219_DIN_LINE    ARDUINO_MOSI_LINE

#endif /* PINS_H */
