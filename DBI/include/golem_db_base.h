/**************************************************************
*
* golem_db_base.h
*
* Description
* ===========
*  Basic database types
*
* Owner
* =====
*  Nicolas Hatier
*
* Copyright  (C) 2003, Laboratoires Golemlabs Inc.
***************************************************************/


#ifndef _GOLEM__H_
#define _GOLEM__H_

namespace DB
{
   enum EDataType
   {
      e_Char,		//char[]
      e_Decimal,	//_int64
      e_Double,	//double
      e_Float,	   //float
      e_Int,		//long
      e_Numeric,	//_int64
      e_SmallInt,	//small int (INT16)
      e_Timestamp,//timestamp (isc_timestamp)
      e_Varchar,	//char[] (sql_varying)
      e_Unknown
   };
}
#endif //_GOLEM__H_
