#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/mutex.h>

#define STACK_SIZE 0
#define TASKMODE 0
#define tAC1PRIO 99
#define tAC2PRIO 99

RT_TASK t1_desc, t2_desc;
RT_MUTEX mutex;

int resource = 100;

void tAC1()
{
	printf("tAC1 start!\n");
	while (1)
	{
		printf("tAC1 Shared Resource Access!\ntAC1 is waiting to be acquired as a MUTEX.\n");
		rt_mutex_acquire(&mutex, TM_INFINITE);
		printf("tAC1 : Acquire a Mutex!\n");
		pritnf("The value of the shared resource after tAC1 is used : %d\nStart of use!\n",resource);
		resource -= 10;
		rt_mutex_release(&mutex);
		printf("tAC1 Mutex Return..\n");
		sleep(2);
	}
}

void tAC2()
{
	printf("tAC2 start!\n");
while (1)
	{
		printf("tAC2 Shared Resource Access!\ntAC2 is waiting to be acquired as a MUTEX.\n");
		rt_mutex_acquire(&mutex, TM_INFINITE);
		printf("tAC2 : Acquire a Mutex!\n");
		pritnf("The value of the shared resource after tAC2 is used : %d\nStart of use!\n",resource);
		resource -= 30;
		rt_mutex_release(&mutex);
		printf("tAC2 Mutex Return..\n");
		sleep(2);
	}
}

int main()
{
	if (rt_task_create(&t1_desc, "tAC1", STACK_SIZE, tAC1PRIO, TASKMODE))
	{
		printf("tAC1 generation failed\n");
		exit(1);
	}

	printf("tAC1 generation succeeded!\n");

	if (rt_task_create(&t2_desc, "tAC2", STACK_SIZE, tAC2PRIO, TASKMODE))
	{
		printf("tAC2 generation failed\n");
		exit(1);
	}

	printf("tAC2 generation succeeded!\n");

	if (rt_mutex_create(&mutex, "key"))
	{
		printf("Mutex generation failed\n");
		exit(1);
	}

	printf("Mutex generation succeeded\n");

	rt_task_start(&t1_desc, &tAC1, NULL);
	rt_task_start(&t2_desc, &tAC2, NULL);
	pause();
	rt_mutex_delete(&mutex);

	return 0;
}