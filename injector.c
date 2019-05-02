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

	// #1. dwPID�� �̿��Ͽ� ��� ���μ����� �ڵ��� ����
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID))) {
		return FALSE;
	}
	printf("dwPID�� �̿��Ͽ� ��� ���μ����� �ڵ��� ���� \n");

	// #2. ��� ���μ��� �޸𸮿� szDllName ũ�⸸ŭ �޸� �Ҵ�
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
	printf("��� ���μ��� �޸𸮿� szDllName ũ�⸸ŭ �޸� �Ҵ� \n");

	// #3. �Ҵ� ���� �޸𸮿� dll ��θ� ��
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllName, dwBufSize, NULL);
	printf("�Ҵ� ���� �޸𸮿� dll ��θ� �� \n");

	// #4. LoadLibraryA() API �ּҸ� ����
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	printf("LoadLibraryA() API �ּҸ� ���� \n");

	// #5. ��� ���μ����� ������ ����
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);
	printf("��� ���μ����� ������ ����\n");

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return 0;
}



