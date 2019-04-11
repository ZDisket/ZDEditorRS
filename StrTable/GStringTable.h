#pragma once

/**************************************************************
*
* GStringTable.h
*
* Description
* ===========
* Allows easy manipulation and loading of string tables.
*  
*
* Owner
* =====
*  ZDisket
*
* Copyright  (C) 2018, YOUR MOM GAY LOLOLOL
***************************************************************/

#include <vector>
#include <string>
#include "golem_string.h"
#include "ext/Signal.h"
using namespace Gallant;
class GIndex_Entry
{
public:
	GIndex_Entry()
	{
	}

	GIndex_Entry(const GString &inValue)
	{
		mOffset = 0;
		mString = inValue;
		mSize = mString.size() * 2;
	};

	int mOffset;
	int mSize;

	GString mString;
};

struct StrEntry {
	int ID;
	GIndex_Entry Entry;

	StrEntry(const StrEntry& in_cNewEntry) {
		ID = in_cNewEntry.ID;
		Entry = in_cNewEntry.Entry;
	}
	StrEntry() {

	}
	StrEntry(const int& in_enID, const GIndex_Entry& in_eEntry) {
		ID = in_enID;
		Entry = in_eEntry;
	}

	StrEntry(const int& in_enID, const GString& in_Str) {
		ID = in_enID;
		Entry = GIndex_Entry(in_Str);
	}
	void SetString(const GString& in_Str) {
		Entry.mString = in_Str;
		Entry.mSize = Entry.mString.size() * 2;

	}
	GString GetString() { return Entry.mString; }

	// For std::sort
	bool operator < (const StrEntry& in_entr) {
		return (ID < in_entr.ID);
	}
};

class GStringTable
{
private:
	std::vector<StrEntry> m_Data;
	int lNBIndex;
	int m_iProgress;

public:
	GStringTable();

    Signal1<int> UpdateProg;
    Signal1<int> UpdateRangeMax;
	// Load a string table file
	bool Load(const std::string& in_gstPath);

	// Save a string table file
	bool Save(const std::string& in_gstSavePath);

	std::vector<StrEntry>& Data() { return m_Data; }

	void Add(const StrEntry& in_Entry);

	int Progress() { return m_iProgress; }
	int getNBIndex() { return lNBIndex; }
	~GStringTable();
};

