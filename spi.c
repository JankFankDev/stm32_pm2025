#include "spi.h"
#include "stm32f10x.h"


void SPI1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN;
    

    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5 |
                    GPIO_CRL_CNF7 | GPIO_CRL_MODE7 |
                    GPIO_CRL_CNF4 | GPIO_CRL_MODE4 |
                    GPIO_CRL_CNF1 | GPIO_CRL_MODE1);
    
    GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5 |
                   GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7 |
                   GPIO_CRL_CNF4_0 | GPIO_CRL_MODE4 |
                   GPIO_CRL_CNF1_0 | GPIO_CRL_MODE1);
    
    SPI1->CR1 = SPI_CR1_MSTR |
                SPI_CR1_BR_1 |
                SPI_CR1_SSM |
                SPI_CR1_SSI;
    
    SPI1->CR2 = 0;
    
    SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI1_Write(uint8_t data) {
    while (!(SPI1->SR & SPI_SR_TXE));
    
    SPI1->DR = data;
    
    while (SPI1->SR & SPI_SR_BSY);
}

uint8_t SPI1_Read(void) {
    SPI1_Write(0x00);
    
    while (!(SPI1->SR & SPI_SR_RXNE));
    
    return SPI1->DR;
}