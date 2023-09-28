// File: TwoTasks.c

#include <stdio.h>
#include "includes.h"
#include <string.h>

#define DEBUG 1
#define SEM   1

/* Definition of Task Stacks */
/* Stack grows from HIGH to LOW memory */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    stat_stk[TASK_STACKSIZE];

OS_EVENT* semaphore1;
OS_EVENT* semaphore2;

/* Definition of Task Priorities */
#define TASK1_PRIORITY      6  // highest priority
#define TASK2_PRIORITY      7
#define TASK_STAT_PRIORITY 12  // lowest priority

void printStackSize(char* name, INT8U prio)
{
  INT8U err;
  OS_STK_DATA stk_data;

  err = OSTaskStkChk(prio, &stk_data);
  if (err == OS_NO_ERR) {
    if (DEBUG == 1)
      printf("%s (priority %d) - Used: %d; Free: %d\n",
	     name, prio, stk_data.OSUsed, stk_data.OSFree);
  }
  else
    {
      if (DEBUG == 1)
	printf("Stack Check Error!\n");
    }
}

/* Prints a message and sleeps for given time interval */
void task1(void* pdata)
{
  INT8U err;
  char state = '0';
  while (1)
    {
      printf("Task 1 - State %c \n", state);

      OSSemPost(semaphore2); // Semaphore is signaled
      PERF_RESET( PERFORMANCE_COUNTER_BASE );   //reset of the counter

      PERF_START_MEASURING( PERFORMANCE_COUNTER_BASE );
  //     char text1[] = "Task 0 - State ";
  //     int i;

  //     for (i = 0; i < strlen(text1); i++)
	// putchar(text1[i]);

  // putchar(state);
  //       putchar('\n');
  //       OSSemPost(semaphore2); // Semaphore is signaled

        OSSemPend(semaphore1, 0, &err); // Semaphore is waiting

  state = (state=='0')?'1':'0';


      OSTimeDlyHMSM(0, 0, 0, 11); /* Context Switch to next task
				   * Task will go to the ready state
				   * after the specified delay
				   */
    }
}

/* Prints a message and sleeps for given time interval */
void task2(void* pdata)
{
  INT8U err;
    char state = '0';
     int clock_cycles = 0;
    long double ContextSwitch_Seconds = 0;

    int count = 0;
    long double ContextSwitchAverage = 0;
    long double ContextSwitchAccumulator=0;
  while (1)
    {
      OSSemPend(semaphore2, 0, &err); // semaphore is waiting

      PERF_STOP_MEASURING( PERFORMANCE_COUNTER_BASE ); //stop the counter when the semaphores say to start next task

      clock_cycles = perf_get_total_time( (void *) PERFORMANCE_COUNTER_BASE ); // alt_u64 number;
      ContextSwitch_Seconds  = (long double) clock_cycles/ 50 ;

      if(ContextSwitchAverage == 0 || ContextSwitch_Seconds < ContextSwitchAverage*1.5)
      {
        ContextSwitchAccumulator = ContextSwitchAccumulator + ContextSwitch_Seconds;
        count = count + 1;
        ContextSwitchAverage = ContextSwitchAccumulator/(double) count;
      }

      printf( "total time Context Switch: %Lf 1e-6 seconds; \n", ContextSwitch_Seconds);

      if(count >= 11)
        printf( "total time Context Switch, AVARAGE: %Lf 1e-6 seconds; \n", ContextSwitchAverage);

      printf("Task 1 - state %c \n",state);
  //     OSSemPend(semaphore2, 0, &err);
  //     char text2[] = "Task 2 - State ";
  //     int i;

  //     for (i = 0; i < strlen(text2); i++)
	// putchar(text2[i]);
  //  putchar(state);
  //       putchar('\n');
      state = (state=='0')?'1':'0';
      OSSemPost(semaphore1);
      OSTimeDlyHMSM(0, 0, 0, 4);
    }
}

/* Printing Statistics */
void statisticTask(void* pdata)
{
  while(1)
    {
      printStackSize("Task1", TASK1_PRIORITY);
      printStackSize("Task2", TASK2_PRIORITY);
      printStackSize("StatisticTask", TASK_STAT_PRIORITY);
    }
}

/* The main function creates two task and starts multi-tasking */
int main(void)
{
  if (SEM == 1){
    semaphore1 = OSSemCreate(1);
    semaphore2 = OSSemCreate(1);
  }
  printf("Lab 3 - Two Tasks\n");

  OSTaskCreateExt
    ( task1,                        // Pointer to task code
      NULL,                         // Pointer to argument passed to task
      &task1_stk[TASK_STACKSIZE-1], // Pointer to top of task stack
      TASK1_PRIORITY,               // Desired Task priority
      TASK1_PRIORITY,               // Task ID
      &task1_stk[0],                // Pointer to bottom of task stack
      TASK_STACKSIZE,               // Stacksize
      NULL,                         // Pointer to user supplied memory (not needed)
      OS_TASK_OPT_STK_CHK |         // Stack Checking enabled
      OS_TASK_OPT_STK_CLR           // Stack Cleared
      );

  OSTaskCreateExt
    ( task2,                        // Pointer to task code
      NULL,                         // Pointer to argument passed to task
      &task2_stk[TASK_STACKSIZE-1], // Pointer to top of task stack
      TASK2_PRIORITY,               // Desired Task priority
      TASK2_PRIORITY,               // Task ID
      &task2_stk[0],                // Pointer to bottom of task stack
      TASK_STACKSIZE,               // Stacksize
      NULL,                         // Pointer to user supplied memory (not needed)
      OS_TASK_OPT_STK_CHK |         // Stack Checking enabled
      OS_TASK_OPT_STK_CLR           // Stack Cleared
      );

  if (DEBUG == 1)
    {
      OSTaskCreateExt
	( statisticTask,                // Pointer to task code
	  NULL,                         // Pointer to argument passed to task
	  &stat_stk[TASK_STACKSIZE-1],  // Pointer to top of task stack
	  TASK_STAT_PRIORITY,           // Desired Task priority
	  TASK_STAT_PRIORITY,           // Task ID
	  &stat_stk[0],                 // Pointer to bottom of task stack
	  TASK_STACKSIZE,               // Stacksize
	  NULL,                         // Pointer to user supplied memory (not needed)
	  OS_TASK_OPT_STK_CHK |         // Stack Checking enabled
	  OS_TASK_OPT_STK_CLR           // Stack Cleared
	  );
    }

  OSStart();
  return 0;
}
