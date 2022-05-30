#include "TM4C123GH6PM.H"

/* Timer0 A Interrupt handler function */

void TIMER0A_Handler(void)
{
  if(TIMER0->MIS & 0x1)
  {
  TIMER0->ICR |= (1U << 0); // Clear the interrupt flag
  GPIOF->DATA ^= (1U << 2); // Toggle Blue LED
  return;
  }
}

int main()
{
  /* Configuring Port F */
  SYSCTL->RCGCGPIO |= (1U << 5); // Enable clock to Port F
  GPIOF->DIR |= (1U << 2); // Make pin 2 an output in (Glowing Blue LED)
  GPIOF->DEN |= (1U << 2); // Enable digital functionality of pin 2
  
  /* Configuting GPTM */
  SYSCTL->RCGCTIMER |= (1U << 0); // Enable clock to Timer 0
  TIMER0->CTL &= ~(1U << 0); // Disable timer0 before configuring it
  TIMER0->CFG |= 0x00; // This value selects the 32-bit timer
  TIMER0->TAMR &= ~(1U << 4); // Counter counts down to '0'
  TIMER0-> TAMR |= 0x02; // Counter works in periodic mode
  TIMER0->TAILR = 0x00F42400; // Genearting 1sec delay, F42400H = 16000000 in decimal
  TIMER0->IMR |= (1U << 0); // Generate interrupt when timerA reaches '0' 
  TIMER0->CTL |= (1U << 0); // Enable timer 0
  NVIC->ISER[0] |= (1U << 19); // Interrupt set enable
  
  while(1)
  {
    // waiting for interrupt
  }
}