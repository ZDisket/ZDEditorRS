#include "GStringTable.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
GStringTable::GStringTable()
{
	m_iProgress = 0;
}

bool GStringTable::Load(const std::string & in_gstPath)
{
	
	m_iProgress = 0;

	// If there's stuff then we clear
	if (m_Data.size())
		m_Data.clear();

	FILE *stream = fopen(in_gstPath.c_str(), "rb");
	// if error
	if (stream == NULL)
		return false;
	
	wchar_t lStr[4096];
	GIndex_Entry lData;

	fread(&lNBIndex, 4, 1, stream);
    UpdateRangeMax(lNBIndex);
    UpdateProg(0);
	int finid = 0;
	for (int c = 0; c < lNBIndex; c++)
	{
		int lID;

		fread(&lID, 4, 1, stream);
		fread(&lData.mOffset, 4, 1, stream);
		fread(&lData.mSize, 4, 1, stream);

		StrEntry tEntry(lID, lData);
		m_Data.push_back(tEntry);
		m_iProgress += c / 2;
        if (c != 0)
            UpdateProg(c / 2);

		finid = c;

	}
	finid /= 2;
	std::vector<StrEntry>::iterator sIt = m_Data.begin();
	int itID = 0;
	while (sIt != m_Data.end()) {
		fseek(stream, (*sIt).Entry.mOffset, SEEK_SET);

		if ((*sIt).Entry.mSize < 4096)
		{
			memset(lStr, 0, 4096);
			fread(&lStr, (*sIt).Entry.mSize, 1, stream);
			(*sIt).Entry.mString = lStr;
		}
		else {
            sIt->Entry.mString.reserve(sIt->Entry.mSize / 2);
            fread((void*)sIt->Entry.mString.data(),sIt->Entry.mSize,1,stream);

		}
		++itID;

        if (m_iProgress != itID)
            UpdateProg(finid + (itID / 2));

		++sIt;
	

	}
		

	fclose(stream);

	//std::sort(m_Data.begin(), m_Data.end());
	return true;
}

bool GStringTable::Save(const std::string & in_gstSavePath)
{
	// output results
	wchar_t lWStr[4096];
	lNBIndex = m_Data.size();
	FILE* stream = fopen(in_gstSavePath.c_str(), "wb");

	if (!stream)
		return false;

	fwrite(&lNBIndex, 4, 1, stream);
	int lPos = (lNBIndex * 12) + 4;

	m_Data.reserve(lNBIndex);
	m_iProgress = 0;
	// header
	int ida = 0;
    //m_pBar->SetRange32(0, lNBIndex);
	for (auto sIt = m_Data.begin(); sIt != m_Data.end(); sIt++)
	{
		(*sIt).Entry.mOffset = lPos;
		lPos += (*sIt).Entry.mSize;

		fwrite(&(*sIt).ID, 4, 1, stream);
		fwrite(&(*sIt).Entry.mOffset, 4, 1, stream);
		fwrite(&(*sIt).Entry.mSize, 4, 1, stream);



        if (ida != 0)
            UpdateProg(ida / 2);

		++ida;
		

	}
	int fid = ida / 2;
	ida = 0;
	// data
	for (auto sIt = m_Data.begin(); sIt != m_Data.end(); sIt++)
	{
		memset(lWStr, 0, 4096);
		for (int t = 0; t < ((*sIt).Entry.mSize / 2); t++)
			lWStr[t] = (*sIt).Entry.mString.at(t);

		fwrite(&lWStr, (*sIt).Entry.mSize, 1, stream);

        if (ida != 0)
            UpdateProg(fid + (ida / 2));

		++ida;
	}

	fclose(stream);
	return true;
}


void GStringTable::Add(const StrEntry & in_Entry)
{
	++lNBIndex;

	m_Data.push_back(in_Entry);
}

GStringTable::~GStringTable()
{
}
