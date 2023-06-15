#ifndef SYSTEMDEFINITIONS_H_
#define SYSTEMDEFINITIONS_H_
/*
 * systemDefinitions.h
 *
 *  Created on: Jun 13, 2023
 *      Author: Berat Erdemkilic
 */
/********************************************************************************/
/********************************** DIAGRAMS ************************************/
/********************************************************************************/

/*
 * 												TASK MEMORY ALLIGNMENT
 * We need to keep in mind that in ARM Cortex Mx processors, RAM utilisation is full descending. It will get initialized
 * by starting from the higher end.
 *
 * Stack Allignment Sequence:
 * <---------------------------<--------------------------<------------------------<----------------------------
 * RAM_START<-->TS_SE<-->TS_SS<-->T4_SE<-->T4_SS<-->T3_SE<-->T3_SS<-->T2_SE<-->T2_SS<-->T1_SE<-->T1_SS<-->RAM_END
 * <---------------------------<--------------------------<------------------------<----------------------------
 *
 * RAM_START = RAM Start Address
 * TS_SE = Task Scheduler Stack End Address
 * TS_SS = Task Scheduler Stack Start Address
 * T4_SE = Task 4 Stack End Address
 * T4_SS = Task 4 Stack Start Address
 * T3_SE = Task 3 Stack End Address
 * T3_SS = Task 3 Stack Start Address
 * T2_SE = Task 2 Stack End Address
 * T2_SS = Task 2 Stack Start Address
 * T1_SE = Task 1 Stack End Address
 * T1_SS = Task 1 Stack Start Address
 * RAM_END = RAM End Address
 *
 * 												SCHEDULING POLICY
 * -> Round robin pre-emptive scheduling
 * -> No task priority
 * -> sysTick timer based timing control (T = 1ms)
 * -> During the scheduler task (context) switching, following register values are preserved:
 *    General purpose registers (R0 to R12) + Stack Pointer (PSP) + Link Register (LR) + Program Counter (PC)
 *    + Program Status Register (PSR) .....> (Private/Special Registers are ignored)
 *
 *    Task X is being run -> Save the content of Task X into its private stack (PUSH) -> Save the PSP value of
 *    task X -> Get the current PSP value of task Y (while the exception handler is running) ->
 *    Retrieve the content of Task Y from its private stack (POP) -> Run task Y
 *
 * 												CLOCK DATA
 * Processor clock = 16 MHz
 * SysTick Timer count clock = 16 MHz
 *
 * HSI --> System Clock --> Processor Clock -- SysTick Timer
 *
 *
 * 												STACK STRUCTURE
 *
 * SF1 (Stack Frame 1) -> xPSR + PC + LR + R12 + R0 to R3
 * SF2 (Stack Frame 2) -> R4 to R11
 * PSP (Stack Pointer) -> To be stored into global storage area
 *
 * Design note:
 * --> In the initial stack frame, all register values shall be assigned to 0.
 * --> For the EPSR, T bit must be set to Thumb State mode. ARM Cortex M4 processor only supports execution of Thumb State (1).
 *     xPSR for every task = 0x01000000
 * --> Return Address (PC) = Address of the Task X handler (LSB of the address must be 1 which stands for Thumb State!)
 * --> LR = 0xFFFFFFFD (Return to thread mode + Non FP state from PSP + Execution will use PSP after return)
 *
 * */

/********************************************************************************/
/********************************** INCLUDES ************************************/
/********************************************************************************/

/********************************************************************************/
/********************************** DEFINITIONS *********************************/
/********************************************************************************/
typedef enum
{
	SYSTEM_STATE_FALSE = 0,
	SYSTEM_STATE_TRUE
}SYSTEM_DEFINITIONS_STATE_ENUM;

typedef enum
{
	TASK_1 = 0,
	TASK_2,
	TASK_3,
	TASK_4,
	TASK_TOTAL_NUM_OF_TASKS
}SYSTEM_DEFINITIONS_NUM_OF_TASKS_ENUM;

/* Definition of stack sizes
 * Start address of SRAM : 0x20000000
 * End address of SRAM   : 0x3FFFFFFF
 *
 * Start address of SRAM for the allocation: 0x30000000
 * End address of SRAM for the allocation  : 0x3FFFFFFF
 *
*/
#define STACK_SIZE_USER_TASK 			1024U
#define STACK_SIZE_SCHEDULER_TASK 		1024U

#define STACK_USED_START_ADDR			0x20000000U
#define STACK_USED_END_ADDR				((STACK_USED_START_ADDR)+(STACK_USED_MEM_SIZE))
#define STACK_USED_MEM_SIZE				((128)*(1024))

#define STACK_T1_SS						STACK_USED_END_ADDR
#define STACK_T2_SS						(STACK_USED_END_ADDR-(1*STACK_SIZE_USER_TASK))
#define STACK_T3_SS						(STACK_USED_END_ADDR-(2*STACK_SIZE_USER_TASK))
#define STACK_T4_SS						(STACK_USED_END_ADDR-(3*STACK_SIZE_USER_TASK))
#define STACK_TS_SS						(STACK_USED_END_ADDR-(4*STACK_SIZE_USER_TASK))

/* Definition of clock and timer values
 * Processor clock = 16 MHz
 * SysTick Timer count clock = 16 MHz
 *
*/
#define TIMER_CLOCK_DIVIDER				16000U
#define TIMER_HSI_CLOCK_FREQ			16000000U
#define TIMER_DESIRED_FREQ				((TIMER_HSI_CLOCK_FREQ)/(TIMER_CLOCK_DIVIDER)) /* 1000Hz , 1ms */

#define TIMER_CLEAR_32BIT_REG(reg)		(*(uint32_t*)reg) &= 0x00000000U
#define TIMER_ENABLE_SYSTICK_EXC(reg)	(*(uint32_t*)reg) |= (1 << 1)	/* Enable systick exception request */
#define TIMER_INDC_CLOCK_SOURCE(reg)	(*(uint32_t*)reg) |= (1 << 2)	/* Set clock source as processor clk source */
#define TIMER_ENABLE_SYSTICK_CTR(reg)	(*(uint32_t*)reg) |= (1 << 0)	/* Enable systick counter */

/* Definition of stack variables */
#define STACK_INITIAL_XPSR_VALUE		0x01000000U
#define STACK_INITIAL_LR_VALUE			0xFFFFFFFDU


/********************************************************************************/
/********************************** VARIABLES ***********************************/
/********************************************************************************/

/********************************************************************************/
/****************************** FUNCTION DECLARATIONS ***************************/
/********************************************************************************/

/********************************************************************************/
/***************************** FUNCTION IMPLEMENTATIONS *************************/
/********************************************************************************/

#endif /* SYSTEMDEFINITIONS_H_ */
