#include   "framework.h"
#include   "FileDialog.h"
#include   <shellapi.h>  // Drag , expolore
#include   <commdlg.h>

bool FileDialog::Open(wstring& selectFile, FileMode mode)
{
	OPENFILENAME OFN;
	WCHAR	lpstrFile[2562] = L"";  // 멀티 최대문자

	WCHAR	lpProgramDiretory[1024];
	const WCHAR* addDirectory = L"";

	WCHAR filter[1024] = L"모든파일\0*.*";
	const WCHAR* addFilter = L"";

	// ./A/B/aa.png
	GetCurrentDirectory(1024, lpProgramDiretory);

	switch (mode)
	{
	case TEXTURE:
		addDirectory = L"\\Textures\\Color";
		addFilter = L"이미지파일\0*.png; *.jpg; *.jpeg; *.bmp;";
		break;
	case FBX:
		addDirectory = L"\\Assets\\FBX";
		addFilter = L"FBX파일\0*.fbx;";
		break;
	case FX:
		addDirectory = L"\\Textures\\Effect";
		addFilter = L"Fx파일\0.fx;";
		break;
	case SCENE:
		addDirectory = L"\\Saved";
		addFilter = L"Scene파일\0.xml;";
	}

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner   = MAIN->GetWindowHandler();  // Modal(Wait)
	OFN.lpstrFilter = wcscat(filter, addFilter); // 우측에 필터
	OFN.lpstrFile = lpstrFile;  // 선택되면 내용이 담겨지는 버퍼
	OFN.nMaxFile = 2562;
	OFN.lpstrTitle = L"불러오기";
	OFN.lpstrInitialDir = wcscat(lpProgramDiretory, addDirectory);
//	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_EXPLORER;
	// OFN_HIDEREADONLY --> GetOpenFile
	// OFN_NOCHANGEDIR  --> Api사용하면 현재 디렉토리 위치 변경안되게
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

bool FileDialog::Save(wstring& selectFile, FileMode mode)
{
	OPENFILENAME	OFN;
	WCHAR			lpstrFile[2562] = L"";

	WCHAR			lpProgramDiretory[1024];
	const WCHAR*	addDirectory = L"";

	WCHAR filter[1024] = L"모든파일\0*.*";
	const WCHAR* addFilter = L"";

	// ./A/B/aa.png
	GetCurrentDirectory(1024, lpProgramDiretory);

	switch (mode)
	{
	case TEXTURE:
		addDirectory = L"\\Textures\\Color";
		addFilter = L"이미지파일\0*.png; *.jpg; *.jpeg; *.bmp;";
		break;
	case FBX:
		addDirectory = L"\\Assets\\FBX";
		addFilter = L"FBX파일\0*.fbx;";
		break;
	case FX:
		addDirectory = L"\\Textures\\Effect";
		addFilter = L"Fx파일\0.fx;";
		break;
	case SCENE:
		addDirectory = L"\\Saved";
		addFilter = L"Scene파일\0.xml;";
	}

	// ./A/B/aa.png
	GetCurrentDirectory(1024, lpProgramDiretory);

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = MAIN->GetWindowHandler();  // Modal
	OFN.lpstrFilter = wcscat(filter, addFilter); // 필터
	OFN.lpstrFile = lpstrFile;  // 파일을 선택후 확인버튼이 눌려지면 선택된 내역이 
	OFN.nMaxFile = 2562;        // Multi / Single --> 2562
	OFN.lpstrTitle = L"저장하기";
	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
	OFN.lpstrInitialDir = wcscat(lpProgramDiretory, addDirectory);

	// OFN_HIDEREADONLY   --> Read Only
	// OFN_NOCHANGEDIR    --> 파일다이올러그 열고 선택하면, 현재 디렉토리 위치 변경 안되게끔
	// OFN_EXPLORER ,OFN_ALLOWMULTISELECT --> Explore 보여 주게
	if (GetSaveFileName(&OFN) == 0)
		return false;

	selectFile = OFN.lpstrFile;
	return true;
}
