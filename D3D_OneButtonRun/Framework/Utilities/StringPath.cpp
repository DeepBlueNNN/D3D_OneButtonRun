#include "Framework.h"
#include "StringPath.h"

void StringPath::Replace(string& str, string comp, string rep)
{
    string temp = str;

    size_t start_pos = 0;
    while ((start_pos = temp.find(comp, start_pos)) != string::npos)
    {
        temp.replace(start_pos, comp.length(), rep);
        start_pos += rep.length();
    }
    str = temp;
}


string StringPath::ToString(wstring str)
{
    string temp;
    temp.assign(str.begin(), str.end());

    return temp;
}

wstring StringPath::ToWString(string str)
{
    wstring temp;
    temp.assign(str.begin(), str.end());

    return temp;
}

vector<string> StringPath::SplitString(string origin, const string& tok, bool bAdder)
{
    vector<string> result;

    Replace(origin, "\\", "/");

    size_t cutAt = 0;

    while((cutAt = origin.find_first_of(tok)) != origin.npos)
    {
        if (cutAt > 0)
            result.push_back(origin.substr(0, cutAt));

        origin = origin.substr(cutAt + 1);
    }

    if(bAdder && (origin.size() > 0))
        result.push_back(origin);

    return result;
}

wstring StringPath::GetExtension(wstring file)
{
    size_t index = file.find_last_of('.');
    wstring tmp  = file.substr(index + 1, file.length());

    for (int i = 0; i < tmp.size(); i++) 
    {
        tmp[i] = std::tolower(tmp[i]);
    }

    return file.substr(index + 1, file.length());
}

string StringPath::GetFileName(string file)
{
    // a/c/d/abc.png --> /abc.png
    string temp = file;

    Replace(temp,"\\","/");

    size_t index = temp.find_last_of('/');

    return temp.substr(index + 1);

}

string StringPath::GetFileNameWithoutExtension(string file)
{
    string filename = GetFileName(file);
    size_t index = filename.find_last_of('.');

    return filename.substr(0,index);
}

bool StringPath::ExistDirectory(string file)
{
    DWORD fileValue = GetFileAttributesA(file.c_str());

    BOOL temp = (fileValue != INVALID_FILE_ATTRIBUTES
        && (fileValue & FILE_ATTRIBUTE_DIRECTORY));

    return temp;
}

void StringPath::CreateFolders(string file)
{
    vector<string> folders = SplitString(file, "/",false);

    string temp = "";
    for (string folder : folders)
    {
        temp = temp + folder + "/";
        printf("%s\n", temp.c_str());
        if (!ExistDirectory(temp))
        CreateDirectoryA(temp.c_str(), 0);
    }
}


