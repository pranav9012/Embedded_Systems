// we will generate two interrupts using NVIC interrupt pending registers
// and observe the execution of ISRs when priorities are same and different
//

#include "TM4C123GH6PM.H"
#include <stdio.h>
#include <stdint.h>
#define IRQNO_I2C0 8
#define IRQNO_GPIOA 0

void priority_configurer(uint8_t IRQNO, uint8_t priority_value)
{
  // NVIC->IP is Interrupt Priority set Register
  // Check TM4C123GH6PM.h and core_cm4.h files for the Commands info
  NVIC->IP[IRQNO] = 0x00; // clear 
  NVIC->IP[IRQNO] = priority_value; 
  
}

int main()
{
   SYSCTL->RCGCGPIO |= (1U << 0);
  // configuring the priority of the peripherals
  
//  //--------------Same Priority-----------------------
//  
//  priority_configurer(IRQNO_I2C0, 0x80);
//  priority_configurer(IRQNO_GPIOA, 0x80);
//  
//  //-----------Set Interrupt Pending bit-------------
//  
//  NVIC->ISPR[0] |= (1U << IRQNO_I2C0);
//  
//  //---------------Enable Interrupe in NVIC----------
//  NVIC->ISER[0] |= (1U << IRQNO_GPIOA); // first let's enable the interrput for GPIO
//  NVIC->ISER[0] |= (1U << IRQNO_I2C0);
  
    //--------------Different Priority-----------------------
  
  priority_configurer(IRQNO_I2C0, 0x80);
  priority_configurer(IRQNO_GPIOA, 0x60);
  
  //-----------Set Interrupt Pending bit-------------
  
  NVIC->ISPR[0] |= (1U << IRQNO_I2C0);
  
  //---------------Enable Interrupe in NVIC----------
  NVIC->ISER[0] |= (1U << IRQNO_GPIOA); // first let's enable the interrput for GPIO
  NVIC->ISER[0] |= (1U << IRQNO_I2C0);
 
  while(1)
  {
    // waiting for interrupt
  }
  //return 0;
}

// ISRs
void I2C0_Handler(void)
{
  printf("inside I2C0 Handler\n");
  NVIC->ISPR[0] |= (1U << IRQNO_GPIOA); // triggering GPIO interrupt while 
  // I2C interrupt is triggered
  printf("ISR of GPIOA was executed\n");
}

void GPIOA_Handler(void)
{
  printf("inside GPIO A Handler\n");
}