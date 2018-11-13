#include "..\stdafx.h"
#include "PEManager.h"

#define MAPPING_FILE_NAME  L"PEToolFileMapping"
#define SAVE_FREE_HANDLE(h)  if(h){ CloseHandle(h); h = NULL;}

CPEManager::CPEManager(LPCWSTR lpFile)
	:m_hFile(NULL)
	, m_hMappingFile(NULL)
	, m_pImageBase(NULL)
	, m_pDosHeader(NULL)
	, m_pNTHeader(NULL)
{

	if (::PathFileExistsW(lpFile))
	{
		//打开文件
		m_hFile = ::CreateFileW(lpFile, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, 0, NULL);
		if (m_hFile)
		{
			//创建映像文件
			m_hMappingFile = ::CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, 1024, MAPPING_FILE_NAME);
			if (m_hMappingFile)
			{
			    //
				m_pImageBase = (LPVOID)MapViewOfFile(m_hMappingFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				if (m_pImageBase)
				{
					m_pDosHeader = (PIMAGE_DOS_HEADER)m_pImageBase;
					m_pNTHeader   = (PIMAGE_NT_HEADERS)((DWORD)m_pDosHeader + m_pDosHeader->e_lfanew);
				}
			}
		}
	}
}


CPEManager::~CPEManager()
{
	if(m_pImageBase)
		::UnmapViewOfFile(m_pImageBase);

	SAVE_FREE_HANDLE(m_hMappingFile);
	SAVE_FREE_HANDLE(m_hFile);
}

BOOL CPEManager::IsPEFile()
{
	if (NULL == m_pDosHeader || NULL == m_pNTHeader)
		return FALSE;

	//MZ  PE00
	if (m_pDosHeader->e_magic != IMAGE_DOS_SIGNATURE                 
		|| m_pNTHeader->Signature != IMAGE_NT_SIGNATURE)               
		return FALSE;

	return TRUE;
}

VOID CPEManager::ReadDataDirectroyInfor()
{
	if (NULL == m_pNTHeader)
		return;

	PIMAGE_OPTIONAL_HEADER pOH = &m_pNTHeader->OptionalHeader;
	if (NULL == pOH)
		return;

	for (int i = 0; i < 16; i++)
	{
		printf("Directroy %d , \t VirtualAddress = 0x%x, \t  Size = 0x%x \n", i,pOH->DataDirectory[i].VirtualAddress, pOH->DataDirectory[i].Size);
	}
}
DWORD CPEManager::RVAToOffset( DWORD VirtualAddress)
{
	if (NULL == m_pNTHeader)
		return 0;

	PIMAGE_SECTION_HEADER SectionHeader = (IMAGE_SECTION_HEADER*)(m_pNTHeader + sizeof(IMAGE_NT_HEADERS));
	int NumOfSections = m_pNTHeader->FileHeader.NumberOfSections;
	for (int i = 0; i<NumOfSections; i++)
	{
		if (VirtualAddress > SectionHeader[i].VirtualAddress
			&&VirtualAddress < SectionHeader[i].VirtualAddress + SectionHeader[i].SizeOfRawData)
		{
			DWORD AposRAV = VirtualAddress - SectionHeader[i].VirtualAddress;
			DWORD Offset     = SectionHeader[i].PointerToRawData + AposRAV;
			return Offset;
		}
	}
	return 0;
}



VOID CPEManager::ReadImportTableInfor()
{
	if (NULL == m_pNTHeader)
		return;

	//基址+偏移量
	PIMAGE_IMPORT_DESCRIPTOR pIMDes = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)m_pImageBase +
		RVAToOffset(m_pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));

	if (NULL == pIMDes)
		return;

	while (pIMDes->Characteristics)
	{
		char* pDllName = (char*)((BYTE*)m_pImageBase + RVAToOffset(pIMDes->Name));

		printf("DllName ： %s \n", pDllName);
		pIMDes++;
	}

}
