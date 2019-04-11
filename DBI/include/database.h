/**************************************************************
*
* database.h
*
* Description
* ===========
*  Database interface, to access the database itself
*
* Owner
* =====
*  Nicolas Hatier
*
* Copyright  (C) 2003, Laboratoires Golemlabs Inc.
***************************************************************/

#ifndef _GOLEM_DATABASE_INCLUDE_H_
#define _GOLEM_DATABASE_INCLUDE_H_

#include "base.h"
#include "golem_db_table_holder.h"
#include "golem_db_table.h"


namespace DB
{

enum EError
{
	//Basic error definition
	DB_NO_ERROR = 1,					      //No error occured
	DB_UNKNOWN_ERROR = 0,				   //An unknown error as occured
	DB_NOT_IMPLEMENTED = 2,				   //Not implemented function
   DB_NOT_INITIALIZED,
   DB_ALREADY_INITIALIZED,

	//Extended error definition
	DB_INVALID_INIT_PARAMETERS,		   //Database init function received invalid args
	DB_FB_DETACH_ERROR,					   //Error detaching database handler
	DB_FB_ATTACH_ERROR,					   //Error attaching database handler
	DB_FB_DB_ALREADY_EXIST,				   //Database already exist
	DB_FB_ERROR_DROPPING_DATABASE,	   //Can't delete database
	DB_FB_INVALID_CREATE_TABLE_QUERY,   //Can't create table
	DB_FB_INVALID_DELETE_TABLE_QUERY,   //Can't delete table
   DB_FB_NO_TRANSACTION_STARTED,       // No transaction has been started, use Start_Transaction
	DB_INVALID_QUERY,					      //The query given to ExecuteQuery() is invalid
	DB_FB_ERROR_FETCHING,				   //Error occured while fetching table results
	DB_FB_ERROR_EXECUTE_QUERY,			   //Error while executing query, check query syntax
	DB_FB_ERROR_RESQUESTING_INFO,		   //Error while requesting information on a query
	DB_FB_ERROR_START_TRANSACTION,	   //Error while starting a transaction
	DB_FB_ERROR_ALLOCATE_STATEMENT,	   //Error allocating statement (stmt)
	DB_FB_ERROR_PREPARE_QUERY,			   //Error preparing query   
	DB_ERROR_QUERY_NOT_SUPPORTED,		   //Error preparing query, check manual for information on insert query
	DB_FB_ERROR_COMMITING_TRANSACTION,   //Error occured while applying the transaction check your query
   DB_FB_DUPLICATE_UNIQUE_INDEX_ENTRY,  //An entry uniquely indexed is duplicate
   DB_QUERY_TIMEOUT
};

class GDatabase
{
	public:
		virtual bool StartTransaction() = 0;
		virtual EError CommitTransaction() = 0;

		virtual EError Init(const GSString & in_sFilename) = 0;
		virtual EError Shutdown() = 0;      
      
      virtual EError ExecuteQuery(const GSString & in_sSqlQuery) = 0;
		virtual EError ExecuteQueryDuringTransaction(const vector<GSString>& in_vSqlQueries, GTable & out_Table) = 0;
      virtual EError ExecuteQuery(const GSString & in_sSqlQuery, GTable &) = 0;      

      virtual EError CreateDatabase(const GSString & in_sDatabaseName) = 0;
      virtual EError DeleteDatabase() = 0;

      //! Create a query for an update statement
		static GSString CreateUpdateQuery(const vector<pair<GSString,GSString> >& in_Vector, const GSString& in_sTable, const GSString& in_sCondition);
};

}


#endif

