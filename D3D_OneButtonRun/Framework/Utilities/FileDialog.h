#pragma once
class  FileDialog
{
public:
	FileDialog() = default;
	~FileDialog() = default;
public:
	static bool Open(wstring& selectFile);
};
