#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>

#define STACKSIZE 0
#define TASKMODE 0
#define SEM_INIT 1
#define SEM_MODE S_PRIO
#define ACC1PRI 99
#define ACC2PRI 98

RT_TASK tAccess1_desc, tAccess2_desc;
RT_SEM sem_desc;

int s_rs = 0;

void tAccessTask1()
{
	printf("tAccessTask1 Started\n");
	while (1)
	{
		printf("tAccessTask1 : start\ntAccessTask1 : wait for Semaphore\n");
		rt_sem_p(&sem_desc, TM_INFINITE);
		printf("tAccessTask1 : get semaphore.\n");
		s_rs += 10;
		printf("tAccessTask1 access complete, Shared Resource = %d\n", s_rs);
		rt_sem_v(&sem_desc);
		printf("tAccessTask1 released semaphore complete\n");
		sleep(3);
	}
}

void tAccessTask2()
{
	printf("tAccessTask2 Started\n");
	while (1)
	{
		printf("tAccessTask2 : start\ntAccessTask2 : wait for Semaphore\n");
		rt_sem_p(&sem_desc, TM_INFINITE);
		printf("tAcessTask2 : get semaphore\n");
		s_rs += 10000;
		printf("tAccessTask2 access complete, Shared Resource =%d\n", s_rs);
		rt_sem_v(&sem_desc);
		printf("tAccessTask2 released semaphore complete\n");
		sleep(8);
	}
}

int main()
{
	if (rt_task_create(&tAccess1_desc, "tAccessTask1", STACKSIZE, ACC1PRI, TASKMODE))
	{
		printf("tAccessTask1 isn't created\n");
		return -1;
	}
	printf("tAccessTask1 Succesfully created\n");

	if (rt_task_create(&tAccess2_desc, "tAccessTask2", STACKSIZE, ACC2PRI, TASKMODE))
	{
		printf("tAccessTask2 isn't created\n");
		return -1;
	}
	printf("tAccessTask2 Succesfully created\n");

	if (rt_sem_create(&sem_desc, "semaphore", SEM_INIT, SEM_MODE))
	{
		printf("Semaphore isn't created\n");
		return -1;
	}
	printf("Semaphore is succesfully created\n");

	rt_task_start(&tAccess1_desc, &tAccessTask1, NULL);
	rt_task_start(&tAccess2_desc, &tAccessTask2, NULL);
	pause(0);
	rt_sem_delete(&sem_desc);
	return 0;
}