#include   "framework.h"
#include   "FileDialog.h"
#include   <shellapi.h>  // Drag , expolore
#include   <commdlg.h>

bool FileDialog::Open(wstring& selectFile)
{
	OPENFILENAME OFN;
	WCHAR        lpstrFile[2562] = L"";  // ��Ƽ �ִ빮��
	WCHAR        lpCurrentDiretory[1024];
	wstring      CurrentDirectory;

	// ./A/B/aa.png
	GetCurrentDirectory(1024, lpCurrentDiretory);
	CurrentDirectory = lpCurrentDiretory;


	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner   = MAIN->GetWindowHandler();  // Modal(Wait)
	OFN.lpstrFilter = L"png File(*.png)\0*.png\0Text File\0*.png;*.png\0";  // ������ ����
	OFN.lpstrFile = lpstrFile;  // ���õǸ� ������ ������� ����
	OFN.nMaxFile = 2562;
	OFN.lpstrInitialDir = lpCurrentDiretory;
//	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_EXPLORER;
	// OFN_HIDEREADONLY --> GetOpenFile
	// OFN_NOCHANGEDIR  --> Api����ϸ� �⺻������ ���� ������ ��ġ�� 
	//                      ����--> �ȵǰ�
	// OFN_EXPLORER | OFN_ALLOWMULTISELECT --> Multi Mode & ���̾�α�

	if (GetOpenFileName(&OFN) == 0)  // Modal
		return false;

	const UINT    nMaxLen = 2562;
//	WCHAR         szPath[nMaxLen];
	WCHAR         szFile[nMaxLen];

	UINT id = wcslen(OFN.lpstrFile) + 1;   // A\nB\n

	// ���õ� ������ 1���� ���
	if (OFN.lpstrFile[id] == NULL)
	{
		wcscpy(szFile, OFN.lpstrFile);
		wstring str = szFile;

		int oldSize;
		oldSize = str.length();
		if (oldSize != str.length())
			str = L"." + str;
		selectFile = str;
		return true;
	}
	return false;
}
