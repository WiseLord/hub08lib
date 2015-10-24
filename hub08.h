#ifndef HUB08_H
#define HUB08_H

#include <inttypes.h>
#include "fonts.h"

#define HUB08_WIDTH				64
#define HUB08_HEIGHT			16
#define HUB08_FB_SIZE			(HUB08_WIDTH / 8 * 16)

#define HUB08_MAX_BRIGNTNESS	32

void hub08Init(void);
void hub08Fill(uint8_t data);
void hub08Brighness(uint8_t level);
void hub08Pixel(uint8_t x, uint8_t y, uint8_t color);

void hub08SetXY(uint8_t x, uint8_t y);
void hub08SetFont(const uint8_t *font, uint8_t color);

void hub08WriteChar(uint8_t code);
void hub08WriteString(char *string);

#endif // HUB08_H
