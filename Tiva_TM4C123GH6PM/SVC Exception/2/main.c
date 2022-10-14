#include "TM4C123GH6PM.h"
#include <stdio.h>
#include <stdint.h>

// a program to +, - , / , * 2 operands using SVC handler and print the result
// in thread mode code. Thread mode code should pass 2 operands via the stack frame
/* 
   SVC number             Operaiton
      36                  addition
      37                 subtraction
      38                multiplication
      39                  division
*/

void SVC_Handle_c(uint32_t *pBaseStackFrame);

int32_t add_numbers(int32_t x , int32_t y )
{
    int32_t res;
    __asm volatile("SVC #36");
    __asm volatile ("MOV %0,R0": "=r"(res) ::);
    return res;

}


int32_t sub_numbers(int32_t x , int32_t y )
{
   int32_t res;
    __asm volatile("SVC #37");
    __asm volatile ("MOV %0,R0": "=r"(res) ::);
    return res;

}

int32_t mul_numbers(int32_t x , int32_t y )
{

    int32_t res;
    __asm volatile("SVC #38");
    __asm volatile ("MOV %0,R0": "=r"(res) ::);
    return res;
}

int32_t div_numbers(int32_t x , int32_t y )
{
    int32_t res;
    __asm volatile("SVC #39");
    __asm volatile ("MOV %0,R0": "=r"(res) ::);
    return res;

}


int main(void)
{
    int32_t res;

    res = add_numbers(40, -90);
    printf("Add result = %ld\n", res);

    res = sub_numbers(25,150);
    printf("Sub result = %ld\n", res);

    res = mul_numbers(374,890);
    printf("mul result = %ld\n", res);

    res = div_numbers(67,-3);
    printf("div result = %ld\n",res);

    while(1);
}


__attribute__( ( naked ) ) void SVC_Handler( void )
{
    __asm volatile ("MRS R0,MSP");
    __asm volatile ( "B SVC_Handler_c");
}


void SVC_Handler_c(uint32_t *pBaseOfStackFrame)
{
    int32_t x; int32_t y; int32_t data;
   
      
    printf("in SVC handler\n");
    //printf("Base of stack farme is %lx\n", pBaseOfStackFrame);

    uint8_t *pReturn_addr = (uint8_t*)pBaseOfStackFrame[6];

    //2. decrement the return address by 2 to point to
    //opcode of the SVC instruction in the program memory
    pReturn_addr-=2;


    //3. extract the SVC number (LSByte of the opcode)
    uint8_t svc_number = *pReturn_addr;

    printf("Svc number is : %d\n",svc_number);


/*SVC number signifies the type of operation , decode it */
    x = pBaseOfStackFrame[0];
    y = pBaseOfStackFrame[1];
    
    printf("Value of x and y is %d , %d\n", x,y);
    
    switch(svc_number)
    {
    case 36:
      data = x + y;
        break;
    case 37:
      data = x - y;
        break;
    case 38:
      data = x*y;
        break;
    case 39:
      data = x/y;
        break;
    default:
        printf("invalid svc code\n");

    }

/*store the result back to stack frame at R0's position */
    pBaseOfStackFrame[0] = data;

}

