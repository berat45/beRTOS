#ifndef MAIN_H_
#define MAIN_H_
/*
 * main.h
 *
 *  Created on: Jun 13, 2023
 *      Author: Berat Erdemkilic
 */

/********************************************************************************/
/********************************** INCLUDES ************************************/
/********************************************************************************/
#include "systemDefinitions.h"
#include <stdint.h>

/********************************************************************************/
/********************************** DEFINITIONS *********************************/
/********************************************************************************/
/* PSP Storage array for all user tasks */
uint32_t UserTasksPspArray[TASK_TOTAL_NUM_OF_TASKS]  = {STACK_T1_SS, STACK_T2_SS, STACK_T3_SS, STACK_T4_SS};

/* Address values of the user tasks */
uint32_t UserTasksAddresses[TASK_TOTAL_NUM_OF_TASKS];

/* ID of the task which is in RUNNING state, default task_1 */
SYSTEM_DEFINITIONS_NUM_OF_TASKS_ENUM currentlyActiveTask = TASK_1;

/********************************************************************************/
/********************************** VARIABLES ***********************************/
/********************************************************************************/

/********************************************************************************/
/****************************** FUNCTION DECLARATIONS ***************************/
/********************************************************************************/
/* Inputs: Void
 * Return value: Void
 * Additional Info: Dummy user task to schedule */
void UserTask_1(void);
/* Inputs: Void
 * Return value: Void
 * Additional Info: Dummy user task to schedule */
void UserTask_2(void);
/* Inputs: Void
 * Return value: Void
 * Additional Info: Dummy user task to schedule */
void UserTask_3(void);
/* Inputs: Void
 * Return value: Void
 * Additional Info: Dummy user task to schedule */
void UserTask_4(void);
/* Inputs: Void
 * Return value: Void
 * Additional Info: Initialize SysTick timer strictly for 1ms */
void initializeSystickTimer(void);
/* Inputs: Stack end address of the scheduler task
 * Return value: Void
 * Additional Info: Load scheduler stack address and return to main function */
__attribute__((naked)) void initializeStackOfSchedulerTask(uint32_t);
/* Inputs: Void
 * Return value: Void
 * Additional Info: According to the stack structure (systemDefinitions.h),
 * initialize the stack of user tasks */
void initializeStackOfUserTasks(void);
/* Inputs: Void
 * Return value: Void
 * Additional Info: Enable processor faults */
void enable_processor_faults(void);
/* Inputs: Void
 * Return value: Void
 * Additional Info: Set SP to PSP to before running user tasks */
void switchSpToPsp(void);
/* Inputs: Void
 * Return value: PSP
 * Additional Info: Returns the stack pointer value of the task which is running on CPU */
uint32_t getPspOfCurrentTask(void);
/* Inputs: Void
 * Return value: PSP
 * Additional Info: Update the PSP value of the related task */
void setPspCurrentVaue(uint32_t);

/* Inputs: Void
 * Return value: void
 * Additional Info: Switch to next task - State machine manner to provide Round Robin fashion */
void DetermineNextTask(void);

/********************************************************************************/
/***************************** FUNCTION IMPLEMENTATIONS *************************/
/********************************************************************************/

#endif /* MAIN_H_ */
