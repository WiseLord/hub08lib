#ifndef HUB08_H
#define HUB08_H

#include <inttypes.h>
#include "fonts.h"

#define HUB08_WIDTH             64
#define HUB08_HEIGHT            16
#define HUB08_FB_SIZE           (HUB08_WIDTH / 8 * 16)

#define HUB08_MAX_BR            16

void hub08Init(void);
void hub08SetBr(uint8_t level);
void hub08DrawPixel(uint8_t x, uint8_t y, uint8_t color);
void hub08Shift(uint8_t *data, uint8_t row);

#endif // HUB08_H
