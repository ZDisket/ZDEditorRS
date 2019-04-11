#pragma once
#include <golem_db_firebird.h>
#include "ZTable.h"
#include "ZDataIndex.h"
using namespace DB;
/*
====================================================================
|| DBManager.h
==========================
|| Description: Serves as the central for all database communications.
|| ===
|| Author: ZDisket
||===================
|| Copyright (C) 2018 YOUR MOM GAY LOLOLOL.
||
==================================================================
*/


typedef void(*vifunc_t)(const int& in); // pointer to function with 1 int arg and void return

//#define GSQLQUERY(q,param) q + GString(SQUOTE) + GString(param).ToUpper() + SQUOTE 
#define IF_ERR(call) if(call != DB::DB_NO_ERROR)
#define _TRY1(call) if (call != DB::DB_NO_ERROR) {CancelSave(); throw new std::exception("T1ERR");}
#define _TRY2(call, err) if (call != DB::DB_NO_ERROR) {CancelSave(); return err;}
#define _RET_IF_ERR(call, err) if (call != DB::DB_NO_ERROR) {CancelSave(); return err;}
#define _RET_ERR_IF_ERR(call) if(call != DB::DB_NO_ERROR) return DBError::err_internal_error
#define _t(type) __typeof(type)
#define IFAIL_RET(call,ret) if (!call){CancelSave();return ret;}
#define __T1(call) if (call != DBError::err_no_error){CancelSave(); throw new std::exception("OH SHIT");}
#define IERR_RET(call,ret) if (call != DBError::err_no_error){CancelSave(); return ret;}

// adjust as desired
#ifndef DBMAN_FORCEWIDE
// VString is a GSString (single byte char), which works natively with the database/Firebird engine, unless DBMAN_FORCEWIDE is defined.
#define VString GSString
#define SQUOTE "\""
#else
#define VString GString
#define SQUOTE L"\""
#endif
namespace DBError {
	enum DBError
	{
		err_no_error = 0,
		err_database_error,
		err_unknown,
		err_internal_error,
		err_invalid_data_in_record,
		err_invalid_data_in_index,
		err_unknown_table,
		err_execute_query,
		err_cant_begin_saving,
		err_saving_table
	};
}
/*
* DBManager: A class to simplify interaction with GolemLabs SP2 Database.
*  Ported to pure C++ from GolemLabs code.

*/
class DBManager
{
private:
	// They told me I could be anything, so I became a database.
	GFirebird * m_Database;
	VString m_File;
	void CancelSave();
	VString m_CurrentTableRealName;
	VString m_CurrentTableTempName;
	bool m_NewTable;

	VString TypeToString(const DB::EDataType& inType);
	void ReplaceAll(VString& str, const VString& from, const VString& to) {
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != VString::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		//return str;
	}
	DBError::DBError DoneSave();
	// Query
	inline VString GSQLQUERY(const VString& query, const VString& param);

	// Query without quoting the parameter.
	inline VString GSQLQUERY_NQ(const VString& query, const VString& param);

	int pgPos;
	int pgMax;

	vifunc_t cMax;
	vifunc_t cPos;
public:
	DBManager();
	EError Exec(const bool& Immediate, const VString& Query);
    EError Exec(const bool& Immediate, const VString& Query,GTable& out_table );

	void DeleteTable(const VString& in_sTableName);

	std::vector<VString> TableNames();

	
	void Shutdown();

	// Initialize the database, with given file name.
	bool Init(const VString& in_sFileName);

	// Save a table. Automatically fetches indices.
	DBError::DBError Save(ZTable& in_table);

	// Save a table, with provided indices.
	DBError::DBError Save(ZTable& in_table, std::vector<ZDB::DataIndex>& in_indices);
	
	// Get a table and get it directly in ZTable
	ZTable GetZTable(const VString& in_sTableName);

	// Add a record.
	// The row doesn't get modified, VS whining about const references again.
	DBError::DBError AddZTRecord(ZDB::ZRow& inRow);
	DBError::DBError RenameTable(const VString& OldTableName, const VString& NewTableName);

	// Add an index.
	// Note that the index doesn't get modified, it's just VS whining when its a const refernece.
	DBError::DBError AddIndex(ZDB::DataIndex& Idx);
	
	// Get a table in GTable format.
	// Specify name and reference to out table.
	// Note: if you want a deserialized/ZTable, use GetZTable()
	EError GetTable(const VString& in_tableName,GTable& table_out);
	vector<ZDB::DataIndex> Indices(const VString& in_sTableName);

	const int& GetPGPos() { return pgPos; }
	const int& GetPGMax() { return pgMax; }
	void SetPGPosF(vifunc_t pfunc) { cPos = pfunc; }
	void SetPGMaxF(vifunc_t pfuncm) { cMax = pfuncm; }
	~DBManager();
};

