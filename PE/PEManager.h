#pragma once

//pe�ļ�����������
class CPEManager
{
public:
	CPEManager(LPCWSTR lpFile);
	~CPEManager();
	//
	BOOL IsPEFile();

	//��ʾ����Ŀ¼����Ϣ����������ַ �� ��С��
	VOID ReadDataDirectroyInfor();

	VOID ReadImportTableInfor();

private:
	DWORD RVAToOffset(DWORD VirtualAddress);

private:
	HANDLE m_hFile;								//�ļ����
	HANDLE m_hMappingFile;				//ӳ���ļ���� 
	LPVOID  m_pImageBase;					//ӳ���ļ�����ַ

	PIMAGE_DOS_HEADER  m_pDosHeader;		 //DOSͷ��ַ
	PIMAGE_NT_HEADERS   m_pNTHeader;		 //NTͷ��ַ
};

