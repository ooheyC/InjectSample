#pragma once
#include "ProcessManager.h"
/*
1����ȡĿ����̵ľ������Դ�ļ�dll����·��д��Ŀ���ڴ��У�VirtualAllocEx  �� WriteProcessMemory��
2����ȡLaodLibrary��ַ��kernel32.dll�ڸ������̵ĵ�ַ����һ�µģ����Կ���ͨ��GetProcAddress��ȡ��ַ��
3����Ŀ����̴����̵߳��� LoadLibrary ��CreateRemoteThread��
*/

class CRemoteIJManager :public CProcessManager
{
public:
	CRemoteIJManager();
	~CRemoteIJManager();
	/*��ʼִ��ע��
	lpProcess: Ŀ���������
	lpSrcDll:    Դdll�ļ�
	*/
	VOID  JT_Start(LPCWSTR lpProcess,LPCSTR lpSrcDll); 

	//ֹͣ��ж�ص�ע��ģ��
	VOID JT_Stop(); 

protected:
	HANDLE m_hDestProcessHanlde;
	char m_szSrcDllFile[MAX_PATH];
	DWORD m_dwDestProcessID;
};

