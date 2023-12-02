#pragma once
class  FileDialog
{
public:
	enum FileMode
	{
		NONE = 0,
		TEXTURE,
		FBX,
		FX,
		SCENE,
	};

public:
	FileDialog() = default;
	~FileDialog() = default;
public:
	static bool Open(wstring& selectFile, FileMode mode = NONE);
	static bool Save(wstring& selectFile, FileMode mode = NONE);
};
