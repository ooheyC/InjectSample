#include "stdafx.h"
#include "ProcessManager.h"

CProcessManager::CProcessManager()
{
}

CProcessManager::~CProcessManager()
{
}

DWORD CProcessManager::GetDestProcessID(LPCWSTR lpProces)
{
	DWORD ProcessID = 0;
	if (NULL == lpProces)
		return ProcessID;
	//获取进程快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap)
		return ProcessID;


	//填充结构
	PROCESSENTRY32 PE32;
	PE32.dwSize = sizeof(PROCESSENTRY32);
	BOOL bNext = ::Process32First(hProcessSnap, &PE32);
	while (bNext)
	{
		if (wcsicmp(lpProces, PE32.szExeFile) == 0)
		{
			ProcessID = PE32.th32ProcessID;
			break;
		}
		bNext = ::Process32Next(hProcessSnap, &PE32);
	}

	CloseHandle(hProcessSnap);
	return ProcessID;
}

HANDLE CProcessManager::GetDestProcessHandle(DWORD dwProcessID)
{
	return ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
}

BOOL CProcessManager::EnablePrivilege(LPCWSTR name)
{
	HANDLE hToken = INVALID_HANDLE_VALUE;

	if (0 == OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
		return FALSE;

	CloseHandle(hToken);
	return TRUE;
}

BOOL  CProcessManager::GetDestProcessModuleEntry(MODULEENTRY32 & me32, LPCWSTR lpDll, DWORD dwProcessID)
{
	BOOL bFoud = FALSE;

	//获取进程快照
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	if (INVALID_HANDLE_VALUE == hProcessSnap)
		return bFoud;

	//填充结
	me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bNext = ::Module32First(hProcessSnap, &me32);
	while (bNext)
	{
		if (wcsicmp(me32.szExePath, lpDll) == 0)
		{
			bFoud = TRUE;
			break;
		}
		bNext = ::Module32Next(hProcessSnap, &me32);
	}

	CloseHandle(hProcessSnap);
	return bFoud;
}