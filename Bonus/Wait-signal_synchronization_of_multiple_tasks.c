#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>

#define STACK_SIZE 0
#define TASKMODE 0
#define T_SIGANAL_PRI 40
#define T_WAIT_PRI1 99
#define T_WAIT_PRI1 98
#define T_WAIT_PRI1 97

#define BINARYSEM 0
#define SEMAPHORE_MODE __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION

RT_TASK T_WAIT_desc1, T_WAIT_desc2, T_WAIT_desc3, T_SIGNAL_desc;
RT_SEM SEMAPHORE_desc;
int i, j;

void TWAITTask1()
{
	printf("TWAITTASK1 start!\n");
	while (1)
	{
		printf("TWAITTASK1 : Currently waiting for semaphore..\n");
		sleep(2);
		rt_sem_p(&SEMAPHORE_desc, TM_INFINITE);
		printf("TWAITTASK1 : Semaphore Obtained!\nTWAITTASK1 have completed this task.");
		sleep(5);
	}
}

void TWAITTask2()
{
	printf("TWAITTASK1 start!\n");
	while (1)
	{
		printf("TWAITTASK2 : Currently waiting for semaphore..\n");
		sleep(2);
		rt_sem_p(&SEMAPHORE_desc, TM_INFINITE);
		printf("TWAITTASK2 : Semaphore Obtained!\nTWAITTASK2 have completed this task.");
		sleep(5);
	}
}

void TWAITTask3()
{
	printf("TWAITTASK3 start!\n");
	while (1)
	{
		printf("TWAITTASK3 : Currently waiting for semaphore..\n");
		sleep(2);
		rt_sem_p(&SEMAPHORE_desc, TM_INFINITE);
		printf("TWAITTASK3 : Semaphore Obtained!\nTWAITTASK3 have completed this task.");
		sleep(5);
	}
}

void TSIGNALTask()
{
	printf("TSIgNAL Task start!\n");
	while (1)
	{
		pritnf("TSIGNAL Task : Now working..\nTSIGNAL Task : Now return the semaphore.\n");
		sleep(2);
		rt_sem_broadcast(&SEMAPHORE_desc);
		printf("TSIGNAL Task : Returns a semaphore.\n");
	}
}

int main(void)
{
	if (rt_task_create(&T_WAIT_desc1, "TWAITTask1", STACK_SIZE, T_WAIT_PRI1, TASKMODE))
	{
		printf("TWAITTASK1 failed to generate.\n");
		exit(1);
	}
	printf("TWAITTASK2 has been created!\n");

	if (rt_task_create(&T_WAIT_desc3, "TWAITTask3", STACK_SIZE, T_WAIT_PRI3, TASKMODE))
	{
		printf("TWAITTASK3 failed to generate.\n");
		exit(1);
	}
	printf("TWAITTASK3 has been created!\n");

	if (rt_task_create(&T_SIGNAL_desc, "TSIGNALTask", STACK_SIZE, T_SIGNAL_PRI, TASKMODE))
	{
		printf("TSIGNALTASK failed to generate.\n");
		exit(1);
	}
	printf("TSIGNALTASK has been created!\n");

	if (rt_sem_create(&SEMAPHORE_desc, "SEMAPHORE", BINARYSEM, SEMAPHORE_MODE))
	{
		printf("SEMAPHORE failed to generate.\n");
		exit(1);
	}
	printf("SEMAPHORE has been created!\n");

	rt_task_start(&T_WAIT_desc1, &TWAITTask1, NULL);
	rt_task_start(&T_WAIT_desc2, &TWAITTask2, NULL);
	rt_task_start(&T_WAIT_desc3, &TWAITTask3, NULL);
	rt_task_start(&T_SIGNAL_desc, &TSIGNALTask, NULL);
	pause();
	rt_sem_delete(&SEMAPHORE_desc);
	return 0;
}