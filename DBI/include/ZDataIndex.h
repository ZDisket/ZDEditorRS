#pragma once
#include <vector>
#include <golem_string.h>
#define _CGR const GString& 
namespace ZDB 
{

	class DataIndex
	{
	private:
		std::vector<GString> Columns;
		GString Name;
		bool Unique;
	public:
		// It's a bird!
		// It's a plane!
		// No, it's the default constructor.
		DataIndex();
		DataIndex(_CGR inName, const std::vector<GString>& inCol, const bool& inUnique);
		DataIndex(_CGR inName);
		DataIndex(const DataIndex& cIndx) {
			Columns = cIndx.Columns;
			Name = cIndx.Name;
			Unique = cIndx.Unique;
		}

		// Returns a reference to the columns vector. Can be modified directly.
		std::vector<GString>& get_Columns() { return Columns; }
		void set_Columns(const std::vector<GString>& in_newCol) { Columns = in_newCol; }

		GString get_Name() { return Name; }
		bool get_Unique() { return Unique; }
		void set_Unique(const bool& uniq) { Unique = uniq; }

		~DataIndex();
	};

}
