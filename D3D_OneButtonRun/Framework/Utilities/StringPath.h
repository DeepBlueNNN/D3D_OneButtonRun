#pragma once

namespace StringPath
{
    string   ToString(wstring str);
    wstring  ToWString(string str);
    void     Replace(string& str, string comp, string rep);

    vector<string> SplitString(string origin, const string& tok, bool bAdder = true);

    wstring  GetExtension(wstring file);
    string   GetFileName(string file);
    string   GetFileNameWithoutExtension(string file);
    void     CreateFolders(string file);
    bool     ExistDirectory(string file);
    void     GetFiles(vector<string>& files, const string& path, const string& filter, bool bFindSubFolder);
    void     GetFiles(vector<wstring>& files, const wstring& path, const wstring& filter, bool bFindSubFolder);
}