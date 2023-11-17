#include   "framework.h"
#include   "FileDialog.h"
#include   <shellapi.h>  // Drag , expolore
#include   <commdlg.h>

bool FileDialog::Open(wstring& selectFile)
{
	OPENFILENAME OFN;
	WCHAR        lpstrFile[2562] = L"";  // 멀티 최대문자
	WCHAR        lpCurrentDiretory[1024];
	wstring      CurrentDirectory;

	// ./A/B/aa.png
	GetCurrentDirectory(1024, lpCurrentDiretory);
	CurrentDirectory = lpCurrentDiretory;


	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner   = MAIN->GetWindowHandler();  // Modal(Wait)
	OFN.lpstrFilter = L"png File(*.png)\0*.png\0Text File\0*.png;*.png\0";  // 우측에 필터
	OFN.lpstrFile = lpstrFile;  // 선택되면 내요이 담겨지는 버퍼
	OFN.nMaxFile = 2562;
	OFN.lpstrInitialDir = lpCurrentDiretory;
//	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_EXPLORER;
	// OFN_HIDEREADONLY --> GetOpenFile
	// OFN_NOCHANGEDIR  --> Api사용하면 기본적으러 현재 디렉토이 위치가 
	//                      변경--> 안되게
	// OFN_EXPLORER | OFN_ALLOWMULTISELECT --> Multi Mode & 다이얼로그

	if (GetOpenFileName(&OFN) == 0)  // Modal
		return false;

	const UINT    nMaxLen = 2562;
//	WCHAR         szPath[nMaxLen];
	WCHAR         szFile[nMaxLen];

	UINT id = wcslen(OFN.lpstrFile) + 1;   // A\nB\n

	// 선택된 파일이 1개인 경우
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
