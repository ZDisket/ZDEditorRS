#pragma once
#include <golem_db_firebird.h>
using namespace DB;
/*
====================================================================
|| ZTable.h
==========================
|| Description: Implements ZTable class and all its dependencies
|| ===
|| Author: ZDisket
||===================
|| Copyright (C) 2018 YOUR MOM GAY LOLOLOL.
|| 
==================================================================
*/



// ZDB:
/*
Each column has an ID, name, type and holds its items.

*/
namespace ZDB 
{

	struct ZItem {
		GString sData;
		size_t id;

		ZItem(const ZItem& item) {
			sData = item.sData;
			id = item.id;
		}

		ZItem(const GString& inDat, const size_t& inID) {
			sData = inDat;
			id = inID;
		}

		ZItem() {
			sData = L"";
			id = NULL;
		}

	};


	// ZColumn represents a column within the table.
	// The structure and native handling of the table is
	//** Table->Columns->ColItems
	// Rows, for all intents and purposes, do not exist. Items are handled directly from the columns.
	struct ZColumn {
		GString name;
		size_t id;
		EDataType dataType;
		std::vector<ZDB::ZItem> vItems;
		ZColumn(const GString& sName, const size_t& inID, const EDataType& type = EDataType::e_Unknown) {
			name = sName;
			id = inID;
			dataType = type;
		}
		// Get reference to items vector
		std::vector<ZDB::ZItem>& Items(){ return vItems; }

		size_t get_MaxLength() {
			if (!vItems.size())
				return 0;

			std::vector<ZDB::ZItem>::iterator It = vItems.begin();
			size_t maxlen = 0;
			while (It != vItems.end())
			{
				if (It->sData.length() > maxlen)
					maxlen = It->sData.length();

				++It;
			}

			return maxlen;
		}
		ZColumn(const ZColumn& col) {
			name = col.name;
			id = col.id;
			dataType = col.dataType;
			vItems = col.vItems;
		}
		ZColumn() {
			name = L"";
			id = NULL;
			dataType = EDataType::e_Unknown;
		}
	};
	struct ZRowItem {
		short RowID;
		// inherits from column
		EDataType DataType;
		GString sData;


		// Assemble a ZRowItem from a column item with ID provided.
		ZRowItem(const short& inID,const ZDB::ZItem& zItem, const ZDB::ZColumn& inCol)
		{
			DataType = inCol.dataType;
			RowID = inID;
			sData = zItem.sData;

		}

		ZRowItem(const short& inrID, const EDataType& inDat, const GString& insDat)
		{
			RowID = inrID;
			DataType = inDat;
			sData = insDat;

		}
		ZRowItem(const ZRowItem& Tem)
		{
			RowID = Tem.RowID;
			DataType = Tem.DataType;
			sData = Tem.sData;

		}
		ZRowItem() {
			RowID = NULL;
			DataType = EDataType::e_Unknown;
			sData = L"";
		}

	};
	struct ZRow {
		vector<ZDB::ZRowItem> m_vCells;

		vector<ZDB::ZRowItem>& Cells() { return m_vCells; }
		// Get a const reference.
		const vector<ZDB::ZRowItem>& c_Cells() { return m_vCells; }

		void Clear() {
			m_vCells.clear();
		}
		ZRow() {

		}
		ZRow(const ZRow& r) {
			m_vCells = r.m_vCells;
		}
	};
	
}

typedef std::vector<ZDB::ZColumn>::iterator ColIt;
typedef std::vector<ZDB::ZItem>::iterator ZItemIt;
/*
||| ZTable: Class to serve as flexible and easy to use table storage.
|| All ZTable records are stringized.
|| The table internally handles itself with column items, but rows can be summoned for convenience.
|| ============================
|| Author: ZDisket
*/
class ZTable
{
private:
	std::vector<ZDB::ZColumn> m_vColumns;
	GString Name;

	inline void _fromGTable(const GTable& gtab);
public:
	ZTable();
	ZTable(const ZTable& ctab);
	// Assemble a ZTable from a GTable object.
	ZTable(const GTable& gtab);

	// Get the total amount of items on all columns.
	unsigned long GetNItems();
	ZTable(const GTable& gtab, const GString& in_Name);

	// Returns a vector of rows created when the function is called.
	// FUNCTION PROVIDED FOR CONVENIENCE ONLY; CAN'T MODIFY A ZTABLE WITH ROWS.
	vector<ZDB::ZRow> MakeRows();

	size_t SzColumns() { return m_vColumns.size(); }

	GString GetName() { return Name; }
	void SetName(const GString& in_sName, bool Trim = true);
	size_t GetItems(std::vector<ZDB::ZItem>& out_vItems);
	std::vector<ZDB::ZColumn>& Columns() { return m_vColumns; }
	~ZTable();
};

