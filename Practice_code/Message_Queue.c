#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <native/task.h>
#include <native/queue.h>

#define TASKMODE 0
#define SNPR 99
#define SOPR 90
#define QPSIZE 30
#define QLMT 5
#define QMD Q_FIFO
#define STACKSIZE 0

RT_TASK sk_desc, sc_desc;
RT_QUEUE que;

void tSoT()
{
	int len, i;
	void *mes;
	const char *message[] = {"Hi", "This is 201801000\n"};
	printf("send start!\n");
	while (1)
	{
		i %= 2;
		printf("Sot:Working!\n");
		len = strlen(message[i]) + 1;
		if (!(mes = rt_queue_alloc(&que, len)))
		{
			printf("Alloc Error\n");
			exit(0);
		}
		strcpy(mes, message[i++]);
		rt_queue_send(&que, mes, len, Q_NORMAL);
		printf("SOT:send Complete\n");
		sleep(1);
	}
}

void tSiT()
{
	size_t len;
	void *mes;
	printf("SiT start\n");
	while (1)
	{
		if (rt_queue_bind(&que, "QUEUE", TM_INFINITE))
		{
			printf("Bind error\n");
			exit(0);
		}

		while ((len = rt_queue_receive(&que, &mes, TM_INFINITE)) > 0)
		{
			printf("SiT Got Message\n");
			printf(" : %s\n", (const char *)mes);
			rt_queue_free(&que, mes);
			sleep(1);
		}
		rt_queue_unbind(&que);
		if (len != -EIDRM)
		{
			printf("Length error\n");
			exit(0);
		}
	}
}

int main()
{
	if (rt_task_create(&sk_desc, "tSot", STACKSIZE, SNPR, TASKMODE) != 0)
	{
		printf("skT Create failed\n");
		return -1;
	}
	printf("skT Create SUC");
	if (rt_task_create(&sc_desc, "tSit", STACKSIZE, SOPR, TASKMODE) != 0)
	{
		printf("scT Create failed\n");
		return -1;
	}
	printf("scT Create SUC");
	if (rt_queue_create(&que, "QUEUE", QPSIZE, QLMT, QMD) != 0)
	{
		printf("meQ Create failed\n");
		return -1;
	}
	printf("meQ Create SUC");

	rt_task_start(&sk_desc, tSoT, NULL);
	rt_task_start(&sc_desc, tSiT, NULL);
	pause();
	return 0;
}