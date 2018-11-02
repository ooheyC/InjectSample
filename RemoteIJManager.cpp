#include "stdafx.h"
#include "RemoteIJManager.h"



CRemoteIJManager::CRemoteIJManager()
	:m_hDestProcessHanlde(NULL)
	, m_dwDestProcessID(0)
{
	ZeroMemory(m_szSrcDllFile, MAX_PATH);
}


CRemoteIJManager::~CRemoteIJManager()
{
	if (m_hDestProcessHanlde)
		CloseHandle(m_hDestProcessHanlde);
}


VOID CRemoteIJManager::JT_Start(LPCWSTR lpProcess, LPCSTR lpSrcDll)
{
	strcpy(m_szSrcDllFile, lpSrcDll);

	m_dwDestProcessID = GetDestProcessID(lpProcess);
	m_hDestProcessHanlde = GetDestProcessHandle(m_dwDestProcessID);
	if (NULL == m_hDestProcessHanlde)
		return;

	DWORD dwSize = strlen(lpSrcDll)+1;
	LPVOID pBuffer = VirtualAllocEx(m_hDestProcessHanlde, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (pBuffer)
	{
		WriteProcessMemory(m_hDestProcessHanlde, pBuffer, (LPVOID)lpSrcDll, dwSize, NULL);
		FARPROC pThreadProc = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
		HANDLE hRemoteHandler = CreateRemoteThread(
			m_hDestProcessHanlde,
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)pThreadProc,
			pBuffer,
			NULL,
			NULL
		);
		if (hRemoteHandler != 0)
		{
			ResumeThread(hRemoteHandler);
			WaitForSingleObject(hRemoteHandler, INFINITE);
		}
		VirtualFreeEx(m_hDestProcessHanlde, pBuffer, dwSize, MEM_RELEASE);
	}
}

VOID CRemoteIJManager::JT_Stop()
{
	MODULEENTRY32 ME32;
	
	string szDllFile = m_szSrcDllFile;
	//string szDllName = szDllFile.substr(szDllFile.rfind('\\')+1);
	wstring wszDLLFile;
	wszDLLFile.append(szDllFile.begin(), szDllFile.end());

	if (GetDestProcessModuleEntry(ME32, wszDLLFile.c_str(),m_dwDestProcessID))
	{
		FARPROC pThreadProc = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "FreeLibrary");
		HANDLE hRemoteHandler = CreateRemoteThread(
			m_hDestProcessHanlde,
			NULL,
			NULL,
			(LPTHREAD_START_ROUTINE)pThreadProc,
			ME32.modBaseAddr,
			NULL,
			NULL
		);

		if (hRemoteHandler != 0)
		{
			WaitForSingleObject(hRemoteHandler, INFINITE);
		}
	}
}

