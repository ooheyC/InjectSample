// InjectTool.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "RemoteIJManager.h"
#include "PE\PEManager.h"

int main()
{
	//远程线程注入示例
	//CRemoteIJManager  RM;
	//RM.JT_Start(L"TestProcess.exe","E:\\study\\code\\MsgDLL.dll\\Debug\\MsgDLL.dll");
	//RM.JT_Stop(); 

	//
	CPEManager pm(L"E:\\study\\GamePPDraw.dll");
	pm.ReadDataDirectroyInfor();
	//pm.ReadImportTableInfor();


	system("pause");
    return 0;
}

