#ifndef MAIN_H_
#define MAIN_H_
//------------------------------------------------------------------------------------------
//----------------------------------Defining MACROS-----------------------------------------


#define MAX_TASKS 4
#define TASK_STACK_SIZE 1024U // each stack is of 1kB
#define SCHED_STACK_SIZE 1024U
#define SRAM_START 0x20000000U // the start of SRAM
#define SIZE_OF_SRAM (32 * 1024) // the SRAM is of 32kB
#define SRAM_END ((SRAM_START + SIZE_OF_SRAM))
#define T1_STACK_START SRAM_END // The stack utilization is full descensding 
#define T2_STACK_START ((SRAM_END) - (TASK_STACK_SIZE))
#define T3_STACK_START ((SRAM_END) - (2*TASK_STACK_SIZE))
#define T4_STACK_START ((SRAM_END) - (3*TASK_STACK_SIZE))
#define SCHED_STACK_START ((SRAM_END) - (4*TASK_STACK_SIZE))
#define DUMMY_XPSR 0x01000000U

//-------------------------------------------------------------------------------------------

#endif /* MAIN_H_ */
