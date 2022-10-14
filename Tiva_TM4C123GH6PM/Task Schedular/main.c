// PSP is used by task and MSP is used by Systick or pendSV.
// Each task will have its own Stack.
// each private stack will be of 1kb.
// We will be using simple Round-Robin preemptive Scheduling.
// Each task have equal priority (no priority value).
// Quanta/ Time_Slice is 1ms.

//------------------Header files-------------------------------------------
//-------------------------------------------------------------------------

#include "TM4C123GH6PM.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>

//-------------------------------------------------------------------------
//-----------------------------Global Variables----------------------------



uint32_t psp_of_tasks[MAX_TASKS] = {T1_STACK_START,T2_STACK_START,T3_STACK_START,T4_STACK_START};
uint32_t tasks_handler[MAX_TASKS]; // Storing address of every task_handler
uint32_t current_task = 0;// task 1 is running


//---------------------------Function Prototypes----------------------------
//--------------------------------------------------------------------------


__attribute__((naked)) void init_sched_stack(uint32_t sched_top_of_stack);
void init_tasks_stack(void);
void task1_handler(void);
void task2_handler(void); 
void task3_handler(void);
void task4_handler(void);
void init_systic_timer(void);
void enable_processor_faults(void);
void switch_sp_to_psp(void);

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


int main()
{
  enable_processor_faults(); // We are switching SPs and from c to inline assembly
  // Let's enable all faults handler to be safe.
  init_sched_stack(SCHED_STACK_START);
  
  tasks_handler[0] = (uint32_t)&task1_handler;
  tasks_handler[1] = (uint32_t)&task2_handler;
  tasks_handler[2] = (uint32_t)&task3_handler;
  tasks_handler[3] = (uint32_t)&task4_handler;
  
  init_tasks_stack();
  init_systic_timer();
  switch_sp_to_psp(); // User tasks will ues PSP.
  task1_handler(); // Manually starting the schedular for the first time
  while(1)
  {
    
  }
}


//---------------------Function Defination---------------------------------
//-------------------------------------------------------------------------


void update_next_task(void)
{
  current_task ++;
  current_task = (current_task % MAX_TASKS);
  // this is the round robin fashion of updating tasks
}


void save_psp_value(uint32_t current_psp_value)
{
  psp_of_tasks[current_task] = current_psp_value;
}

__attribute__((naked)) void init_sched_stack(uint32_t sched_top_of_stack)
{
  asm volatile("MSR MSP, R0");
//asm volatile("MSR MSP %0" : : "r"(sched_top_of_stack) :); both are same, first
//attribute is stored in R0
  asm volatile("BX LR");// return from function call(go back to main)
}


void init_tasks_stack(void)
{
  uint32_t *pPSP;
  for(int i = 0; i < MAX_TASKS; i++)
  {
    pPSP = (uint32_t*)psp_of_tasks[i];
    pPSP--;// the stack frame is full descending
    *pPSP = DUMMY_XPSR;// 0x01000000, that 1 is at 24th bit which 
    //represents the instr is of Thumb type
    
    pPSP--;//PC
    *pPSP = tasks_handler[i];
    
    pPSP--;//LR
    *pPSP = 0xFFFFFFFD; // return to thread mode, exception return uses non-floating point state
    //from the PSP and execution uses PSP after return
    
    for(int j = 0; j < 13; j++)
    {
      pPSP--;// zero in 13 registers R12,R3,R2,R1,R0,R11,R10,R9,R8,R7,R6,R5,R4. order matters
      *pPSP = 0x0;
    }
    psp_of_tasks[i] = (uint32_t)pPSP;
    
  }
}

void enable_processor_faults(void)
{
  // Enable usage, mem manage and bus fault
  SCB->SHCSR |= (1U << 16) | (1U << 17) | (1U << 18);
}

uint32_t get_psp_value(void)
{
  return psp_of_tasks[current_task];// return value is stored in R0, ARM procedure call standard
}

__attribute__((naked)) void switch_sp_to_psp(void)
{
  // Initialize the PSP to Task1 stack start address
  // first, get the value of psp of current task
  // {} -> Register set must be used with {}.
  asm volatile("PUSH {LR}");// this holds the address to return to main since
  // the current function was called from main
  asm volatile("BL get_psp_value"); // Branch + Link -> link cuz we need to come back
  // to this func.Save LR address to return to main or it will get corrupted by get_psp_value.
  asm volatile("MSR PSP, R0");// initialize PSP
  asm volatile("POP {LR}");
  
  //Change SP to PSP using CONTROL Register
  asm volatile("MOV R0, #0x02");
  asm volatile("MSR CONTROL, R0");
  asm volatile("BX LR");
   
}

void init_systic_timer(void)
{
  __disable_irq(); // Disable interrupt routine before configuring SysTick
  SysTick->CTRL &= ~(1U << 0); // disable the counter before configuring it
  SysTick->CTRL |= (1U << 1); // enable interrupt generation 
  SysTick->CTRL |= (1U << 2); // use system clock as clock source
  SysTick->LOAD = 16000-1; // 1ms delay
  SysTick->VAL = 0x0; // clear current value register
  SysTick->CTRL |= (1U << 0); // Enable the counter;
  __enable_irq(); // Enable interrupt routine after configuring SysTick

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//---------------------Task and SysTick Handlers Declaration-------------------
//-----------------------------------------------------------------------------

__attribute__((naked)) void inside_of_systick()
{
  asm volatile("MRS R0, PSP");// get the current value of PSP
  // using the PSP value store R11 to R4 in task's private stack
  // we cannot use PUSH. Handler is using MSP and it will affect the MSP.
  // STM is used to store to memory, STMBD = decrement then store
  asm volatile("STMDB R0!, {R4-R11}");// ! copies the last stored address to R0
  // save the current value of PSP(needed for POP)
  // Save LR value
  asm volatile("PUSH {LR}");
  asm volatile("BL save_psp_value");
  
  
  asm volatile("BL update_next_task");
  // get the changed task PSP value
  asm volatile("BL get_psp_value");
  asm volatile("LDMIA R0!, {R4-R11}");// Load Multiple regs and increment after.
  //update psp or you can say POP the changed task stack.
  asm volatile("MSR PSP, R0");
  asm volatile("POP {LR}");
    
}

void SysTick_Handler()
{
  asm volatile("PUSH {LR}");
  asm volatile("BL get_psp_value");
  asm volatile("POP {LR}");
  
}
void task1_handler(void)
{
  while(1)
  {
    printf("This is task 1\n");
  }
}
void task2_handler(void)
{
  while(1)
  {
    printf("This is task 2\n");
  }
}
void task3_handler(void)
{
  while(1)
  {
    printf("This is task 3\n");
  }
}
void task4_handler(void)
{
  while(1)
  {
    printf("This is task 4\n");
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



//-------------------------Fault Handlers---------------------------------------
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------