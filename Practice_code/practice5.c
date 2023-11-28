#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/sem.h>

#define STACKSIZE 0
#define TASKMODE 0
#define SEM_INIT 1
#define SEM_MODE S_PRIO
#define ACCPRI 99
#define RoAPRI 98
#define RoBPRI 97
RT_TASK tAccess1_desc, tAccess2_desc;
RT_SEM sem_desc;
RT_MUTEX mux_desc;

int s_rs = 0;

void tAccessTask()
{
	printf("tAccessTask Started\n");
	while (1)
	{
		printf("tAccessTask : start\ntAccessTask : wait for Mutex\n");
		rt_sem_p(&sem_desc, TM_INFINITE);
		printf("tAccessTask : get Mutex.\n");
		s_rs += 10;
		printf("tAccessTask access complete, Shared Resource = %d\n", s_rs);
		rt_sem_v(&sem_desc);
		printf("tAccessTask1 released Mutex complete\n");
		sleep(3);
	}
}

void Routine A
{
	printf("Routine A  Started\n");
	while (1)
	{
		printf("Routine A  : start\nRoutine A : wait for Mutex\n");
		rt_sem_p(&sem_desc, TM_INFINITE);
		printf("Routine A : get Mutex\n");
		s_rs += 10000;
		printf("Routine A  access complete, Shared Resource =%d\n", s_rs);
		rt_sem_v(&sem_desc);
		printf("Routine A  released Mutex complete\n");
		sleep(8);
	}
}

void Routine B
{
	printf("Routine B  Started\n");
	while (1)
	{
		printf("Routine B  : start\nRoutine A : wait for Mutex\n");
		rt_sem_p(&sem_desc, TM_INFINITE);
		printf("Routine B : get Mutex\n");
		s_rs += 10000;
		printf("Routine B  access complete, Shared Resource =%d\n", s_rs);
		rt_sem_v(&sem_desc);
		printf("Routine B  released Mutex complete\n");
		sleep(8);
	}
}

int main()
{
	if (rt_mutex_create(&tAccess1_desc, "tAccessTask1", STACKSIZE, ACC1PRI, TASKMODE))
	{
		printf("tAccessTask1 isn't created\n");
		return -1;
	}
	printf("tAccessTask1 Succesfully created\n");

	if (rt_mutex_create(&tAccess2_desc, "tAccessTask2", STACKSIZE, ACC2PRI, TASKMODE))
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