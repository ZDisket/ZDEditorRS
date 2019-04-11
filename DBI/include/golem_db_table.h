/**************************************************************
*
* golem_table.h
*
* Description
* ===========
*  Describes the interface of class GTable
* 
*  To be clear, a Recordset and a Table is the same thing with two different names. 
*  I know that most database engines call stored structured data "Tables", and structured data 
*  transferred in response of a query "Recordset". However, it's the exact same thing. 
*  I used the word Table when creating the new database to be sure to not forget conversion of parts 
*  of the previous code which used "Recordset". It's discutable, I know. The re-conversion to 
*  Recordset or something else, if needed, should take less than 3 minutes... Search and replace...
*
* Owner
* =====
*  Nicolas Hatier
*
* Copyright  (C) 2003, Laboratoires Golemlabs Inc.
***************************************************************/

#ifndef _GOLEM_TABLE_H_
#define _GOLEM_TABLE_H_

#include "golem_db_base.h"
#include "golem_db_col.h"
#include "golem_db_row.h"

namespace DB
{
   /*!
   * GTable. Main class of the database storage interface. 
   *  This class can't be copied. It must be passed by reference, to avoid
   *  unneeded data creation.
   **/
   class GTable 
   {
   public:
      GTable() {}
      ~GTable();

      /*! 
         Adds a new column to the table, and returns a pointer to it. Specify the 
         name, the type; if the type is string, specify the size.
         Table must be empty
      */
      GCol * NewCol(const GSString &, EDataType, UINT32 = -1);

      //! Removes a column from the table. Table must be empty
      void DeleteCol(UINT32);
      void DeleteCol(const GSString &);

      //! Return Column at position
      GCol * Col(UINT32);
      const GCol * Col(UINT32) const;

      //! Return Column named
      GCol * Col(const GSString &);
      const GCol * Col(const GSString &) const;

      //! Adds a new row to the table and returns its pointer. At least one col must be defined.
      GRow * NewRow();

      //! Removes Row at position
      void   DeleteRow(UINT32);

      //! Returns Row pointer at position
      GRow * Row(UINT32);
      const GRow * Row(UINT32) const;

      //! Returns Row Count
      UINT32 RowCount() const;
      
      //! Returns Col Count
      UINT32 ColCount() const;

      //! Empties the table
      void Clear();

   private:
      //! Called by NewCol and Unserialize. Creates a Col without verification
      GCol * NewColInternal(const GSString &, EDataType, UINT32);

      vector<GRow*> m_Rows;
      vector<GCol> m_Cols;
      
      UINT32 m_RowSize;
      
      //! Table can't be copied.
      GTable(const GTable &);
      GTable & operator = (const GTable &);
   };
}

#endif //_GOLEM_TABLE_H_

