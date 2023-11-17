#include "Framework.h"
#include "BinaryWriter.h"

BinaryWriter::BinaryWriter(string filePath)
{
    // FILE* fp;   general
    // std???      std c++
    // winAPI      file

    file = CreateFileA(filePath.c_str(), GENERIC_WRITE,
        0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

     assert(file != nullptr);
}

BinaryWriter::BinaryWriter(wstring filePath)
{
    file = CreateFile(filePath.c_str(), GENERIC_WRITE,
        0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    assert(file != nullptr);
}

BinaryWriter::~BinaryWriter()
{
    CloseHandle(file);
}

void BinaryWriter::Int(int data)
{
    // fwrite(&data,sizeof(int),1,fp);
    WriteFile(file, &data, sizeof(int), &size, nullptr);
}

void BinaryWriter::UInt(UINT data)
{
    WriteFile(file, &data, sizeof(UINT), &size, nullptr);
}

void BinaryWriter::Float(float data)
{
    WriteFile(file, &data, sizeof(float), &size, nullptr);
}

void BinaryWriter::String(string data)
{
    UInt(data.size());

    const char* str = data.c_str();
    WriteFile(file, str, sizeof(char) * data.size(), &size, nullptr);
}

void BinaryWriter::WString(wstring data)
{
    UInt(data.size());

    const WCHAR* str = data.c_str();
    WriteFile(file, str, sizeof(WCHAR) * data.size(), &size, nullptr);
}

void BinaryWriter::Matrix(XMMATRIX data)
{
    WriteFile(file, &data, sizeof(XMMATRIX), &size, nullptr);
}

void BinaryWriter::Byte(void* data, UINT dataSize)
{
    // struct TDATA
    // {
    //    int x;
    //    int y;
    // }
    // fwrite(&data,sizeof(TDATA),1,fp);


    WriteFile(file, data, dataSize, &size, nullptr);
}
