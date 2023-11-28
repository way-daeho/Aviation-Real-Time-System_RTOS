#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<native/task.h>
#include<native/mutex.h>

#define STACK_SIZE 0
#define TASKMODE 0
#define TA_PRI 80
#define Sem_MODE SEMAPHORE_PRIO

RT_TASK tA_desc;
RT_MUTEX mutex;

int resource = 100;

int routine_a()
{
	printf("A routine approached! We need shared resources.\n");
	printf("Waiting to get Routione A mutex...\n");
	rt_mutex_acquire(&mutex, TM_INFINITE);
	printf("Earn a Routione A mutex! Start a routine..\nAccess to shared resources\n");
	printf("The value of the current shared resource : %d\n", resource);
	resource += 20;
	printf("Invoke Routine B\n");
	routine_b();
	printf("Routine A Mutex Return..!\n");
	rt_mutex_release(&mutex);
	printf("Mutex return successful! Exit Routine A.\n");

	return 0;
}

int routine_b()
{
	printf("B routine approached! We need shared resources.\n");
	printf("Waiting to get Routione B mutex...\n");
	rt_mutex_acquire(&mutex, TM_INFINITE);
	printf("Earn a Routione B mutex! Start a routine..\nAccess to shared resources\n");
	printf("The value of the current shared resource : %d\n", resource);
	resource += 20;
	printf("Routine B Mutex Return..!\n");
	rt_mutex_release(&mutex);
	printf("Mutex return successful! Exit Routine B.\n");

	return 0;
}

void tA()
{
	printf("tA start!\n");
	printf("Waiting to get mutex...\n");
	rt_mutex_acquire(&mutex, TM_INFINITE);
	printf("Earn a tA mutex! Start a routine..\nAccess to shared resources\n");
	printf("The value of the current shared resource : %d\n", resource);
	resource += 20;
	printf("tA Mutex Return..!\n");
	rt_mutex_release(&mutex);
	printf("Mutex return successful! Exit the program.\n");

	return 0;
}

int main()
{
	if (rt_task_create(&tA_desc, "tA", STACK_SIZE, tA_PRI, TASKMODE))
	{
		printf("tAC1 generation failed\n");
		exit(1);
	}

	printf("tAC1 generation succeeded!\n");

	if (rt_mutex_create(&mutex, "KEY"))
	{
		printf("Mutex generation failed\n");
		exit(1);
	}

	printf("Mutex generation succeeded\n");

	rt_task_start(&tA_desc, &tA, NULL);
	pause();
	rt_mutex_delete(&mutex);

	return 0;
}