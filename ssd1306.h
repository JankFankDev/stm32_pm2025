#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

#define SSD1306_WIDTH  128
#define SSD1306_HEIGHT 64

void SSD1306_Init(void);
void SSD1306_Command(uint8_t cmd);
void SSD1306_Data(uint8_t data);
void SSD1306_Clear(void);
void SSD1306_Update(void);
void SSD1306_DrawChessBoard(void);
void SSD1306_SetPixel(uint8_t x, uint8_t y, uint8_t color);

#endif