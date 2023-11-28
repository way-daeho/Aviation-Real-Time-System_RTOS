#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>

#define STACK_SIZE 0
#define TASKMODE 0
#define SEMAPHORE_CT 1
#define SEMAPHORE_MODE S_PRIO
#define tAC1PRIO 99
#define tAC2PRIO 99

RT_TASK t1_desc, t2_desc;
RT_SEM Semaphore_desc;

int resource = 100;

void tAC1()
{
	printf("tAC1 start!\n");
	while (1)
	{
		printf("tAC1 Shared Resource Access!\ntAC1 is waiting to be acquired as a semaphore.\n");
		rt_sem_p(&Semaphore_desc, TM_INFINITE);
		printf("tAC1 : Acquire a semaphore!\n");
		resource -= 10;
		pritnf("Successful acquisition of tAC1 shared resources! Shared Resources : %d\n",resource);
		rt_sem_v(&Semaphore_desc);
		printf("tAC1 has returned the shared resource.\n");
		sleep(2);
	}
}

void tAC2()
{
	printf("tAC2 start!\n");
	while (1)
	{
		printf("tAC2 Shared Resource Access!\ntAC2 is waiting to be acquired as a semaphore.\n");
		rt_sem_p(&Semaphore_desc, TM_INFINITE);
		printf("tAC2 : Acquire a semaphore!\n");
		resource += 110;
		pritnf("Successful acquisition of tAC2 shared resources! Shared Resources : %d\n",resource);
		rt_sem_v(&Semaphore_desc);
		printf("tAC2 has returned the shared resource.\n");
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

	if (rt_sem_create(&Semaphore_desc, "semaphore", SEMAPHORE_CT, SEMAPHORE_MODE))
	{
		printf("Semaphore generation failed\n");
		exit(1);
	}

	printf("Semaphore generation succeeded\n");

	rt_task_start(&t1_desc, &tAC1, NULL);
	rt_task_start(&t2_desc, &tAC2, NULL);
	pause();
	rt_sem_delete(&Semaphore_desc);

	return 0;
}