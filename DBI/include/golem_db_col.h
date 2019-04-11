/**************************************************************
*
* golem_col.h
*
* Description
* ===========
*  Describes the interface of class GCol
*
* Owner
* =====
*  Nicolas Hatier
*
* Copyright  (C) 2003, Laboratoires Golemlabs Inc.
***************************************************************/

#ifndef _GOLEM_COL_H_
#define _GOLEM_COL_H_

namespace DB
{
   /*!
   * Data Table Column. This class defines the different properties of a table layout
   *  Pretty straightforward
   **/
   class GCol
   {
   public:
      EDataType        Type() const {return m_iType;}
      UINT32           Size() const {return m_iSize;}
      UINT32          Index() const {return m_iIndex;}
      const GSString & Name() const {return m_sName;}      
            GSString & Name()       {return m_sName;}

   private:
      friend class GTable;

      //! Only the Table can construct its cols
      GCol(const GSString &, EDataType, UINT32, UINT32);
      GSString  m_sName;
      EDataType m_iType;
      UINT32    m_iSize;
      UINT32    m_iIndex;
   };
}

#endif //_GOLEM_COL_H_
