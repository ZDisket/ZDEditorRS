/**************************************************************
*
* golem_db_table_holder.h
*
* Description
* ===========
* The GTableHolder is a semi-dynamic structure that allow table caching, to reduce the amount of data 
* transferred with the server, and to minimize internal data copy, and it's designed to be very efficient.
* This class has a fixed amount of tables in it (an array, allocated on the heap, no new(), no delete()). 
* This fixed amount is the template parameter
* When you need a table, you just ask it for one, with a name (any name, define your own), and it returns you one 
* reference to a table you can use for what you want. If you need it later, you can reask the TableHolder for it
* (use the same name), and it will return the same one. If the holder runs out of table, it will take the "older" 
* one, clear it, deassociate it from its name an reassociate it to the new name you gave. This allow a very nice 
* Most Recent Used implementation for, by instance, swapping between two countries in the Contries information 
* window, without refetching data each time. 
* 
* I hope this is clear now.
*
* Don't keep a pointer to the returned class when you exit the function that called GetData. Reask the table later.
*
* This class is not 100% multithread safe, however, the risk is minimal if you create an instance of it with a 
* reasonable template argument
*
* If you're not satisfied with the date you get doing GetTable and need to refetch table's data, Issue a ReleaseTable
* and reask GetData before doing so, or the date won't be updated.
*
* Owner
* =====
*  Nicolas Hatier
*
* Copyright  (C) 2003, Laboratoires Golemlabs Inc.
***************************************************************/
#ifndef _GTable_Holder_H_
#define _GTable_Holder_H_

#include <map>
#include "golem_db_table.h"

namespace DB
{

   // Internal class to be able to put code in the C++. If it was 
   // implemented in the template below, all would be in the .h
   class GTableHolderInternal
   {
   public:
      /*! 
      * Always returns a reference to a GTable
      * If the table (name: param 1) has been requested before, the same table is returned, containing the 
      * previous data. The second parameter is set to true if the table has been requested before, and false if
      * it has not (empty). The third parameter is set to the last request time of the table
      * If the class has no more free tables, it clears the oldest one and returns it with (bool) = false. 
      */

      GTable & GetTable(GString, bool * = NULL, GDateTime * = NULL);

      //! Release the usage of a table. 
      bool ReleaseTable(GString);
      
      UINT32 TablesUsed();
      UINT32 MaxTables();

   protected:
      // Only the below template can instanciate the class
      GTableHolderInternal(UINT32, GTable *, bool *);      

   private:
      
      const UINT32 m_iMaxCount;

      // Internal data
      struct TableData
      {
         UINT32 m_TableID;
         GDateTime m_TimeCreated;
      };   

      GTable * m_pTables;
      bool   * m_pTableUsed;
      UINT32   m_iTablesUsed;

      std::map<GString, TableData> m_pTablesMap;

      // Table can't be copied
      GTableHolderInternal(const GTableHolderInternal &);
      GTableHolderInternal & operator = (const GTableHolderInternal&);
   };


   /*
      TableHolder (see GTableHolderInternal description)
      This template allocates T tables for storage.
   */
   template <UINT32 T>
   class GTableHolder : public GTableHolderInternal
   {
      GTable m_pTablesArray[T];
      bool m_pTableUsedArray[T];

   public:
      GTableHolder() 
         : GTableHolderInternal(T, m_pTablesArray, m_pTableUsedArray) {}
   };

}

#endif //_GTable_Holder_H_
