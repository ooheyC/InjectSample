#pragma once
#include "ProcessManager.h"
/*
1、获取目标进程的句柄，将源文件dll加载路径写入目标内存中（VirtualAllocEx  、 WriteProcessMemory）
2、获取LaodLibrary地址（kernel32.dll在各个进程的地址的是一致的，所以可以通过GetProcAddress获取地址）
3、给目标进程创建线程调用 LoadLibrary （CreateRemoteThread）
*/

class CRemoteIJManager :public CProcessManager
{
public:
	CRemoteIJManager();
	~CRemoteIJManager();
	/*开始执行注入
	lpProcess: 目标进程名称
	lpSrcDll:    源dll文件
	*/
	VOID  JT_Start(LPCWSTR lpProcess,LPCSTR lpSrcDll); 

	//停止，卸载掉注入模块
	VOID JT_Stop(); 

protected:
	HANDLE m_hDestProcessHanlde;
	char m_szSrcDllFile[MAX_PATH];
	DWORD m_dwDestProcessID;
};

