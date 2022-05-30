#include "TM4C123GH6PM.h"
volatile unsigned int adc_value;
void ADC0SS3_Handler(void){
  adc_value = ADC0->SSFIFO3; // read adc coversion result from SS3 FIFO
  ADC0->ISC = 8;          // clear coversion clear flag bit
  if (adc_value>2048)
    {
      GPIOF->DATA |= (1<<1);
    }
    else
    {
      GPIOF->DATA &= ~(1<<1);
    }
  ADC0->PSSI |= (1<<3);        // Enable SS3 conversion or start sampling data from AN0 
  return;

}
int main(void)
{
   /* Enable Clock to ADC0 and GPIO pins*/
    SYSCTL->RCGCGPIO |= (1<<4);   // Enable Clock to GPIOE or PE3/AN0 
    SYSCTL->RCGCADC |= (1<<0);    // AD0 clock enable
    
    /* initialize PE3 for \AIN0 input  */
    GPIOE->AFSEL |= (1<<3);       // enable alternate function 
    GPIOE->DEN &= ~(1<<3);        // disable digital function 
    GPIOE->AMSEL |= (1<<3);       // enable analog function 
   
    /* initialize sample sequencer3 */
    ADC0->ACTSS &= ~(1<<3);        // disable SS3 during configuration 
    ADC0->EMUX &= ~0xF000;    // software trigger conversion 
    ADC0->SSMUX3 = 0;         // get input from channel 0 
    ADC0->SSCTL3 |= (1<<1)|(1<<2);        // take one sample at a time, set flag at 1st sample 
   
    /* Enable ADC Interrupt */
    ADC0->IM |= (1<<3);       // Unmask ADC0 sequence 3 interrupt
    NVIC->ISER[0] |= 0x00020000;        // enable IRQ17 for ADC0SS3
    ADC0->ACTSS |= (1<<3);      // enable ADC0 sequencer 3 
    ADC0->PSSI |= (1<<3);       //Enable SS3 conversion or start sampling data from AN0 
    
    while(1)
    {
      ADC0SS3_Handler();
    }
}