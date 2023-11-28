#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>

#define SSIZE 0
#define TMODE 0
#define TSPRI 90
#define TWPRI 99

RT_TASK tW_desc, tS_desc;

void tWTask()
{
	printf("tWaitTask Started\n");
	while (1)
	{
		printf("tWaitTask : working\n");
		sleep(3);
	}
}

void tSTask()
{
	printf("tSignalTask Started\n");
	while (1)
	{
		printf("tSignalTask : Working\n");
		sleep(3);
	}
}

int main()
{

	if (rt_task_create(&tW_desc, "tWTask", SSIZE, TWPRI, TMODE))
	{
		printf("tWaitTask create fail\n");
		return -1;
	}
	printf("tWaitTask create SUC\n");

	if (rt_task_create(&tS_desc, "tSTask", SSIZE, TSPRI, TMODE))
	{
		printf("tSignalTask create fail\n");
		return -1;
	}

	printf("tSignalTask create SUC\n");

	rt_task_start(&tS_desc, &tSTask, NULL);
	rt_task_start(&tW_desc, &tWTask, NULL);
	pause();

	return 0;
}
