#include <stdio.h>
#include <sys/mman.h>
// RTOS : Xenomai API
#include <native/task.h>
#include <native/sem.h>

#define STACK_SIZE 0
#define TASKMODE 0
#define T_SIGNAL_PRI 40
#define T_WAIT_PRI 99

#define BINARYSEM 0
#define SEMAPHORE_MODE SEMAPHORE_PRIO

RT_TASK T_WAIT_desc, T_SIGNAL_desc;
RT_SEM SEMAPHORE_desc;
int i, j;

void TWAITTask()
{
	printf("TWAITSIGNAL start!\n");
	while (1)
	{
		for (i = 0; i < 1000; i++)
			for (j = 0; j < 1000; j++)
				;
		printf("TWAITTask : working..\nTWAITTask : Currently waiting for semaphore..\n");
		rt_sem_p(&SEMAPHORE_desc, TM_INFINITE);
		printf("TWAITTASK : Acquire a semaphore! We will continue to run it.\n");
		for (i = 0; i < 1000; i++)
		{
			for (j = 0; j < 1000; j++)
			{
				;
			}
		}
	}
}

void TSIGNALTask()
{
	printf("TSIGNALTask start!\n");
	while (1)
	{
		for (i = 0; i < 1000; i++)
			for (j = 0; j < 1000; j++)
				;
		printf("TSIGNALTask : working..\nTSIGNALTask : Now it returns a semaphore.\n");
		rt_sem_v(&SEMAPHORE_desc);
		printf("TSIGNALTask : Return semaphore! I will continue to work.\n");
	}
}

int main()
{

	if (rt_task_create(&T_WAIT_desc, "TWAITTask", STACK_SIZE, T_WAIT_PRI, TASKMODE))
	{
		printf("TWAITTASK was not created.\n");
		return -1;
	}
	printf("TTWAITTASK has been created!\n");

	if (rt_task_create(T_SIGNAL_desc, "TSIGNALTask", STACK_SIZE, T_SIGNAL_PRI, TASKMODE))
	{
		printf("TSIGANLTASK was not created.\n");
		return -1;
	}
	printf("TSIGNALTASK has been created!\n");

	if (rt_sem_create(&SEMAPHORE_desc, "SEMAPHORE", BINARYSEM, SEMAPHORE_MODE))
	{
		printf("The semaphore was not created.\n");
		return -1;
	}
	printf("The semaphore has been created!\n");

	rt_task_start(&T_WAIT_desc, &TWAITTask, NULL);
	rt_task_start(&T_SIGNAL_desc, &TSIGNALTask, NULL);
	pause();
	rt_sem_delete(&SEMAPHORE_desc);
	return 0;
}