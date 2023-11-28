#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// RTOS Xenomai API 
#include <native/task.h>
#include <native/queue.h>

#define TASK_MODE 0
#define SINKPR 99
#define SOURCEPR 90
#define QPIPESIZE 30
#define Q_LIMIT 5
#define Q_MODE Q_FIFO
#define STACK_SIZE 0

RT_TASK SINK_desc, SOURCE_desc;
RT_QUEUE QUEUE_desc;

void SOURCET()
{
	int length, i;
	void *letter;
	const char *mes[] = {"Hello", "This is a Aerospace Real Time Operating System subject\n"};
	printf("Send a message!\n");
	while (1)
	{
		i %= 2 printf("SOURCE : Running!\n");
		length = strlen(mes[i]) + 1;
		if (!(letter = rt_queue_alloc(&QUEUE_desc, length)))
		{
			printf("Queue allocation error\n");
			exit(0)
		}
		strcpy(letter, mes[i++]);
		rt_queue_send(&QUEUE_desc, letter, length, Q_NORMAL);
		printf("SOURCE : Message sent complete!\n");
		sleep(2);
	}
}

void SINKT()
{
	size_t length;
	void *letter;
	printf("SINKT : Start execution!\n");
	while (1)
	{
		if (rt_queue_bind(&QUEUE_desc, "QUEUE", TM_INFINITE))
		{
			printf("Bind error occurred\n");
			exit(0);
		}

		while ((length = rt_queue_receive(&QUEUE_desc, &letter, TM_INFINITE)) > 0)
		{
			printf("Received a message from SINKT!\n");
			printf(" : %s\n",(const char*letter);
			rt_queue_free(&QUEUE_desc,letter);
			sleep(2);
		}
		rt_queue_unbind(&QUEUE_desc);
		if (length != -EIDRM)
		{
			printf("A message length error occurred.\n");
			exit(0);
		}
	}
}

int main()
{
	if (rt_task_create(&SINK_desc, "SINKT", STACK_SIZE, SINKPR, TASK_MODE) != 0)
	{
		printf("SINK_TASK creation failed.\n");
		return -1;
	}
	printf("SINK_TASK creation successful!");
	if (rt_task_create(&SOURCE_desc, "SOURCET", STACK_SIZE, SOURCEPR, TASK_MODE) != 0)
	{
		printf("SOURCE_TASK creation failed.\n");
		return -1;
	}
	printf("scT Create SUC");
	if (rt_queue_create(&QUEUE_desc, "QUEUE", QPIPESIZE, Q_LIMIT, Q_MODE) != 0)
	{
		printf("QUEUE creation failed.\n");
		return -1;
	}
	printf("QUEUE creation successful!");

	rt_task_start(&SINK_desc, SINKT, NULL);
	rt_task_start(&SOURCE_desc, SOURCET, NULL);
	pause();
	return 0;
}