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

extern void *tlsf_create_with_pool(void* mem, size_t bytes);
extern void *g_heap;

/**
 * GCC insists on __main
 *    http://gcc.gnu.org/onlinedocs/gccint/Collect2.html
 */
void __main()
{
    size_t heap_size = 32*1024*1024;
    void  *heap_base = mmap(NULL, heap_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
	g_heap = tlsf_create_with_pool(heap_base, heap_size);
}




//画线自定义函数
void drawLine(int x1, int y1, int x2, int y2, int extra_x, COLORREF cr)
{
	line(x1+extra_x,(y1/5)*3,x2+extra_x,(y2/5)*3,cr);
}

//睡眠函数
void mySleep()
{
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 10000000;
	nanosleep(&tim, &tim2);
}


//第一种排序算法：插入排序
void insertSort(int* data,int n)
	{
		int i,j;
		
		for(i=1;i<n;i++)
		{
			int temp=data[i];
			for(j=i;j>0&&data[j-1]>temp;j--)
			{
				drawLine(0,j*6+60,data[j],j*6+60,0,RGB(0,0,0));//覆盖原来的线
				data[j]=data[j-1];
				drawLine(0, j * 6 + 60, data[j], j * 6 + 60, 0, 0x4682B4);
			}
			drawLine(0, j * 6 + 60, data[j] , j * 6 + 60,0,RGB(0,0,0));
			data[j]=temp;
			drawLine(0, j * 6 + 60, data[j], j * 6 + 60, 0, 0x4682B4);
			mySleep();
		}
	}

//第二种  选择排序
void selsort(int *a, int n)
{
	int i, j,low,temp;
	for (i = 0; i < n - 1; i++)
	{
		low = i;
		for (j = i; j < n; j++)
		{
			if (a[low] > a[j])
				low = j;
		}
		//进行交换，先抹掉原来线段
		drawLine(0, low * 6 + 60, a[low], low * 6 + 60, 150, RGB(0, 0, 0));
		drawLine(0, i * 6 + 60, a[i], i * 6 + 60, 150, RGB(0, 0, 0));
		temp = a[low];
		a[low] = a[i];
		a[i] = temp;
		drawLine(0, low * 6 + 60, a[low], low * 6 + 60, 150, 0x4169E1);
		drawLine(0, i * 6 + 60, a[i], i * 6 + 60, 150, 0x4169E1);
		mySleep();

	}
}

//第三种排序，冒泡排序
void bubsort(int*a, int n)
{
	int i, j, temp;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if (a[j] > a[j + 1])
			{
				//覆盖排序前的两条线段
				drawLine(0, j * 6 + 60, a[j], j * 6 + 60, 300, RGB(0, 0, 0));
				drawLine(0, (j+1) * 6 + 60, a[j+1], (j+1) * 6 + 60, 300, RGB(0, 0, 0));
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
				drawLine(0, j * 6 + 60, a[j], j * 6 + 60, 300, 0x556B2F);
				drawLine(0, (j + 1) * 6 + 60, a[j + 1], (j + 1) * 6 + 60, 300, 0x556B2F);
				mySleep();
			}
		}
	}
}

//第四种排序,快速排序
void quicksort(int *a, int left, int right)
{
	if (left >= right)
		return;
	int j = right, l = left, r = right,temp;
	while (l < r)
	{
		while (a[l] <= a[j] && l < r)l++;
		while (l < r && a[r] >= a[j])r--;
		//swap(a[l], a[r]);
		drawLine(0, l * 6 + 60, a[l], l * 6 + 60, 450, RGB(0, 0, 0));
		drawLine(0, r* 6 + 60, a[r], r * 6 + 60, 450, RGB(0, 0, 0));
		temp = a[l];
		a[l] = a[r];
		a[r] = temp;
		drawLine(0, l * 6 + 60, a[l], l * 6 + 60, 450, 0xFFFF00);
		drawLine(0, r * 6 + 60, a[r], r * 6 + 60, 450, 0xFFFF00);
		mySleep();

	}
	//swap(a[j], a[l]);
	drawLine(0, l * 6 + 60, a[l], l * 6 + 60, 450, RGB(0, 0, 0));
	drawLine(0, j * 6 + 60, a[j], j * 6 + 60, 450, RGB(0, 0, 0));
	temp = a[l];
	a[l] = a[j];
	a[j] = temp;
	drawLine(0, l * 6 + 60, a[l], l * 6 + 60, 450, 0xFFFF00);
	drawLine(0, j * 6 + 60, a[j], j * 6 + 60, 450, 0xFFFF00);
	mySleep();
	quicksort(a, left, l - 1);
	quicksort(a, l + 1, right);
}


//线程函数
void tsk_foo1(void *pv)
{
	time_t time(time_t *loc);
	srand(time(NULL));
	int myCount_1[150];
	int i,k;
	for(i=0;i<150;i++)
	{
		myCount_1[i]=rand()%150;
		printf("%d\n",myCount_1[i]);
	}
	
	//显示未排序的画面
	for(k=0;k<150;k++)
	{
		drawLine(0,k*6+60,myCount_1[k],k*6+60,0, 0x4682B4);
	}

	mySleep();

	insertSort(myCount_1, 150);


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
		myCount_2[i] = rand() % 150;
		printf("%d\n", myCount_2[i]);
	}

	//显示未排序的画面
	for (k = 0; k < 150; k++)
	{
		drawLine(0, k * 6 + 60, myCount_2[k], k * 6 + 60,150, 0x4169E1);
	}

	mySleep();
	selsort(myCount_2, 150);
	task_exit(0);
}

void tsk_foo3(void* pv)
{
	time_t time(time_t * loc);
	srand(time(NULL));
	int myCount_3[150];
	int i, k;
	for (i = 0; i < 150; i++)
	{
		myCount_3[i] = rand() % 150;
		printf("%d\n", myCount_3[i]);
	}

	//显示未排序的画面
	for (k = 0; k < 150; k++)
	{
		drawLine(0, k * 6 + 60, myCount_3[k], k * 6 + 60, 300, 0x556B2F);
	}

	mySleep();
	bubsort(myCount_3, 150);
	task_exit(0);
}

void tsk_foo4(void* pv)
{
	time_t time(time_t * loc);
	srand(time(NULL));
	int myCount_4[150];
	int i, k;
	for (i = 0; i < 150; i++)
	{
		myCount_4[i] = rand() % 150;
		printf("%d\n", myCount_4[i]);
	}

	//显示未排序的画面
	for (k = 0; k < 150; k++)
	{
		drawLine(0, k * 6 + 60, myCount_4[k], k * 6 + 60, 450, 0xFFFF00);
	}
	mySleep();
	quicksort(myCount_4,0,149);
	task_exit(0);
}
/**
 * 第一个运行在用户模式的线程所执行的函数
 */
void main(void *pv)
{
	unsigned char *stack_foo_1,*stack_foo_2, * stack_foo_3,* stack_foo_4;
	unsigned int  stack_size = 1024*1024;  
	stack_foo_1 = (unsigned char *)malloc(stack_size );
	stack_foo_2=(unsigned char*)malloc(stack_size);
	stack_foo_3 = (unsigned char*)malloc(stack_size);
	stack_foo_4 = (unsigned char*)malloc(stack_size);
	
	init_graphic(0x143);
	task_create(stack_foo_1+stack_size,&tsk_foo1,(void *)0);
	task_create(stack_foo_2 + stack_size, &tsk_foo2, (void*)0);
	task_create(stack_foo_3 + stack_size, &tsk_foo3, (void*)0);
	task_create(stack_foo_4 + stack_size, &tsk_foo4, (void*)0);
	free(stack_foo_1);
	free(stack_foo_2);
	free(stack_foo_3);
	free(stack_foo_4);

    while(1)
        ;
    task_exit(0);
}