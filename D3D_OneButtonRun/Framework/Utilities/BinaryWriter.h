#pragma once

class BinaryWriter
{
public:
    BinaryWriter(string filePath);
    BinaryWriter(wstring filePath);
    ~BinaryWriter();

    void Int(int data);
    void UInt(UINT data);
    void Float(float data);
    void String(string data);
    void WString(wstring data);
    void Matrix(XMMATRIX data);

    void Byte(void* data, UINT dataSize);

private:
    HANDLE file;
    DWORD size = 0;
};