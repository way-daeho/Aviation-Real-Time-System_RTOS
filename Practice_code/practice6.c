#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/types.h>
#include <native/mutex.h>

#define STACKSIZE 0
#define TASKMODE 0

#define SEM_INIT 1
#define SEM_MODE S_PRIO

#define FPRI 99
#define SPRI 95
#define TPRI 90

RT_TASK tAccess1_desc, tAccess2_desc, tAccess3_desc;

RT_MUTEX mux1_desc, mux2_desc;

int s_rs = 0;
int i, j;

void tAccess1Task()
{
	printf("tAccessTask Started\n");
	while (1)
	{
		printf("tAccess1Task : start\ntAccessTask : wait for Mutex\n");
		if (rt_mutex_acquire(&mux1_desc, TM_NONBLOCK) != 0)
		{
			rt_mutex_acquire(&mux2_desc, TM_INFINITE);
			printf("tAccess1Task : get Mutex2.\n");
			s_rs += 10;
			printf("tAccess1Task access complete, Shared Resource = %d\n", s_rs);
			sleep(4);
			rt_mutex_release(&mux2_desc);
		}
		else
		{
			printf("tAccess1Task : get Mutex1.\n");
			s_rs += 10;
			printf("tAccess1Task access complete, Shared Resource = %d\n", s_rs);
			sleep(4);
			rt_mutex_release(&mux1_desc);
		}
		printf("tAccess1Task released Mutex complete\n");
		sleep(1);
	}
}
void tAccess2Task()
{
	printf("tAccess2Task Started\n");
	while (1)
	{
		printf("tAccess2Task : start\ntAccessTask : wait for Mutex\n");
		if (rt_mutex_acquire(&mux1_desc, TM_NONBLOCK) != 0)
		{
			rt_mutex_acquire(&mux2_desc, TM_INFINITE);
			printf("tAccess2Task : get Mutex2.\n");
			s_rs += 100;
			printf("tAccess2Task access complete, Shared Resource = %d\n", s_rs);
			sleep(5);
			rt_mutex_release(&mux2_desc);
		}
		else
		{
			printf("tAccess2Task : get Mutex1.\n");
			s_rs += 100;
			printf("tAccess2Task access complete, Shared Resource = %d\n", s_rs);
			sleep(5);
			rt_mutex_release(&mux1_desc);
		}
		printf("tAccess2Task released Mutex complete\n");
		sleep(1);
	}
}
void tAccess3Task()
{
	printf("tAccess3Task Started\n");
	while (1)
	{
		printf("tAccess3Task : start\ntAccessTask : wait for Mutex\n");
		if (rt_mutex_acquire(&mux1_desc, TM_NONBLOCK) != 0)
		{
			rt_mutex_acquire(&mux2_desc, TM_INFINITE);
			printf("tAccess3Task : get Mutex2.\n");
			s_rs += 1000;
			printf("tAccess3Task access complete, Shared Resource = %d\n", s_rs);
			sleep(5);
			rt_mutex_release(&mux2_desc);
		}
		else
		{
			printf("tAccess3Task : get Mutex1.\n");
			s_rs += 1000;
			printf("tAccess3Task access complete, Shared Resource = %d\n", s_rs);
			sleep(5);
			rt_mutex_release(&mux1_desc);
		}
		printf("tAccess3Task released Mutex complete\n");
		sleep(1);
	}
}

int main()
{
	if (rt_task_create(&tAccess1_desc, "tAccessTask1", STACKSIZE, FPRI, TASKMODE))
	{
		printf("tAccess1Task isn't created\n");
		return -1;
	}
	printf("tAccess1Task Succesfully created\n");

	if (rt_task_create(&tAccess2_desc, "tAccessTask3", STACKSIZE, SPRI, TASKMODE))
	{
		printf("tAccess2Task isn't created\n");
		return -1;
	}
	printf("tAccess2Task Succesfully created\n");

	if (rt_task_create(&tAccess3_desc, "tAccess3Task", STACKSIZE, TPRI, TASKMODE))
	{
		printf("tAccess3Task isn't created\n");
		return -1;
	}
	printf("tAccess3Task Succesfully created\n");

	if (rt_mutex_create(&mux1_desc, "mutex1"))
	{
		printf("Mutex1 isn't created\n");
		return -1;
	}
	printf("Mutex1 is succesfully created\n");

	if (rt_mutex_create(&mux2_desc, "mutex2"))
	{
		printf("Mutex2 isn't created\n");
		return -1;
	}
	printf("Mutex2 is succesfully created\n");

	rt_task_start(&tAccess1_desc, &tAccess1Task, NULL);
	rt_task_start(&tAccess2_desc, &tAccess2Task, NULL);
	rt_task_start(&tAccess3_desc, &tAccess3Task, NULL);
	pause(0);
	rt_mutex_delete(&mux1_desc);
	rt_mutex_delete(&mux2_desc);
	return 0;
}