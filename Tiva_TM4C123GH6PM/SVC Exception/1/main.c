#include "TM4C123GH6PM.h"
#include <stdio.h>
#include <stdint.h>
// Execute a SVC instruction, Implement a SVC Handler, print the SVC number.
// Increment the SVC number by 4 and then print it in thread mode

int main()
{
  asm volatile ("SVC #0x08"); // this causes SVC exception
  uint32_t data; 
  asm volatile ("MOV %0, R0" : "=r"(data));
  printf("Returned to Thread mode\n");
  printf("SVC number is %d\n", data);
  
  while(1);
}

__attribute__((naked))void SVC_Handler(void)
{
  asm volatile("MRS R0, MSP");
  asm volatile("B SVC_Handler_c");// on branching R0 will be used and its 
  // content will be copied in the variable
}

void SVC_Handler_c(uint32_t *pBaseStackFrame)
{
  printf("In SVC handler\n");
  printf("PC value is %lx\n", pBaseStackFrame[6]);
  uint8_t *pRet_address =  (uint8_t*)(pBaseStackFrame[6]);
  printf("Return Address is %lx\n",pRet_address);
  pRet_address -= 2; // -2 to go to SVC opcode
  // SVC number is the LSB of the opcode;
  uint8_t SVC_number = *pRet_address;
  SVC_number += 4;
  // store the value of SVC_number to a register,return to thread mode
  // and then store that register value to another C variable
 
  pBaseStackFrame[0] = SVC_number;
  
}