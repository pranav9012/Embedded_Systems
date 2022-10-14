#include "TM4C123GH6PM.H"
#include <stdio.h>
#include <stdint.h>


void generate_intrrupt()
{ 
  //enable IRQ0 interrupt
  NVIC->ISER[0] = (1U << GPIOB_IRQn);
  // Software triggered interrupt on IRQ0
  NVIC->STIR |= 0x01;
  
}

int main()
{
  printf("In thread mode : before Interrupt\n");
  generate_intrrupt();
  printf("In thread mode : after Interrupt\n");
  
  
  return 0;
}

void GPIOB_Handler()
{
  printf("In handeler mode\n");
}