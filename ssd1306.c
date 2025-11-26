#include "ssd1306.h"
#include "spi.h"
#include "stm32f10x.h"

static uint8_t display_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

void SSD1306_Command(uint8_t cmd) {
    GPIOA->BRR = GPIO_BRR_BR4;
    GPIOA->BRR = GPIO_BRR_BR1;
    
    SPI1_Write(cmd);
    
    GPIOA->BSRR = GPIO_BSRR_BS4;
}

void SSD1306_Data(uint8_t data) {
    GPIOA->BRR = GPIO_BRR_BR4;
    GPIOA->BSRR = GPIO_BSRR_BS1;
    
    SPI1_Write(data);
    
    GPIOA->BSRR = GPIO_BSRR_BS4;
}

void SSD1306_Init(void) {
    SPI1_Init();
    
    for(volatile int i = 0; i < 10000; i++);
    
    SSD1306_Command(0xAE);
    
    SSD1306_Command(0x20);
    SSD1306_Command(0x00);
    
    SSD1306_Command(0x21);
    SSD1306_Command(0x00);
    SSD1306_Command(0x7F);
    
    SSD1306_Command(0x22);
    SSD1306_Command(0x00);
    SSD1306_Command(0x07);
    
    SSD1306_Command(0x40);
    
    SSD1306_Command(0x81);
    SSD1306_Command(0xFF);
    
    SSD1306_Command(0xA1);
    SSD1306_Command(0xC8);
    
    SSD1306_Command(0xA6);
    SSD1306_Command(0xA8);
    SSD1306_Command(0x3F);
    
    SSD1306_Command(0xD3);
    SSD1306_Command(0x00);
    
    SSD1306_Command(0xD5);
    SSD1306_Command(0x80);
    
    SSD1306_Command(0xD9);
    SSD1306_Command(0x22);
    
    SSD1306_Command(0xDA);
    SSD1306_Command(0x12);
    
    SSD1306_Command(0xDB);
    SSD1306_Command(0x20);
    
    SSD1306_Command(0x8D);
    SSD1306_Command(0x14);
    
    SSD1306_Command(0xAF);
    
    SSD1306_Clear();
}

void SSD1306_Clear(void) {
    for (int i = 0; i < sizeof(display_buffer); i++) {
        display_buffer[i] = 0x00;
    }
    SSD1306_Update();
}

void SSD1306_Update(void) {
    SSD1306_Command(0x21);
    SSD1306_Command(0x00);
    SSD1306_Command(0x7F);
    
    SSD1306_Command(0x22);
    SSD1306_Command(0x00);
    SSD1306_Command(0x07);
    
    for (int i = 0; i < sizeof(display_buffer); i++) {
        SSD1306_Data(display_buffer[i]);
    }
}

void SSD1306_SetPixel(uint8_t x, uint8_t y, uint8_t color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
    
    uint16_t index = x + (y / 8) * SSD1306_WIDTH;
    
    if (color) {
        display_buffer[index] |= (1 << (y % 8));
    } else {
        display_buffer[index] &= ~(1 << (y % 8));
    }
}

void SSD1306_DrawChessBoard(void) {
    SSD1306_Clear();
    
    uint8_t cell_size = 8;
    
    for (uint8_t y = 0; y < SSD1306_HEIGHT; y++) {
        for (uint8_t x = 0; x < SSD1306_WIDTH; x++) {
            uint8_t cell_x = x / cell_size;
            uint8_t cell_y = y / cell_size;
            uint8_t color = (cell_x + cell_y) % 2;
            
            SSD1306_SetPixel(x, y, color);
        }
    }
    
    SSD1306_Update();
}