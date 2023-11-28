#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>

#define STACK_SIZE 0
#define TASKMODE 0
#define Sem_CT 0
#define Sem_MODE S_PRIO
#define tSTPRIO 99
#define tWTPRIO 90

RT_TASK tS_desc, tW_desc;
RT_SEM sem_desc;

int count = 0;

void tS()
{
	printf("tSignal start!\n");
	while (1)
	{
		printf("Working...\n");
		rt_sem_v(&sem_desc);
		count++;
		printf("Return the semaphore : %d\n",count);
		sleep(1);
	}
}

void tW()
{
	printf("tWait start!\n");
	while (1)
	{
		printf("tWait wants to acquire a semaphore.\n");
		rt_sem_p(&sem_desc, TM_INFINITE);
		count--;
		printf("Semaphore Obtained! : %d\n", count);
		printf("working..\n");
		rt_sem_v(&sem_desc);
		count++;
		printf("Return the tWait's semaphore : %d\n", count);
		sleep(5);
	}
}

int main(void)
{
	if (rt_task_create(&tS_desc, "tS", STACK_SIZE, tSTPRIO, TASKMODE))
	{
		printf("tSignalTask has not been created.\n");
		exit(1);
	}

	printf("tSignalTask creation completed!\n");

	if (rt_task_create(&tW_desc, "tW", STACK_SIZE, tSTPRIO, TASKMODE))
	{
		printf("tWaitTask has not been created.\n");
		exit(1);
	}

	printf("tWaitTask creation completed!\n");

	if (rt_sem_create(&sem_desc, "semaphore", Sem_CT, Sem_MODE))
	{
		printf("Semaphore has not been created.\n");
		exit(1);
	}

	printf("Semaphore creation completed!\n");

	rt_task_start(&tS_desc, &tS, NULL);
	rt_task_start(&tW_desc, &tW, NULL);
	pause();
	rt_sem_delete(&sem_desc);

	return 0;
}