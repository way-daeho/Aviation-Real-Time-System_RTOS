#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>

#define STACK_SIZE 0
#define TASKMODE 0
#define SEMAPHORE_CT 2
#define SEMAPHORE_MODE S_PRIO
#define tAC1PRI 99
#define tAC2PRI 99
#define tAC3PRI 99

int resource1 = 100;
int resource2 = 100;

RT_TASK tAC1_desc, tAC2_desc, tAC3_desc;
RT_SEM sem_desc;

void tAC1()
{
	printf("tAC1! start!\n");

	while (1)
	{
		printf("tAC1 Shared Resource Access!\ntAC1 is waiting to be acquired as a MUTEX.\n");
		rt_sem_p(&sem_desc, TM_INFINITE);
		printf("tAC1 : Acquire a Semaphore!\n");
		printf("Shared Resource to Use -> Shared Resource 1, Value of Shared Resource 1: %d\nStart of use!\n", resource1);
		resource1 -= 10;
		printf("tAC1 Shared Resource Access!\nThe value of the current shared resource1 : %d\n", resource1);
		rt_sem_v(&Sem_desc);
		printf("Shared resources are used! Return tAC1 Semaphore!\n");
		sleep(2);
	}
}

void tAC2()
{
	printf("tAC2! start!\n");

	while (1)
	{
		printf("tAC2 Shared Resource Access!\ntAC2 is waiting to be acquired as a MUTEX.\n");
		rt_sem_p(&sem_desc, TM_INFINITE);
		printf("tAC2 : Acquire a Semaphore!\n");
		printf("Shared Resource to Use -> Shared Resource 2, Value of Shared Resource 2: %d\nStart of use!\n", resource1);
		resource2 -= 10;
		printf("tAC2 Shared Resource Access!\nThe value of the current shared resource2 : %d\n", resource2);
		rt_sem_v(&sem_desc);
		printf("Shared resources are used! Return tAC2 Semaphore!\n");
		sleep(2);
	}
}

void tAC3()
{
	printf("tAC3 start!\n");

	while (1)
	{
		printf("tAC3 Shared Resource Access!\ntAC3 is waiting to be acquired as a MUTEX.\n");
		rt_sem_p(&sem_desc, TM_INFINITE);
		printf("tAC3 : Acquire a Semaphore!\n");
		printf("Shared Resource to Use -> Shared Resource 1, Value of Shared Resource 1: %d\nStart of use!\n", resource1);
		resource1 -= 10;
		printf("tAC3 Shared Resource Access!\nThe value of the current shared resource1 : %d\n", resource1);
		rt_sem_v(&sem_desc);
		printf("Shared resources are used! Return tAC3 Semaphore!\n");
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

	if (rt_task_create(&t3_desc, "tAC3", STACK_SIZE, tAC3PRIO, TASKMODE))
	{
		printf("tAC3 generation failed\n");
		exit(1);
	}

	printf("tAC3 generation succeeded!\n");

	if (rt_sem_create(&Semaphore_desc, "semaphore", SEMAPHORE_CT, SEMAPHORE_MODE))
	{
		printf("Semaphore generation failed\n");
		exit(1);
	}

	printf("Semaphore generation succeeded\n");

	rt_task_start(&tAC1_desc, &tAC1, NULL);
	rt_task_start(&tAC2_desc, &tAC2, NULL);
	rt_task_start(&tAC3_desc, &tAC3, NULL);
	pause();
	rt_sem_delete(&Sem_desc);

	return 0;
}