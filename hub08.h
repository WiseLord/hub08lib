#ifndef HUB08_H
#define HUB08_H

#include <inttypes.h>

#define HUB08_WIDTH				64
#define HUB08_HEIGHT			16

#define HUB08_MAX_BRIGNTNESS	15

void hub08Init(void);
void hub08Fill(uint8_t data);
void hub08Brighness(uint8_t level);
void hub08Pixel(uint8_t x, uint8_t y, uint8_t color);

#endif // HUB08_H
