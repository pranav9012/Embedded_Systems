#include "TM4C123GH6PM.H"
#include <stdio.h>

#define SRAM_START 0x20000000U // we cannot use c macros in inline assembly code
#define SRAM_SIZE (128*1024)
#define SRAM_END ((SRAM_START) + (SRAM_SIZE))
#define STACK_START SRAM_END
#define STACK_MSP_START STACK_START
#define STACK_MSP_END (STACK_START + (512))// 1kb of SRAM, half for MSP and half for PSP
//#define STACK_PSP_START STACK_MSP_END

int fun_add(int a, int b, int c, int d)
{
  return a+b+c+d;
}

void generate_exception(void)
{
  asm volatile("SVC #0x2");// SVC expection will be triggered.
}

 __attribute__((naked))void change_sp_to_psp(void)
{
  //asm volatile(".equ SRAM_END, (0x20000000 + (128 * 1024))"); // these 2 are not working
  //asm volatile(".equ PSPSTART, (SRAM_END - 512)");
  asm volatile("LDR R0, =((0x20000000 + (128 * 1024)) - 512)");// is a 32bit value
  asm volatile("MSR PSP, R0");
  asm volatile("MOV R0, #0x02"); // can use MOV cuz of 16bit value
  asm volatile("MSR CONTROL, R0");
  asm volatile("BX LR"); // go back to main function
  
}

int main()
{
  change_sp_to_psp();
  int ret = 0;
  ret = fun_add(1,4,5,6);
  printf("result = %d\n", ret);
  generate_exception();
  
  return 0;
}

void SVC_Handler(void)
{
  printf("in SVC_handler");
}
