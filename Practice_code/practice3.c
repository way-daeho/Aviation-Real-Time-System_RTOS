#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>

#define STACKSIZE 0
#define TASKMODE 0
#define TSIGPRI 97
#define TWAITPRI 95
#define SEM_INIT 0
#define SEM_MODE S_PRIO
#define COUNT 10

RT_TASK tWait_desc, tSignal_desc;
RT_SEM sem_desc;

int i, j, a;
int count = 0;
void tWaitTask()
{
	printf("tWaitTask Started\n");
	while (1)
	{
		printf("tWaitTask : working\ntWaitTask : Waiting for Semaphore...\n");

		if (rt_sem_p(&sem_desc, TM_INFINITE) == 0)
		{
			count--;
			printf("tWaitTask count : %d\n", count);
			for (i = 0; i < 10000; i++)
				for (j = 0; j < 1000; j++)
					;
		}
		printf("tWaitTask : get semaphore\n");
		for (i = 0; i < 10000; i++)
			for (j = 0; j < 1000; j++)
				;
	}
}

void tSignalTask()
{
	printf("tSignalTask Started\n");
	while (1)
	{
		printf("tSignalTask : Working\n");
		printf("tSignalTask : relese Semaphore\n");
		for (a = 0; a < COUNT; a++)
		{
			count++;
			printf("----tSignalTask count : %d\n", count);
			rt_sem_v(&sem_desc);
			for (i = 0; i < 10000; i++)
				for (j = 0; j < 1000; j++)
					;
		}
		printf("tSignalTask : relese complete \n");
		for (i = 0; i < 10000; i++)
			for (j = 0; j < 1000; j++)
				;
	}
}

int main()
{

	if (rt_task_create(&tWait_desc, "tWaitTask", STACKSIZE, TWAITPRI, TASKMODE))
	{
		printf("tWaitTask1 isn't created\n");
		return -1;
	}
	printf("tWaitTask1 Succesfully created\n");

	if (rt_task_create(&tSignal_desc, "tSignalTask", STACKSIZE, TSIGPRI, TASKMODE))
	{
		printf("tSignalTask isn't created\n");
		return -1;
	}
	printf("tSignalTask Succesfully created\n");

	// counting semapthre create
	if (rt_sem_create(&sem_desc, "SEMAPHORE", SEM_INIT, SEM_MODE))
	{
		printf("Semaphore isn't created\n");
		return -1;
	}
	printf("Semaphore is succesfully created\n");

	rt_task_start(&tWait_desc, &tWaitTask, NULL);
	rt_task_start(&tSignal_desc, &tSignalTask, NULL);
	pause();
	rt_sem_delete(&sem_desc);
	return 0;
}