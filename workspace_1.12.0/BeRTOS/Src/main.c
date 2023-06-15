/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 * @Author		   : Berat Erdemkilic
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/********************************************************************************/
/********************************** INCLUDES ************************************/
/********************************************************************************/
#include <stdio.h>
#include "main.h"

/********************************************************************************/
/********************************** DEFINITIONS *********************************/
/********************************************************************************/

/********************************************************************************/
/********************************** VARIABLES ***********************************/
/********************************************************************************/

/********************************************************************************/
/****************************** FUNCTION DECLARATIONS ***************************/
/********************************************************************************/

/********************************************************************************/
/***************************** FUNCTION IMPLEMENTATIONS *************************/
/********************************************************************************/
void UserTask_1(void)
{
	while(SYSTEM_STATE_TRUE)
	{
		printf("User task 1 is working..!");
	}
}
void UserTask_2(void)
{
	while(SYSTEM_STATE_TRUE)
	{
		printf("User task 2 is working..!");
	}
}
void UserTask_3(void)
{
	while(SYSTEM_STATE_TRUE)
	{
		printf("User task 3 is working..!");
	}
}
void UserTask_4(void)
{
	while(SYSTEM_STATE_TRUE)
	{
		printf("User task 4 is working..!");
	}
}

void initializeSystickTimer(void)
{
	uint32_t *pSRVR   = (uint32_t*)0xE000E014; /* SysTick Reload Value Register */
	uint32_t *pSCSR   = (uint32_t*)0xE000E010; /* SysTick Control and Status Register */
	uint32_t  counter = TIMER_DESIRED_FREQ - 1; /* Reload will create exception */

	/* Clear the value of reload register */
	TIMER_CLEAR_32BIT_REG(pSRVR);

	/* Load the counter into SVR */
	*pSRVR |= counter;

	/* Counter settings */
	TIMER_ENABLE_SYSTICK_EXC(pSCSR);
	TIMER_INDC_CLOCK_SOURCE(pSCSR);
	TIMER_ENABLE_SYSTICK_CTR(pSCSR);
}

__attribute__((naked)) void initializeStackOfSchedulerTask(uint32_t SchedulerStackEndAddr)
{
	/* Move stack value from general register (or from a variable in C as I amm doing) to special register */
	__asm volatile("MSR MSP, %0": : "r" (SchedulerStackEndAddr) : );

	/* Return from function call */
	__asm volatile ("BX LR");
}

void initializeStackOfUserTasks(void)
{
	SYSTEM_DEFINITIONS_NUM_OF_TASKS_ENUM  stackFrame1Counter;
	uint8_t  stackFrame2Counter;
	const uint8_t stackFrame2Registers = 13; /* R0 to R12 */
	uint32_t *pPSP;

	/* Null PSP at start */
	pPSP = NULL;

	/* Start from high end address (task 1) and go through the low end address, according to the stack structure
	 * (systemDefinitions.h), fill the user stack accordingly */
	for(stackFrame1Counter = TASK_1; stackFrame1Counter < TASK_TOTAL_NUM_OF_TASKS; ++stackFrame1Counter)
	{
		pPSP = (uint32_t*)UserTasksPspArray[stackFrame1Counter];

		/* Relocate PSP and process xPSR */
		pPSP--;
		*pPSP = STACK_INITIAL_XPSR_VALUE;

		/* Relocate PSP and process PC */
		pPSP--;
		*pPSP = UserTasksAddresses[stackFrame1Counter];

		/* Relocate PSP and process LR */
		pPSP--;
		*pPSP = STACK_INITIAL_LR_VALUE;

		for(stackFrame2Counter = 0; stackFrame2Counter < stackFrame2Registers; ++stackFrame2Counter)
		{
			pPSP--;
			*pPSP = 0;
		}

		/* Preserve the value of PSP */
		UserTasksPspArray[stackFrame1Counter] = (uint32_t)pPSP;
	}
}

void DetermineNextTask(void)
{
	switch (currentlyActiveTask)
	{
		case TASK_1:
		{
			currentlyActiveTask = TASK_2;
			break;
		}
		case TASK_2:
		{
			currentlyActiveTask = TASK_3;
			break;
		}
		case TASK_3:
		{
			currentlyActiveTask = TASK_4;
			break;
		}
		case TASK_4:
		{
			currentlyActiveTask = TASK_1;
			break;
		}
		default:
		{
			break;
		}
	}
}

void enable_processor_faults(void)
{
	uint32_t* const pSHCSR = (uint32_t*)0xE000ED24;

	/* Mem manage */
	*pSHCSR |= (1 << 16);
	/* Bus fault */
	*pSHCSR |= (1 << 17);
	/* Usage fault */
	*pSHCSR |= (1 << 18);
}

void HardFault_Handler(void)
{
	printf("HardFault_Handler()\n");
	while(1);
}
void MemManage_Handler(void)
{
	printf("MemManage_Handler()\n");
	while(1);
}
void BusFault_Handler(void)
{
	printf("BusFault_Handler()\n");
	while(1);
}

void setPspCurrentVaue(uint32_t currentPsp)
{
	UserTasksPspArray[currentlyActiveTask] = currentPsp;
}

uint32_t getPspOfCurrentTask(void)
{
	return UserTasksPspArray[currentlyActiveTask];
}

/* Systick timer handler; do the main scheduling operation */
__attribute__ ((naked)) void SysTick_Handler(void)
{
	/* Save the context of the current task */
		/* Get the PSP value of the currently RUNNING task */
	__asm volatile ("MRS R0,PSP");
		/* Using PSP, store SF2 (R4 - R11) */
	__asm volatile ("STMDB R0!,{R4-R11}");

	__asm volatile ("PUSH {LR}");
		/* Save the current value of PSP , R0 --> currentPsp */
	__asm volatile ("BL setPspCurrentVaue");

	/* Retrieve the context of the next task */
		/* Determine the next task - Linking */
	__asm volatile ("BL DetermineNextTask");
		/* Get the PSP value of this task */
	__asm volatile ("BL getPspOfCurrentTask");
		/* Retrieve SF2 of the current task which is the switched one */
	__asm volatile ("LDMIA R0!,{R4-R11}");
		/* Update PSP and time to exit */
	__asm volatile ("MSR PSP,R0");

	__asm volatile ("POP {LR}");
	__asm volatile ("BX LR");

}

__attribute__ ((naked)) void switchSpToPsp(void)
{
	/* Initialize the PSP with TASK1 (current task, task1 is the default one) stack start address */
		/* Save LR before corruption which connects back to main */
	__asm volatile ("PUSH {LR}");

		/* Take UserTasksPspArray[currentlyActiveTask] into R0 */
	__asm volatile ("BL getPspOfCurrentTask");

		/* Set PSP */
	__asm volatile ("MSR PSP,R0");

		/* Store LR back */
	__asm volatile ("POP {LR}");

	/* Change SP to PSP via the Control register*/
	__asm volatile ("MOV R0, #0X02");
	__asm volatile ("MSR CONTROL,R0");

	/* Back to main() */
	__asm volatile ("BX LR");
}

int main(void)
{
	enable_processor_faults();

	/* Initialize private stack of scheduler task */
	initializeStackOfSchedulerTask(STACK_TS_SS);

	/* Initialize user tasks address array */
	UserTasksAddresses[TASK_1] = (uint32_t)UserTask_1;
	UserTasksAddresses[TASK_2] = (uint32_t)UserTask_2;
	UserTasksAddresses[TASK_3] = (uint32_t)UserTask_3;
	UserTasksAddresses[TASK_4] = (uint32_t)UserTask_4;

	/* Initialize private stack of user tasks */
	initializeStackOfUserTasks();

    /* Initialize SysTick timer */
	initializeSystickTimer();

	/* Switch Stack Pointer to PSP for user thread runs */
	switchSpToPsp();

	/* Start the first task */
	UserTask_1();

	for(;;);
}
