#include "TM4C123GH6PM.h"
/* Blinking Red LED using SysTick Counter */

void SysTick_Handler()
{
  GPIOF->DATA ^= (1U << 1);
  return;
}

int main()
{
  /* Enabling clock to GPIO Port F */
    SYSCTL->RCGCGPIO |= (1U << 5); // Enable clock to GPIOF
  
  /* Initializing Red LED */  
    GPIOF->DIR |= (1U << 1); // Set Pin 1 of port F as output
    GPIOF->DEN |= (1U << 1); // Enable digital function
    
  /* Initializing SysTick Counter */   
    __disable_irq(); // Disbale the interrupts before using them
    SysTick->CTRL |= (1U << 0); // Diable the counter before configuring it
    SysTick->CTRL |= (1U << 1); // Generate interrupt when the counter hits '0'
    SysTick->CTRL |= (1U << 2); // Set system clock(16MHz) as clock source
    SysTick->LOAD = 16000000-1; // load the delay value. we will be creating a one sec delay
    SysTick->VAL = 0x0; // Initialize the current value register
    __enable_irq(); // Enable interrupts after configuring them
  
  while (1)
  {
    // waiting for interrupt
  }
}

