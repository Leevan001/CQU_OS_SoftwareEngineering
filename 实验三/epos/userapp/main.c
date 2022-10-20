/*
 * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
 */
#include <inttypes.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <sys/mman.h>
#include <syscall.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "graphics.h"
#include<time.h>

extern void* tlsf_create_with_pool(void* mem, size_t bytes);
extern void* g_heap;

/**
 * GCC insists on __main
 *    http://gcc.gnu.org/onlinedocs/gccint/Collect2.html
 */
void __main()
{
    size_t heap_size = 32 * 1024 * 1024;
    void* heap_base = mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    g_heap = tlsf_create_with_pool(heap_base, heap_size);
}

//线程优先级
int tid_foo1, tid_foo2, tid_foo3;

//画线自定义函数
void drawLine(int x1, int y1, int x2, int y2, int extra_x, COLORREF cr)
{
    line(x1 + extra_x, (y1 / 5) * 3, x2 + extra_x, (y2 / 5) * 3, cr);
}

//睡眠函数
void mySleep()
{
	struct timespec tim, tim2;
	tim.tv_sec = 1;
	tim.tv_nsec = 10000000;
	nanosleep(&tim, &tim2);
}

//冒泡排序
void bubsort1(int* a, int n)
{
	int i, j, temp;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if (a[j] > a[j + 1])
			{
				//覆盖排序前的两条线段
				drawLine(-a[j], j * 6 , 0, j * 6, 250, RGB(0, 0, 0));
				drawLine(-a[j + 1], (j + 1) * 6 ,0, (j + 1) * 6 , 250, RGB(0, 0, 0));
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
				drawLine(-a[j], j * 6 , 0, j * 6 , 250, 0x4682B4);
				drawLine(-a[j + 1], (j + 1) * 6 , 0, (j + 1) * 6 , 250, 0x4682B4);
				//mySleep();
			}
			//mySleep();
		}
		mySleep();
	}
}

void bubsort2(int* a, int n)
{
	int i, j, temp;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if (a[j] > a[j + 1])
			{
				//覆盖排序前的两条线段
				drawLine(0, j * 6 , a[j], j * 6 , 250, RGB(0, 0, 0));
				drawLine(0, (j + 1) * 6 , a[j + 1], (j + 1) * 6 , 250, RGB(0, 0, 0));
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
				drawLine(0, j * 6 , a[j], j * 6 , 250, 0xFFFF00);
				drawLine(0, (j + 1) * 6 , a[j + 1], (j + 1) * 6 , 250, 0xFFFF00);
				//mySleep();
			}
			//mySleep();
		}
		mySleep();
	}
}

//线程函数
void tsk_foo1(void* pv)
{
	time_t time(time_t * loc);
	srand(time(NULL));
	int myCount_1[150];
	int i, k;
	for (i = 0; i < 150; i++)
	{
		myCount_1[i] = rand() % 200;
		printf("%d\n", myCount_1[i]);
	}

	//显示未排序的画面
	for (k = 0; k < 150; k++)
	{
		drawLine(-myCount_1[k], k * 6 ,0, k * 6 , 250, 0x4682B4);
	}

	mySleep();
	bubsort1(myCount_1, 150);
	task_exit(0);
}

void tsk_foo2(void* pv)
{
	time_t time(time_t * loc);
	srand(time(NULL));
	int myCount_2[150];
	int i, k;
	for (i = 0; i < 150; i++)
	{
		myCount_2[i] = rand() % 200;
		printf("%d\n", myCount_2[i]);
	}

	//显示未排序的画面
	for (k = 0; k < 150; k++)
	{
		drawLine(0, k * 6 , myCount_2[k], k * 6 , 250, 0xFFFF00);
	}
	mySleep();
	bubsort2(myCount_2, 150);
	task_exit(0);
}


//优先级展示条
void show_priority(int tid, int k) {
	int length = 10 * getpriority(tid);
	int m = 0;
	switch (k)
	{
	case 1:
	{
		for (m = 0; m < 20; m++)
			drawLine(-250, 910+m, 0, 910+m, 250, RGB(0, 0, 0));
		for (m = 0; m < 20; m++)
			drawLine(-length, 910 + m, 0, 910 + m, 250, 0x4682B4);
	}break;
	case 2:
	{
		for (m = 0; m < 20; m++)
			drawLine(0, 910 + m, 200, 910 + m, 250, RGB(0, 0, 0));
		for (m = 0; m < 20; m++)
			drawLine(0, 910 + m, length, 910 + m, 250, 0xFFFF00);
	}break;
	default:
		break;
	}

}

//控制线程
void mytask_control(void* pv) {
	show_priority(tid_foo1,1);
	show_priority(tid_foo2, 2);
	int mykeypress;
	while (1) {
		mykeypress = getchar();
		switch (mykeypress)
		{
		case 0x4800://(up)
		{
			setpriority(tid_foo1, getpriority(tid_foo1) + 2);
			show_priority(tid_foo1, 1);
		}
		break;
		case 0x5000://(down)
		{
			setpriority(tid_foo1, getpriority(tid_foo1)-2);
			show_priority(tid_foo1, 1);
		}
		break;
//0x4d00(right)/0x4b00(left)
		case 0x4d00:
		{
			setpriority(tid_foo2, getpriority(tid_foo2) + 2);
			show_priority(tid_foo2, 2);
		}
		break;
		case 0x4b00:
		{
			setpriority(tid_foo2, getpriority(tid_foo2) - 2);
			show_priority(tid_foo2, 2);
		}
		break;
		default:
			break;
		}
	}
}

void main(void* pv)
{
	unsigned char* stack_foo_1, * stack_foo_2, * stack_foo_3;
	unsigned int  stack_size = 1024 * 1024;
	stack_foo_1 = (unsigned char*)malloc(stack_size);
	stack_foo_2 = (unsigned char*)malloc(stack_size);
	stack_foo_3 = (unsigned char*)malloc(stack_size);
	init_graphic(0x143);

	tid_foo1=task_create(stack_foo_1 + stack_size, &tsk_foo1, (void*)0);
	tid_foo2=task_create(stack_foo_2 + stack_size, &tsk_foo2, (void*)0);
	tid_foo3 = task_create(stack_foo_3 + stack_size, &mytask_control, (void*)0);
	setpriority(tid_foo3, 0);
	setpriority(tid_foo1, 10);
	setpriority(tid_foo2, 10);


	free(stack_foo_1);
	free(stack_foo_2);
	free(stack_foo_3);

	while (1)
		;
	task_exit(0);
}