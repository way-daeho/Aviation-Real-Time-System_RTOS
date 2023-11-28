#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<native/task.h>
#include<native/mutex.h>

#define STACK_SIZE 0
#define TASKMODE 0
#define tAC1PRI 99
#define tAC2PRI 99
#define tAC3PRI 99

int resource1 = 100;
int resource2 = 100;

RT_TASK tAC1_desc, tAC2_desc, tAC3_desc;
RT_MUTEX resource1_mutex, resource2_mutex;

void tAC1()
{
	printf("tAC1! start!\n");

	while (1)
	{
		printf("tAC1 Shared Resource Access!\ntAC1 is waiting to be acquired as a Resource1 MUTEX.\n");
		rt_mutex_acquire(&resource1_mutex, TM_INFINITE);
		printf("tAC1 : Acquire a mutex!\n");
		printf("Shared Resource to Use -> Shared Resource 1, Value of Shared Resource 1: %d\nStart of use!\n",resource1);
		resource1 -= 10;
		printf("tAC1 Shared Resource Access!\nThe value of the current shared resource1 : %d\n", resource1);
		rt_mutex_release(&resource1_mutex);
		printf("Shared resources are used! Return tAC1 Resource1 Mutex!\n");
		sleep(2);
	}
}

void tAC2()
{
	printf("tAC2! start!\n");

	while (1)
	{
		printf("tAC2 Shared Resource Access!\ntAC2 is waiting to be acquired as a Resource2 MUTEX.\n");
		rt_mutex_acquire(&resource2_mutex, TM_INFINITE);
		printf("tAC2 : Acquire a mutex!\n");
		printf("Shared Resource to Use -> Shared Resource 2, Value of Shared Resource 2: %d\nStart of use!\n",resource1);
		resource2 -= 10;
		printf("tAC2 Shared Resource Access!\nThe value of the current shared resource2 : %d\n", resource2);
		rt_mutex_release(&resource2_mutex);
		printf("Shared resources are used! Return tAC2 Resource2 Mutex!\n");
		sleep(2);
	}
}

void tAC3()
{
	printf("tAC3 start!\n");

	while (1)
	{
		printf("tAC3 Shared Resource Access!\ntAC3 is waiting to be acquired as a MUTEX.\n");
		rt_mutex_acquire(&resource1_mutex, TM_INFINITE);
		printf("tAC3 : Acquire a Resource1 Mutex!\n");
		printf("Shared Resource to Use -> Shared Resource 1, Value of Shared Resource 1: %d\nStart of use!\n",resource1);
		resource1 -= 10;
		printf("tAC3 Shared Resource Access!\nThe value of the current shared resource1 : %d\n", resource1);
		rt_mutex_release(&resource1_mutex);
		printf("Shared resources are used! Return tAC3 Resource1 Mutex!\n");
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

	if (rt_mutex_create(&resource1_mutex, "key1"))
	{
		printf("Resource1 Mutex generation failed\n");
		exit(1);
	}

		if (rt_mutex_create(&resource2_mutex, "key2"))
	{
		printf("Resource2 Mutex generation failed\n");
		exit(1);
	}

	printf("Mutex generation succeeded\n");

	rt_task_start(&t1_desc, &tAC1, NULL);
	rt_task_start(&t2_desc, &tAC2, NULL);
	rt_task_start(&t3_desc, &tAC3, NULL);
	pause();
	rt_mutex_delete(&resource1_mutex);
	rt_mutex_delete(&resource2_mutex);

	return 0;
}