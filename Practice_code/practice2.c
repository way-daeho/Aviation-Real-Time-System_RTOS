#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>

#define SSIZE 0
#define TMODE 0
#define TSPRI 50
#define TWPRI 99

#define BISEM 0
#define SMODE S_PRIO

RT_TASK tW_desc, tS_desc;
RT_SEM sM_desc;
int i, j;

void tWTask()
{
	printf("tWTask Started\n");
	while (1)
	{
		for (i = 0; i < 1000; i++)
			for (j = 0; j < 1000; j++)
				;
		printf("tWTask : working\ntWTask : Waiting Semaphore Now\n");
		rt_sem_p(&sM_desc, TM_INFINITE);
		printf("tWTask : get semaphore-> countinue work.\n");
		for (i = 0; i < 1000; i++)
		{
			for (j = 0; j < 1000; j++)
			{
				;
			}
		}
	}
}

void tSTask()
{
	printf("tSTask Started\n");
	while (1)
	{
		for (i = 0; i < 1000; i++)
			for (j = 0; j < 1000; j++)
				;
		printf("tSTask : Working\ntSTask : relese Semaphore Now\n");
		rt_sem_v(&sM_desc);
		printf("tSignalTask : release -> continue work.\n");
	}
}

int main()
{

	if (rt_task_create(&tW_desc, "tWaitTask", SSIZE, TWPRI, TMODE))
	{
		printf("tWaitTask1 isn't created\n");
		return -1;
	}
	printf("tWaitTask1 Succesfully created\n");

	if (rt_task_create(&tS_desc, "tSignalTask", SSIZE, TSPRI, TMODE))
	{
		printf("tSignalTask isn't created\n");
		return -1;
	}
	printf("tSignalTask Succesfully created\n");

	if (rt_sem_create(&sM_desc, "SEMAPHORE", BISEM, SMODE))
	{
		printf("Semaphore isn't created\n");
		return -1;
	}
	printf("Semaphore is succesfully created\n");

	rt_task_start(&tW_desc, &tWTask, NULL);
	rt_task_start(&tS_desc, &tSTask, NULL);
	pause();
	rt_sem_delete(&sM_desc);
	return 0;
}
