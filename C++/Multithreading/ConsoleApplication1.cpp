#include <stdio.h>

#include <stdlib.h>

#include <conio.h>

#include <windows.h>

const int n = 100, // длина буфера

m = 7, // количество производителей

k = 3; // количество потребителей

int buf[n], front = 0, rear = 0;

HANDLE hSemFull, hSemEmpty; // семафоры для условной синхронизации

// процесс, пополняющий буфер

DWORD WINAPI Producer(PVOID pvParam)

{

	int num;

	long prev;

	num = *((int*)pvParam);

	printf("thread %d (producer): start!\n", num);

	while (true)

	{
		for (int i = 0;i < k;i++) {
			WaitForSingleObject(hSemEmpty, INFINITE);
		}

		buf[rear] = rand() % n;

		printf("\nproducer %d: data = %d to %d", num, buf[rear], rear);

		rear = (rear+1) % n;
		Sleep(1000);

		ReleaseSemaphore(hSemFull, k, &prev);

	} return 0;

}

// процесс, берущий данные из буфера

DWORD WINAPI Consumer(PVOID pvParam)

{

	int num, data;

	long prev;

	num = *((int*)pvParam);

	printf("thread %d (consumer): start!\n", num);

	while (true)

	{

		WaitForSingleObject(hSemFull, INFINITE);


		data = buf[front];
		if(data==0){ printf("\nin %d =0,you can't take it",front);
		}
		else{ printf("\nconsumer %d: data = %d from %d", num, data, front); }
		front = (front+1) % n;
		Sleep(1000);

		ReleaseSemaphore(hSemEmpty, 1, &prev);

	}

	return 0;

}

int main()

{

	int i, x[k + m];

	DWORD dwThreadId[k + m], dw;

	HANDLE hThread[k + m];

	hSemEmpty = CreateSemaphore(NULL, n, n, L"Empty");

	hSemFull = CreateSemaphore(NULL, 0, n, L"Full");




	for (i = 0; i < k; i++)

	{

		x[i] = i;

		hThread[i] = CreateThread(NULL, 0, Producer, (PVOID)&x[i], 0, &dwThreadId[i]);

		if (!hThread) printf("main process: thread %d not execute!", i);

	}

	for (i = k; i < k + m; i++)

	{

		x[i] = i;

		hThread[i] = CreateThread(NULL, 0, Consumer, (PVOID)&x[i], 0, &dwThreadId[i]);

		if (!hThread) printf("main process: thread %d not execute!", i);

	}

	WaitForMultipleObjects(k + m, hThread, TRUE, INFINITE);

	// закрытие описателей событий

	CloseHandle(hSemFull);

	CloseHandle(hSemEmpty);


	return 0;

}