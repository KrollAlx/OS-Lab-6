#include<iostream>
#include<Windows.h>
#include<ctime>
using namespace std;

int readerCounter = 0;
int writerCounter = 0;
CRITICAL_SECTION cs;
HANDLE hReaderCounterSemaphor;
HANDLE hWritetCounterSemaphor;
HANDLE hDataSemaphor;
int _data = 0;

void reader(void* params)
{
	int t = 20;
	while (t)
	{
		int readedData;
		WaitForSingleObject(hReaderCounterSemaphor, INFINITE);
		readerCounter++;
		if (readerCounter == 1)
		{
			WaitForSingleObject(hDataSemaphor, INFINITE);
		}
		ReleaseSemaphore(hReaderCounterSemaphor, 1, NULL);
		
		readedData = _data;

		WaitForSingleObject(hReaderCounterSemaphor, INFINITE);

		ReleaseSemaphore(hDataSemaphor, 1, NULL);
		ReleaseSemaphore(hReaderCounterSemaphor, 1, NULL);

		EnterCriticalSection(&cs);
		cout << "readed data: " << readedData << endl;
		LeaveCriticalSection(&cs);

		t--;
		Sleep(1000);
	}
}

void writer(void* params)
{
	int t = 20;
	while (t)
	{
		int newData = rand() % 10;

		WaitForSingleObject(hWritetCounterSemaphor, INFINITE);
		writerCounter++;
		if (writerCounter == 1)
		{
			WaitForSingleObject(hDataSemaphor, INFINITE);
		}
		ReleaseSemaphore(hWritetCounterSemaphor, 1, NULL);

		_data = newData;

		WaitForSingleObject(hWritetCounterSemaphor, INFINITE);
		writerCounter--;
		if (writerCounter == 0) {
			ReleaseSemaphore(hDataSemaphor, 1, NULL);
		}
		ReleaseSemaphore(hWritetCounterSemaphor, 1, NULL);

		EnterCriticalSection(&cs);
		cout << "we write data: " << newData << endl;
		LeaveCriticalSection(&cs);

		t--;
		Sleep(2000);
	}
}

int main()
{
	srand(time(0));
	InitializeCriticalSection(&cs);

	HANDLE hThreadWrite1;
	DWORD IDThreadWrite1;
	HANDLE hThreadWrite2;
	DWORD IDThreadWrite2;
	HANDLE hThreadWrite3;
	DWORD IDThreadWrite3;
	HANDLE hThreadRead1;
	DWORD IDThreadRead1;
	HANDLE hThreadRead2;
	DWORD IDThreadRead2;
	HANDLE hThreadRead3;
	DWORD IDThreadRead3;

	hThreadWrite1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)writer, NULL, 0, &IDThreadWrite1);
	hThreadWrite2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)writer, NULL, 0, &IDThreadWrite2);
	hThreadWrite3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)writer, NULL, 0, &IDThreadWrite3);
	hThreadRead1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)reader, NULL, 0, &IDThreadRead1);
	hThreadRead2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)reader, NULL, 0, &IDThreadRead2);
	hThreadRead3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)reader, NULL, 0, &IDThreadRead3);

	hReaderCounterSemaphor = CreateSemaphore(NULL, 1, 1, L"semaphor_rc");
	hWritetCounterSemaphor = CreateSemaphore(NULL, 1, 1, L"semaphor_wc");
	hDataSemaphor = CreateSemaphore(NULL, 1, 1, L"semaphor_data");

	WaitForSingleObject(hThreadWrite1, INFINITE);
	WaitForSingleObject(hThreadWrite2, INFINITE);
	WaitForSingleObject(hThreadWrite3, INFINITE);
	WaitForSingleObject(hThreadRead1, INFINITE);
	WaitForSingleObject(hThreadRead2, INFINITE);
	WaitForSingleObject(hThreadRead3, INFINITE);

	DeleteCriticalSection(&cs);
	system("pause");
	return 0;
}