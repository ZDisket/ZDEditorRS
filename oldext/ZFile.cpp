#include "ZFile.h"

using namespace std;
int ZFile::EZFOpenModeToIos(const EZFOpenMode::Enum & input)
{
	if (input == EZFOpenMode::BinaryRead)
		return ios::in | ios::binary;
	else if (input == EZFOpenMode::BinaryWrite)
		return ios::out | ios::binary;
	else if (input == EZFOpenMode::TextRead)
		return ios::in;
	else if (input == EZFOpenMode::TextWrite)
		return ios::out;

	return ios::in | ios::binary;

}
bool ZFile::Open(const std::string & in_sFileName, const EZFOpenMode::Enum & in_Mode)
{
	OpenMode = in_Mode;

	Stream.open(in_sFileName, EZFOpenModeToIos(in_Mode));
	return Stream.good();

}

bool ZFile::Open(const std::wstring & in_sFileName, const EZFOpenMode::Enum & in_Mode)
{
	OpenMode = in_Mode;
	Stream.open(in_sFileName, EZFOpenModeToIos(in_Mode));
	return Stream.good();
}

void ZFile::Seek(const INT64 & in_Pos)
{
	if (OpenMode == EZFOpenMode::BinaryRead || OpenMode == EZFOpenMode::TextRead)
		Stream.seekg(in_Pos, ios::beg);
	else if (OpenMode == EZFOpenMode::BinaryWrite || OpenMode == EZFOpenMode::TextWrite)
		Stream.seekp(in_Pos, ios::beg);
}

INT64 ZFile::GetPos()
{
	if (OpenMode == EZFOpenMode::BinaryRead || OpenMode == EZFOpenMode::TextRead)
		return Stream.tellg();
	else if (OpenMode == EZFOpenMode::BinaryWrite || OpenMode == EZFOpenMode::TextWrite)
		return Stream.tellp();

	// NO TYPE?????????????
	return -1;
}

void ZFile::Read(void * out, const INT64 & count)
{
	Stream.read((char*)out, count);
}

void ZFile::Write(void * in, const INT64 & incount)
{
	Stream.write((char*)in, incount);
	
}

void ZFile::Close()
{
	Stream.close();
}

ZFile::ZFile()
{
}


ZFile::~ZFile()
{
}
