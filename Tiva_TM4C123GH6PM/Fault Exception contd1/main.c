// In this we will trigger all fault exception
#include "TM4C123GH6PM.H"
#include <stdio.h>
#include <stdint.h>
int fun_divide(int x, int y)
{
  return x/y;
}
int main()
{
  // Enable usage, mem manage and bus fault
  SCB->SHCSR |= (1U << 16) | (1U << 17) | (1U << 18);
  // if we don't enable usagefault, it will be escalated to hard fault
  // SCB->SHCSR &= ~(1U << 18);
  
  // Enable divide by zero trap
  SCB->CCR |= (1U << 4);
  
  //Dividing by zero
  fun_divide(10,0);
  
  return 0;
}

// Implement the fault handlers
void HardFault_Handler(void)
{
   printf("Inside HardFault Handler\n");
    while(1);
}

void MemManage_Handler(void)
{
  printf("Inside MemManage Handler\n");
    while(1);
}

void BusFault_Handler(void)
{
    printf("Inside BusFault Handler\n");
    while(1);
}

__attribute__((naked)) void UsageFault_Handler(void)
{
      asm volatile ("MRS R0, MSP"); 
      asm volatile ("B UsageFault_Handler_c"); // jump to this address
      

}

// according to Procedure call standard for Arm, 
// content of R0 becomes the 1st argument of the callee, in our case pBaseStackFrame 
void UsageFault_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pUFSR = (uint32_t*) (0xE000E000 + 0xD2A);

    printf("UFSR = %x\n",(*pUFSR));
    
   // When a debugger is not available, we analyze stack to find out the cause of exception
   //-------------------Method - 02 Analyzing Stack---------------------------------------
    //------------------------------------------------------------------------------------
  //---------------NOTE------------------------------------------------------------------
  
    /* The handler should excute after the Stack is printed, if the handler instruction is
        executed first, there are chances that MSP may change and will not point the stacked registers.
    */
  //----------------------------------------------------------------------------------------
    // In the disassembly you can see the compiler generated some other instruction
  // before our assembly instructions are used, so the value inside MSP has chagned
  // and the Value printed is not correct
  // for this we will write function using assembly language.
  

    printf("MSP value = %p\n", pBaseStackFrame);
    printf("R0 value = %lx\n", pBaseStackFrame[0]);
    printf("R1 value = %lx\n", pBaseStackFrame[1]); 
    printf("R2 value = %lx\n", pBaseStackFrame[2]);
    printf("R3 value = %lx\n", pBaseStackFrame[3]);
    printf("R12 value = %lx\n", pBaseStackFrame[4]);
    printf("LR value = %lx\n", pBaseStackFrame[5]);
    printf("PC value = %lx\n", pBaseStackFrame[6]);
    printf("xPSR value = %lx\n", pBaseStackFrame[7]);
    printf("Inside UsageFault Handler\n");
    
    while(1);
}