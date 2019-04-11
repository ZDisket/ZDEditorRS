/**************************************************************
*
* golem_cell.h
*
* Description
* ===========
*  Describes the interface of class GCell
*
* Owner
* =====
*  Nicolas Hatier
*
* Copyright  (C) 2003, Laboratoires Golemlabs Inc.
***************************************************************/

#ifndef _GOLEM_CELL_H_
#define _GOLEM_CELL_H_


namespace DB
{
   class GTable;
   class GRow;
   class GCol;

   /*!
   * Cell contents accessor. This class doesn't store data by itself, 
   * it uses it's parent row's buffer and it's parent Col data
   **/   
   class GCell
   {
   public:
      //! Returns a pointer to the Raw data of the cell
      const INT8 * Data() const;

      //! Assigns the data to the cell. Uses the data size
      void   Data(INT8 *);

      //! Assigns the data to the cell. Uses the given size, up to the internal data sizes. If smaller, fills the remaining with Zeroes.
      void   Data(INT8 *, UINT32);

      //! Byte size of the Cell (ask the Col)
      UINT32            Size() const;

      //! Datatype of the Cell (ask the Col)
      EDataType         Type() const;

      //! Name of the Cell (ask the Col)
      const GSString &  Name() const;
      
      //! Cell content is Null (special flag)
      bool            Null() const;
      void            Null(bool);

      //! Assigns arbitrary data to the cell, using template and data size
      bool                    SetData(const char *);            
      bool                    SetData(const GSString &);
      bool                    SetData(const GDateTime &);
      template<class T> bool  SetData(const T & in_Value)
      {
         // size matters. when it doesn't fits, no set at all.
         if (sizeof(T) != Size()) return false;
         //Here NULL is not set since a Reference can't be NULL.
         //To set NULL, use Data(INT8 *).
         Data((INT8*)&in_Value);
         return true;
      }

      //! Gets arbitrary data from the cell, using template and data size
      bool                    GetData(GSString &) const;
      bool                    GetData(GDateTime &) const;
      template<class T> bool  GetData(T & out_Value) const
      {   
         // size matters. when it doesn't fits, no set at all.
         if (sizeof(T) != Size()) return false;
         if (Null()) return false;

         // cast the internal buffer in the out type
         out_Value = *(T*)Data();
         return true;
      }
	  //VS2010: Public constructor for vector<GCell>
	  GCell();
   private:
      friend class GRow;
      //friend class vector<GCell>;

      //! Constructor. Only the GRow can create its cells.
      GCell(INT8*, const GCol *, INT8 * in_Null);
      //GCell();

      const GCol * m_pCol;
      INT8       * m_pData;
      INT8       * m_pNULL;
   };
}
#endif //_GOLEM_CELL_H_
   