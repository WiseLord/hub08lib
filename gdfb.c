#include "gdfb.h"

#include <avr/pgmspace.h>
#include <avr/eeprom.h>

static const uint8_t *_fontData;
static uint8_t _fontHeight;
static uint8_t _fontColor;

static uint8_t _x, _y;

inline void gdInit(void)
{
	hub08Init();

	return;
}

inline void gdClear(void)
{
	hub08Fill(0x00);

	return;
}

void gdSetBrightness(uint8_t br)
{
	hub08Brighness(br);

	return;
}

inline void gdDrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
	hub08Pixel(x, y, color);

	return;
}

void gdDrawHorizLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t color)
{
	uint8_t i;

	/* Swap X */
	if (x1 > x2) {
		i = x1;
		x1 = x2;
		x2 = i;
	}

	for (i = x1; i <= x2; i++)
		gdDrawPixel(i, y, color);

	return;
}

void gdDrawVertLine(uint8_t x, uint8_t y1, uint8_t y2, uint8_t color)
{
	uint8_t i;

	/* Swap Y */
	if (y1 > y2) {
		i = y1;
		y1 = y2;
		y2 = i;
	}

	for (i = y1; i <= y2; i++)
		gdDrawPixel(x, i, color);

	return;
}

void gdDrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
	gdDrawHorizLine(x, x + w - 1, y, color);
	gdDrawHorizLine(x, x + w - 1, y + h - 1, color);
	gdDrawVertLine(x, y, y + h - 1, color);
	gdDrawVertLine(x + w - 1, y, y + h - 1, color);

	return;
}

void gdDrawFilledRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color)
{
	uint8_t i;

	for (i = 0; i < h; i++)
		gdDrawHorizLine(x, x + w - 1, y + i, color);

	return;
}

void gdDrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
	int8_t sX, sY, dX, dY, err;
	int16_t err2;

	sX = x1 < x2 ? 1 : -1;
	sY = y1 < y2 ? 1 : -1;
	dX = sX > 0 ? x2 - x1 : x1 - x2;
	dY = sY > 0 ? y2 - y1 : y1 - y2;
	err = dX - dY;

	while (x1 != x2 || y1 != y2) {
		gdDrawPixel(x1, y1, color);
		err2 = err * 2;
		if (err2 > -dY / 2) {
			err -= dY;
			x1 += sX;
		}
		if (err2 < dX) {
			err += dX;
			y1 += sY;
		}
	}
	gdDrawPixel(x2, y2, color);

	return;
}

void gdDrawCircle(uint8_t x0, uint8_t y0, int16_t radius, uint8_t color)
{
	int8_t x, y;
	int16_t delta, err, px, py;

	x = 0;
	y = radius;
	delta = 2 - 2 * radius;
	err = 0;

	while (y >= x)
	{
		if ((px = x0 - x) >= 0 && (py = y0 - y) >= 0)
			gdDrawPixel(px, py, color);
		if ((px = x0 - y) >= 0 && (py = y0 - x) >= 0)
			gdDrawPixel(px, py, color);

		if ((px = x0 + x) < GD_SIZE_X && (py = y0 - y) >= 0)
			gdDrawPixel(px, py, color);
		if ((px = x0 + y) < GD_SIZE_X && (py = y0 - x) >= 0)
			gdDrawPixel(px, py, color);

		if ((px = x0 - x) >= 0 && (py = y0 + y) < GD_SIZE_Y)
			gdDrawPixel(px, py, color);
		if ((px = x0 - y) >= 0 && (py = y0 + x) < GD_SIZE_Y)
			gdDrawPixel(px, py, color);

		if ((px = x0 + x) < GD_SIZE_X && (py = y0 + y) < GD_SIZE_Y)
			gdDrawPixel(px, py, color);
		if ((px = x0 + y) < GD_SIZE_X && (py = y0 + x) < GD_SIZE_Y)
			gdDrawPixel(px, py, color);

		err = 2 * (delta + y) - 1;

		if(delta < 0 && err <= 0) {
			++x;
			delta += 2 * x + 1;
			continue;
		}
		err = 2 * (delta - x) - 1;
		if(delta > 0 && err > 0) {
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void gdSetXY(uint8_t x, uint8_t y)
{
	_x = x;
	_y = y;

	return;
}

uint8_t gdGetX(void)
{
	return _x;
}

void gdLoadFont(const uint8_t *font, uint8_t color)
{
	_fontHeight = pgm_read_byte(&font[FONT_HEIGHT]);
	_fontData = font + FONT_DATA;
	_fontColor = color;
}

void gdWriteChar(uint8_t code)
{
	uint8_t i;
	uint8_t j;
	uint8_t k;

	uint8_t pgmData;

	uint8_t spos = code - ' ';

	uint16_t oft = 0;	/* Current symbol offset in array*/
	uint8_t swd = 0;	/* Current symbol width */

	for (i = 0; i < spos; i++) {
		swd = pgm_read_byte(_fontData + i);
		oft += swd;
	}
	swd = pgm_read_byte(_fontData + spos);

	oft *= (_fontHeight + 7) / 8;
	oft += (256 - ' ');

	for (j = 0; j < (_fontHeight + 7) / 8; j++) {
		for (i = 0; i < swd; i++) {
			pgmData = pgm_read_byte(_fontData + oft + (swd * j) + i);
			if (!_fontColor)
				pgmData = ~pgmData;
			for (k = 0; k < 8; k++)
				gdDrawPixel(_x + i, _y + (8 * j + k), pgmData & (1<<k));
		}
	}
	gdSetXY(_x + swd, _y);

	return;
}

void gdWriteString(char *string)
{
	if (*string)
		gdWriteChar(*string++);
	while(*string) {
		gdWriteChar(0x7F);
		gdWriteChar(*string++);
	}
	return;
}
