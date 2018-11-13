#pragma once

//pe文件分析管理类
class CPEManager
{
public:
	CPEManager(LPCWSTR lpFile);
	~CPEManager();
	//
	BOOL IsPEFile();

	//显示数据目录表信息（相对虚拟地址 和 大小）
	VOID ReadDataDirectroyInfor();

	VOID ReadImportTableInfor();

private:
	DWORD RVAToOffset(DWORD VirtualAddress);

private:
	HANDLE m_hFile;								//文件句柄
	HANDLE m_hMappingFile;				//映射文件句柄 
	LPVOID  m_pImageBase;					//映像文件基地址

	PIMAGE_DOS_HEADER  m_pDosHeader;		 //DOS头基址
	PIMAGE_NT_HEADERS   m_pNTHeader;		 //NT头基址
};

