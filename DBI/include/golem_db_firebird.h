/**************************************************************
* Header file : golem_DB_firebird.h
* Created on  : 2003/04/22
* Owner	   : François Durand
*
* Utility	   : Header file for golem_DB_firebird.cpp,
*			     it contains firebird api object declaration.
**************************************************************/
#ifndef _GOLEM_FIREBIRD_DB_BINARY_H_
#define _GOLEM_FIREBIRD_DB_BINARY_H_

#include "ibase.h"
#include "database.h"
#include "golem_string.h"
#include "golem_date_time.h"

const int DB_ALIGNMENT_LENGTH = 12;
#define FB_ALIGN(n,b) ((n + b - 1) & ~(b - 1))

namespace DB
{
   class GFirebird : public GDatabase
   {
   public:
      //Main function declaration

      //Constructor
      GFirebird();			
      //Destructor
      ~GFirebird();

      bool StartTransaction();
      EError CommitTransaction();
      bool RollbackTransaction(const char * in_sErrormsg = NULL);

      EError QueryTransaction(const GSString & in_sSQLQuery, GTable &);

      EError CreateDatabase(const GSString & in_sDatabaseName);
      EError DeleteDatabase();

      //Database initialisation function
      EError Init(const GSString & in_sFilename);

      //Function used to execute query (call private query func.)
      EError ExecuteQuery(const GSString & in_sSqlQuery);
		EError ExecuteQueryDuringTransaction(const vector<GSString>& in_vSqlQueries, GTable & out_Table);
      EError ExecuteQuery(const GSString & in_sSqlQuery, GTable &);      
      
      //Database shutdown function
      EError Shutdown();

      //Convert FB data type to our type
      static EDataType FB2Golem(short in_iType);

      //Convert our data type to FB type
      static short Golem2FB(EDataType in_eType);


   private:  

      // SQL Commands
      enum ESQLCommand
      {
         SQL_INVALID,
         SQL_CREATE,
         SQL_DROP,
         SQL_DELETE,
         SQL_INSERT,
         SQL_UPDATE,
         SQL_SELECT,      
         SQL_SUBCOMMAND_TABLE,
         SQL_SUBCOMMAND_DATABASE,
         SQL_OTHER
      };

      // SQL Commands conversion   

      ESQLCommand SQLCommand(const GSString & in_sCommand);
      ESQLCommand SQLSubCommand(const GSString & in_sCommand);

      //Main sub-function declaration

      EError Query   (const char *, const char * ErrorText, EError ErrorCode); 
      EError Select  (const char *, GTable &); //Select query
      

      //Utils sub-functions declaration

      //Report and log a firebird error
      EError ReportFbError(const GSString & in_sEvent, EError in_eErrNumber = DB_UNKNOWN_ERROR);

      //Report and log an error
      EError ReportError(const GSString & in_sError, EError in_eErrNumber = DB_INVALID_QUERY);

      //Generate a record from an sqlda
      void CreateRowFromSQLDA(XSQLDA* in_pSQLDA, GTable & out_Table);
      
      //Main member declaration
      isc_db_handle	m_DB;			         //Database handler
      isc_tr_handle  m_Transaction;       //Current transaction
      long           m_pStatusVector[20];  //Status vector

      std::map<GSString, ESQLCommand> m_SQLCommands; // Fast SQL Commands mapping
   };
}
   
#endif