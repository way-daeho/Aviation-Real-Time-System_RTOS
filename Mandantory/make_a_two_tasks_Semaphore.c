#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h> // Xenomai API (Use a task)
#include <native/sem.h>	 // Xenomai API (Use a Semaphore)

#define STACK_SIZE 0
#define TAKSMODE 0
#define T_SIGNAL_PRI 90 // 숫자가 높을 수록, 우선 순위가 높다.
#define T_WAIT_PRI 99

RT_TASK T_WAIT_desc, T_SIGNAL_desc;

void TWAITTask()
{
	printf("TWAITSIGNAL start!\n");
	while (1)
	{
		printf("TWAITSIGNAL_TASK : working..\n");
		sleep(2);
	}
}

void TSIGNALTask()
{
	printf("TSIGANAL_TASK start!\n");
	while (1)
	{
		printf("TSIGNAL_TASK : working..\n");
		sleep(2);
	}
}

int main(void)
{

	if (rt_task_create(T_WAIT_desc, "TWAITTask", STACK_SIZE, T_WAIT_PRI, TASKMODE))
	{
		printf("T_WAIT_SIGNAL_TASK generation failed.\n");
		return -1;
	}

	printf("T_WAIT_TASK was created!\n");

	if (rt_task_create(&T_SIGNAL_desc, "TSIGNALTask", STACK_SIZE, T_SIGNAL_PRI, TASKMODE))
	{
		printf("T_SIGNAL_TASK generation failed.\n");
		return -1;
	}

	printf("T_SIGNAL_TASK was created!\n");

	rt_task_start(&T_SIGNAL_desc, &TSIGNALTask, NULL);
	rt_task_start(&T_WAIT_desc, &TWAITTask, NULL);
	pause();

	return 0;
}