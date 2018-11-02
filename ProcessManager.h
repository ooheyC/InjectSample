#pragma once
class CProcessManager {
public:
	CProcessManager();
	~CProcessManager();

public:
	//Read PID
	DWORD  GetDestProcessID(LPCWSTR lpProces);

	//Read Process Handle
	HANDLE  GetDestProcessHandle(DWORD dwProcessID);

	BOOL EnablePrivilege(LPCWSTR name);

	//read module infor 
	BOOL  GetDestProcessModuleEntry(MODULEENTRY32& me32, LPCWSTR lpDll, DWORD dwProcessID);

};

