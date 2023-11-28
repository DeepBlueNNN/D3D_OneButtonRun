#pragma once
class  FileDialog
{
public:
	FileDialog() = default;
	~FileDialog() = default;
public:
	static bool Open(wstring& selectFile);
	static bool Save(wstring& selectFile, int mode = 0);
};
