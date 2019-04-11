/**************************************************************
*
* golem_row.h
*
* Description
* ===========
*  Describes the interface of class GRow
*
* Owner
* =====
*  Nicolas Hatier
*
* Copyright  (C) 2003, Laboratoires Golemlabs Inc.
***************************************************************/

#ifndef _GOLEM_ROW_H_
#define _GOLEM_ROW_H_

#include "golem_db_cell.h"

namespace DB
{
   /*!
   * Table row. This class does almost everything for the table.
   **/
   class GRow
   {
   public:
      //! Returns the Cell at position
      GCell * Cell(UINT32);
      const GCell * Cell(UINT32) const;

      //! Returns the Cell named
      GCell * Cell(const GSString &);
      const GCell * Cell(const GSString &) const;

      ~GRow();
   private:
      friend class GTable;

      //!Only the Table can construct Rows
      GRow(const GTable *, UINT32);      

      // A Row can't be copied
      GRow(const GRow &);
      GRow & operator = (const GRow &);

      // Cells vector
      vector<GCell> m_Cells;

      // Pointer to raw Row data 
      INT8 *       m_pData;      
   };
}

#endif //_GOLEM_ROW_H_
