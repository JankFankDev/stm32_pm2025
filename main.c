#include <stdint.h>
#include "stm32f10x.h"

#define MIN_FREQ 1
#define MAX_FREQ 64

volatile uint32_t current_freq = 1;
volatile uint8_t button_a_pressed = 0;
volatile uint8_t button_b_pressed = 0;

int main(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOB->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
    GPIOB->CRL |= GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1;
    GPIOB->ODR |= GPIO_ODR_ODR0 | GPIO_ODR_ODR1;
    
    while (1) {
		if (((GPIOB->IDR & GPIO_IDR_IDR0) == 0) && !button_a_pressed) {
			button_a_pressed = 1;
			if (current_freq < MAX_FREQ) current_freq *= 2;
		}
		if (((GPIOB->IDR & GPIO_IDR_IDR1) == 0) && !button_b_pressed) {
			button_b_pressed = 1;	
			if (current_freq > MIN_FREQ) current_freq /= 2;
		}	
		if (!((GPIOB->IDR & GPIO_IDR_IDR0) == 0)) button_a_pressed = 0;	
		if (!((GPIOB->IDR & GPIO_IDR_IDR1) == 0)) button_b_pressed = 0;
        
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        
		for (uint32_t i = 0; i < 1000000 / current_freq; i++) __NOP();
    }
}