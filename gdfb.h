#ifndef FBGD_H
#define FBGD_H

#include "hub08.h"

#define GD_MIN_BRIGHTNESS			ST7920_MIN_BRIGHTNESS
#define GD_MAX_BRIGHTNESS			ST7920_MAX_BRIGHTNESS

#define GD_SIZE_X					HUB08_WIDTH
#define GD_SIZE_Y					HUB08_HEIGHT

#include "fonts.h"

#define FONT_PARAM_COUNT			6
enum {
	FONT_HEIGHT,
	FONT_LTSPPOS,
	FONT_CCNT,
	FONT_OFTA,
	FONT_OFTNA,
	FONT_COLOR,
};

inline void gdInit(void);
inline void gdClear(void);

void gdSetBrightness(uint8_t br);

inline void gdDrawPixel(uint8_t x, uint8_t y, uint8_t color);

void gdDrawHorizLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t color);
void gdDrawVertLine(uint8_t x, uint8_t y1, uint8_t y2, uint8_t color);
void gdDrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);

void gdDrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void gdDrawFilledRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

void gdDrawCircle(uint8_t x0, uint8_t y0, int16_t radius, uint8_t color);

void gdSetXY(uint8_t x, uint8_t y);
uint8_t gdGetX(void);
void gdLoadFont(const uint8_t *font, uint8_t color);

void gdWriteChar(uint8_t code);
void gdWriteString(char *string);

#endif /* FBGD_H */
