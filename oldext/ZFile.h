#pragma once

//* ZFile: Class for reading files
#include <fstream>
#include <string>
#ifndef BYTE
#define BYTE unsigned char
#endif // !BYTE

#ifndef INT64
#define INT64 long long
#endif // !INT64



namespace EZFOpenMode {
	enum Enum {
		BinaryRead = 0,
		TextRead,
		BinaryWrite,
		TextWrite
	};
}
namespace DataSizes {
	enum Sz {
		FLOAT_SZ = 4,
		DOUBLE_SZ = 8,
		LONG_DOUBLE_SZ = 10,
		SIZE_T_SZ = 4,
		INT64_SZ = 8,
		LONG_SZ = 4,
		INT32_SZ = 4,
		INT16_SZ = 2,
		CHAR_SIZE = 1,
		WCHAR_SIZE = 2
	};
}
class ZFile
{
private:
	BYTE * m_pData;
	bool FileOpened;
	std::fstream Stream;

	EZFOpenMode::Enum OpenMode;

	int EZFOpenModeToIos(const EZFOpenMode::Enum& input);

public:
	bool Open(const std::string& in_sFileName,const EZFOpenMode::Enum& in_Mode);
	bool Open(const std::wstring& in_sFileName, const EZFOpenMode::Enum& in_Mode);

	void Seek(const INT64& in_Pos);
	INT64 GetPos();
	// Reads from the file
	// Please pass a pointer to this
	void Read(void* out, const INT64& count);
	// Writes to the file
	// Please pass a pointer
	void Write(void* in, const INT64& incount);

	// Write with template argument to not pass size.
	// Only works with Plain Old Datatypes (PODs)
    template <typename Dat>
	void Write(const Dat& dta)
	{
		Stream.write((char*)&dta, sizeof(dta));
		
	
	}

    // Read with template argument to not pass size.
    // Only works with Plain Old Datatypes (PODs)
    template <typename Dat>
    void Read(Dat& out_dta)
    {
        Stream.read((char*)&out_dta, sizeof(out_dta));


    }
	void Close();


	ZFile();
	~ZFile();
};

