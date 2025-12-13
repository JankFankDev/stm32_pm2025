#include <stdint.h>
#include "stm32f10x.h"

#define MIN_FREQ 1
#define MAX_FREQ 64
#define BASE_FREQ 1000

volatile uint32_t current_freq = 1;
volatile uint8_t button_a_pressed = 0;
volatile uint8_t button_b_pressed = 0;

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        GPIOC->ODR ^= GPIO_ODR_ODR13;
    }
}

int main(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13_1;
    GPIOC->ODR &= ~GPIO_ODR_ODR13;

    GPIOB->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
    GPIOB->CRL |= GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1;
    GPIOB->ODR |= GPIO_ODR_ODR0 | GPIO_ODR_ODR1;
    
    TIM2->PSC = 7200 - 1;
    TIM2->ARR = (10000 / (2 * current_freq)) - 1;
    
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
    
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 0);
    
    while (1) {
        if (((GPIOB->IDR & GPIO_IDR_IDR0) == 0) && !button_a_pressed) {
            button_a_pressed = 1;
            if (current_freq < MAX_FREQ) {
                current_freq *= 2;
                TIM2->PSC = TIM2->PSC << 1;
                TIM2->EGR |= TIM_EGR_UG;
            }
        }
        
        if (((GPIOB->IDR & GPIO_IDR_IDR1) == 0) && !button_b_pressed) {
            button_b_pressed = 1;
            if (current_freq > MIN_FREQ) {
                current_freq /= 2;
                TIM2->PSC = TIM2->PSC >> 1;
                TIM2->EGR |= TIM_EGR_UG;
            }
        }
        
        if ((GPIOB->IDR & GPIO_IDR_IDR0) != 0) button_a_pressed = 0;
        if ((GPIOB->IDR & GPIO_IDR_IDR1) != 0) button_b_pressed = 0;
    }
}