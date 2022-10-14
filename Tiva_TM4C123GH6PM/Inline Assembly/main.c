#include <stdio.h>
int main()
{
  //----------------Code------------------------
  //---------------------------------------------
//  asm volatile("LDR R1, =0x20001000");
//  asm volatile("LDR R2, =0x20001004");
//  asm volatile ("LDR R0, [R1]");
//  asm volatile ("LDR R1, [R2]");
//  asm volatile("ADD R0, R0,R1");
//   asm volatile("STR R0, [R2]");
  
  //----------------Code + Input operand---------------------
  //--------------------------------------------------
//  int val = 50;
//  asm volatile("MOV R0, %0" : : "r"(val)); // r - use register for data manipulation
//  asm volatile("MOV R0, %0" : : "i"(0x50)); // i - use the immediate value, 'r' also works here
  
  //---------------Code + Output operand-------------------------
  //-----------------------------------------------------
  // Read CONTROL reg to control_reg
//  int control_reg = 0;
//  asm volatile("MRS %0, CONTROL" : "=r"(control_reg)); // '=' indicates write-only operand and 'r'
                                                        // indicates that registers will be used for data manipulation
  
//-----------Copy the contents of C variable var1 to var2-----------------------
//  int var1= 10, var2 = 0;
//  printf("%d\t", var1);
//  printf("%d\t", var2);
//  
//  asm volatile("MOV %0, %1" : "=r"(var2) : "r"(var1));
//  
//  printf("%d\t", var1);
//  printf("%d\t", var2);
  
  //----------Pointer to a C variable-------------------------------------------
  
  int p1, *p2;
  p2 = (int*)0x20000008;
  asm volatile("LDR %0, [%1]" : "=r"(p1) : "r" (p2));
  printf("%d\t", p1);
  
  
  return 0;
}
