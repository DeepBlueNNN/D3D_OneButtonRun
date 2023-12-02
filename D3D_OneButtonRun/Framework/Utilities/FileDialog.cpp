#include   "framework.h"
#include   "FileDialog.h"
#include   <shellapi.h>  // Drag , expolore
#include   <commdlg.h>

bool FileDialog::Open(wstring& selectFile, FileMode mode)
{
	OPENFILENAME OFN;
	WCHAR	lpstrFile[2562] = L"";  // ��Ƽ �ִ빮��

	WCHAR	lpProgramDiretory[1024];
	const WCHAR* addDirectory = L"";

	WCHAR filter[1024] = L"�������\0*.*";
	const WCHAR* addFilter = L"";

	// ./A/B/aa.png
	GetCurrentDirectory(1024, lpProgramDiretory);

	switch (mode)
	{
	case TEXTURE:
		addDirectory = L"\\Textures\\Color";
		addFilter = L"�̹�������\0*.png; *.jpg; *.jpeg; *.bmp;";
		break;
	case FBX:
		addDirectory = L"\\Assets\\FBX";
		addFilter = L"FBX����\0*.fbx;";
		break;
	case FX:
		addDirectory = L"\\Textures\\Effect";
		addFilter = L"Fx����\0.fx;";
		break;
	case SCENE:
		addDirectory = L"\\Saved";
		addFilter = L"Scene����\0.xml;";
	}

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner   = MAIN->GetWindowHandler();  // Modal(Wait)
	OFN.lpstrFilter = wcscat(filter, addFilter); // ������ ����
	OFN.lpstrFile = lpstrFile;  // ���õǸ� ������ ������� ����
	OFN.nMaxFile = 2562;
	OFN.lpstrTitle = L"�ҷ�����";
	OFN.lpstrInitialDir = wcscat(lpProgramDiretory, addDirectory);
//	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_EXPLORER;
	// OFN_HIDEREADONLY --> GetOpenFile
	// OFN_NOCHANGEDIR  --> Api����ϸ� ���� ���丮 ��ġ ����ȵǰ�
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

bool FileDialog::Save(wstring& selectFile, FileMode mode)
{
	OPENFILENAME	OFN;
	WCHAR			lpstrFile[2562] = L"";

	WCHAR			lpProgramDiretory[1024];
	const WCHAR*	addDirectory = L"";

	WCHAR filter[1024] = L"�������\0*.*";
	const WCHAR* addFilter = L"";

	// ./A/B/aa.png
	GetCurrentDirectory(1024, lpProgramDiretory);

	switch (mode)
	{
	case TEXTURE:
		addDirectory = L"\\Textures\\Color";
		addFilter = L"�̹�������\0*.png; *.jpg; *.jpeg; *.bmp;";
		break;
	case FBX:
		addDirectory = L"\\Assets\\FBX";
		addFilter = L"FBX����\0*.fbx;";
		break;
	case FX:
		addDirectory = L"\\Textures\\Effect";
		addFilter = L"Fx����\0.fx;";
		break;
	case SCENE:
		addDirectory = L"\\Saved";
		addFilter = L"Scene����\0.xml;";
	}

	// ./A/B/aa.png
	GetCurrentDirectory(1024, lpProgramDiretory);

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = MAIN->GetWindowHandler();  // Modal
	OFN.lpstrFilter = wcscat(filter, addFilter); // ����
	OFN.lpstrFile = lpstrFile;  // ������ ������ Ȯ�ι�ư�� �������� ���õ� ������ 
	OFN.nMaxFile = 2562;        // Multi / Single --> 2562
	OFN.lpstrTitle = L"�����ϱ�";
	OFN.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
	OFN.lpstrInitialDir = wcscat(lpProgramDiretory, addDirectory);

	// OFN_HIDEREADONLY   --> Read Only
	// OFN_NOCHANGEDIR    --> ���ϴ��̿÷��� ���� �����ϸ�, ���� ���丮 ��ġ ���� �ȵǰԲ�
	// OFN_EXPLORER ,OFN_ALLOWMULTISELECT --> Explore ���� �ְ�
	if (GetSaveFileName(&OFN) == 0)
		return false;

	selectFile = OFN.lpstrFile;
	return true;
}
