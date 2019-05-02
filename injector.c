#include <stdio.h>
#include <string.h>
#include <windows.h>

int main(int argc, char *argv[]) {
	HANDLE hProcess, hThread;
	LPVOID pRemoteBuf;
	LPTHREAD_START_ROUTINE pThreadProc;
	DWORD dwPID = atoi(argv[1]);
	LPCSTR szDllName = argv[2];
	DWORD dwBufSize = lstrlen(szDllName) + 1;

	// #1. dwPID를 이용하여 대상 프로세스의 핸들을 구함
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID))) {
		return FALSE;
	}
	printf("dwPID를 이용하여 대상 프로세스의 핸들을 구함 \n");

	// #2. 대상 프로세스 메모리에 szDllName 크기만큼 메모리 할당
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
	printf("대상 프로세스 메모리에 szDllName 크기만큼 메모리 할당 \n");

	// #3. 할당 받은 메모리에 dll 경로를 씀
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllName, dwBufSize, NULL);
	printf("할당 받은 메모리에 dll 경로를 씀 \n");

	// #4. LoadLibraryA() API 주소를 구함
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	printf("LoadLibraryA() API 주소를 구함 \n");

	// #5. 대상 프로세스에 스레드 실행
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);
	printf("대상 프로세스에 스레드 실행\n");

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return 0;
}



