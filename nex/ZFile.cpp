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
ZFile::ZFile(const std::wstring & coFileName, const EZFOpenMode::Enum & Mode)
{
	Open(coFileName, Mode);
}
ZFile::ZFile(const std::string & coFName, const EZFOpenMode::Enum & coMode)
{
	Open(coFName, coMode);
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

INT64 ZFile::GetFileLength()
{
	std::streampos lpos = GetPos();

	if (OpenMode == EZFOpenMode::BinaryRead || OpenMode == EZFOpenMode::TextRead)
		Stream.seekg(0, Stream.end);
	else if (OpenMode == EZFOpenMode::BinaryWrite || OpenMode == EZFOpenMode::TextWrite)
		Stream.seekp(0, Stream.end);

	const INT64 Len = GetPos();
	Seek(lpos);

	return Len;

}

void ZFile::Read(void * out, const INT64 & count)
{
	Stream.read((BYTE*)out, count);

}

void ZFile::Write(void * in, const INT64 & incount)
{
	Stream.write((BYTE*)in, incount);
	
}

ByteArr ZFile::ReadEntireFile()
{

	ByteArr ArrRet;

	Stream.seekg(0, Stream.end);
	INT64 length = Stream.tellg();
	Stream.seekg(0, Stream.beg);
	ArrRet.CAlloc(length);

	Stream.read(ArrRet.GetData(), length);
	
	return ArrRet;

}

void ZFile::Write(const ByteArr & BrDat)
{
	Stream.write(BrDat.CoData(), BrDat.Size());
}

void ZFile::Close()
{
	Stream.close();
}

void ZFile::operator>>(FAttrib& ExAtr) {
    Read(ExAtr.Archive);
    Read(ExAtr.Compressed);
    Read(ExAtr.Hidden);
    Read(ExAtr.Normal);
    Read(ExAtr.ReadOnly);
    Read(ExAtr.System);
    Read(ExAtr.Temporary);
}

void ZFile::operator >>(SYSTEMTIME& SysTime) {
	// Convert it to file time to export easier;
	FILETIME TimeC;

	(*this) >> TimeC.dwHighDateTime;
	(*this) >> TimeC.dwLowDateTime;

	FileTimeToSystemTime(&TimeC, &SysTime);


}
void ZFile::operator<<(const FAttrib& Atr) {
	Write(Atr.Archive);
	Write(Atr.Compressed);
	Write(Atr.Hidden);
	Write(Atr.Normal);
	Write(Atr.ReadOnly);
	Write(Atr.System);
	Write(Atr.Temporary);


}
void ZFile::operator<<(const SItemW& ItemEx) {
	// Write our attributes
	(*this) << ItemEx.Attributes;

	// Write basic data

	Write(ItemEx.FileSzHigh);
	Write(ItemEx.FileSzLow);
	Write(ItemEx.IType);

	(*this) << ItemEx.LastAccessTime;
	(*this) << ItemEx.LastWriteTime;

	(*this) << ItemEx.Name;
	(*this) << ItemEx.TimeOfCreation;

	// Write subentries
	(*this) << ItemEx.SubEntries;


}


void ZFile::operator<<(const SYSTEMTIME& SysTime) {
	// Convert it to file time to export easier;
	FILETIME TimeC;
	SystemTimeToFileTime(&SysTime, &TimeC);

	(*this) << TimeC.dwHighDateTime;
	(*this) << TimeC.dwLowDateTime;

}
void ZFile::operator>>(SItemW& ItemEx) {
	// read our attributes
	(*this) >> ItemEx.Attributes;

	// read basic data

	Read(ItemEx.FileSzHigh);
	Read(ItemEx.FileSzLow);
	Read(ItemEx.IType);

	(*this) >> ItemEx.LastAccessTime;
	(*this) >> ItemEx.LastWriteTime;

	(*this) >> ItemEx.Name;
	(*this) >> ItemEx.TimeOfCreation;

	(*this) >> ItemEx.SubEntries;


}
void ZFile::operator>>(ByteArr& BarDat) {
	size_t BaSz = 0;
	Read(BaSz);
	BarDat.CAlloc(BaSz);
	Stream.read(BarDat.GetData(), BaSz);

}

ZFile::ZFile()
{
}


ZFile::~ZFile()
{
}
