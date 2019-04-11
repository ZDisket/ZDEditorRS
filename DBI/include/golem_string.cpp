/**************************************************************
*
* golem_string.cpp
*
* Description
* ===========
* See corresponding .h file
*
* Owner
* =====
* Nicolas Hatier
*
* Copyright (C) 2003, Laboratoires Golemlabs Inc.
***************************************************************/

#include "../include/golem_string.h"

#pragma warning(disable : 4996)

WINT8 GString::s_sWBuffer[15]; 
INT8 GString::s_sCBuffer[15]; 
INT8 GSString::s_sCBuffer[15]; 

/*
* Default constructor. Builds an empty GSString
* in_Param : Param description
* Return value
*/
GSString::GSString(const std::wstring & content) : std::string(content.length(), std::string::value_type())
{
   UINT32 length = (UINT32)content.length();
   for(UINT32 i = 0; i < length; i++)
      at(i) = (INT8)(content[i] % 256); 
} 

GSString::GSString() : std::string() 
{
}

GSString::GSString(const std::string & content) : std::string(content) 
{
}

GSString::GSString(const INT8 * content) : std::string(content) 
{
}

GSString::GSString(INT32 content) : std::string(_itoa(content, s_sCBuffer, 10)) 
{
}

GSString::GSString(INT64 content) : std::string(_i64toa(content, s_sCBuffer, 10)) 
{
}

GSString::GSString(REAL32 content)
{
   sprintf_s(s_sCBuffer, "%7f", content);
   this->assign(GString(s_sCBuffer).Trim()); 
}

GSString::GSString(REAL64 content)
{
   sprintf_s(s_sCBuffer, "%15f", content);
   this->assign(GString(s_sCBuffer).Trim()); 
}

GSString::GSString(UINT32 content) : std::string(_ui64toa(content, s_sCBuffer, 10)) 
{
}

GSString GSString::operator + (const GSString& pOther) const 
{ 
   return (std::string)*this + (std::string)pOther; 
}

GSString GSString::operator + (const std::string& pOther) const 
{ 
   return *this + GSString(pOther); 
}

GSString GSString::operator + (const INT8* pOther) const 
{ 
   return *this + GSString(pOther); 
} 

void GSString::operator += (const std::string& pOther) 
{ 
   append(GSString(pOther)); 
}

void GSString::operator += (const GSString& pOther) 
{ 
   append(pOther); 
}

void GSString::operator += (const INT8* pOther) 
{ 
   append(GSString(pOther)); 
}

bool GSString::operator == (const std::string& pOther) const 
{ 
   return *this == pOther.c_str(); 
}

bool GSString::operator == (const GSString& pOther) const 
{ 
   return (std::string)*this == (std::string)pOther; 
}

bool GSString::operator == (const INT8* pOther) const 
{ 
   return (std::string)*this == pOther; 
}

bool GSString::operator != (const std::string& pOther) const 
{ 
   return !(*this == pOther.c_str()); 
}

bool GSString::operator != (const INT8* pOther) const 
{ 
   return !(*this == pOther); 
}

bool GSString::operator != (const GSString& pOther) const 
{ 
   return !(*this == pOther); 
}

/*
* Casting operator to get a single-byte character string buffer
* return a const char pointer to the internal buffer
*/
GSString::operator const INT8 * () const 
{ 
   return c_str(); 
}

GSString::operator const signed INT8 * () const 
{ 
   return (signed INT8 *)c_str(); 
}

GSString::operator const UINT8 * () const 
{ 
   return (UINT8 *)c_str(); 
}

GSString::operator GString () const 
{ 
   return GString(*this); 
}

/*
* Transform a GSString to uppercase
* return A new GSString with former uppercased content
*/
GSString GSString::ToUpper() const
{
   GSString ret(*this);
   for(GSString::iterator itr = ret.begin(); itr != ret.end(); itr++)
      *itr = toupper(*itr);

   return ret;
}

/*
* Transform a GSString to lowercase
* return A new GSString with former lowercased content
*/
GSString GSString::ToLower() const
{
   GSString ret(*this);
   for(GSString::iterator itr = ret.begin(); itr != ret.end(); itr++)
      *itr = tolower(*itr);

   return ret;
}

/*
* Constructor. Builds a GString based on existing text
* content Contents for the new GString
*/
GString::GString(const std::string& content) : std::wstring(content.length(), std::wstring::value_type())
{
   UINT32 length = (UINT32)content.length();
   for(UINT32 i = 0; i < length; i++)
      at(i) = (UINT8)content[i]; 
}

/*
* Constructor. Builds a GString based on existing text
* content Contents for the new GString
*/
GString::GString(const INT8* content) : std::wstring(strlen(content), std::wstring::value_type())
{
   UINT32 length = (UINT32)strlen(content);
   for(UINT32 i = 0; i < length; i++)
      at(i) = (UINT8)content[i]; 
}

// Constructors. Builds a GString based on existing text or numbers 
GString::GString() : std::wstring() 
{
}

GString::GString(const std::wstring& content) : std::wstring(content) 
{
}

GString::GString(const WINT8* content) : std::wstring(content) 
{
}

GString::GString(INT32 content) : std::wstring(_itow(content, s_sWBuffer, 10)) 
{
}

GString::GString(INT64 content) : std::wstring(_i64tow(content, s_sWBuffer, 10)) 
{
}

GString::GString(REAL32 content)
{
   swprintf(s_sWBuffer, 15, L"%7.2f", content);
   this->assign(s_sWBuffer); 
}

GString::GString(REAL64 content)
{
   swprintf(s_sWBuffer, 15, L"%15.2f", content);
   this->assign(s_sWBuffer); 
}

GString::GString(UINT32 content) : std::wstring(GString(_ui64tow_s(content, s_sWBuffer, 15, 10))) 
{
}

/*
* Addition operator 
* pOther Widestring to be appended 
* return new object containing appended string
*/
GString GString::operator + (const GString& pOther) const 
{ 
   return (std::wstring)*this + (std::wstring)pOther; 
}

GString GString::operator + (const WINT8* pOther) const 
{ 
   return (std::wstring)*this + std::wstring(pOther); 
}

GString GString::operator + (const std::string& pOther) const 
{ 
   return *this + GString(pOther); 
}

GString GString::operator + (const INT8* pOther) const 
{ 
   return *this + GString(pOther); 
} 

GString GString::operator +(const INT32 pOther) const
{
   swprintf(s_sWBuffer, 15, L"%d", pOther);
   return *this + GString(s_sWBuffer); 
}

GString GString::operator +(const REAL32 pOther) const
{
   swprintf(s_sWBuffer, 15, L"%f", pOther);
   return *this + GString(s_sWBuffer); 
}

/*
* Concatenation operator
*/
void GString::operator += (const std::string& pOther) 
{ 
   append(GString(pOther)); 
}

void GString::operator += (const GString& pOther) 
{ 
   append(pOther); 
}

void GString::operator += (const WINT8* pOther) 
{ 
   append(pOther); 
}

void GString::operator += (const INT8* pOther) 
{ 
   append(GString(pOther)); 
}

void GString::operator +=(const INT32 pOther)  
{
   swprintf(s_sWBuffer, 15, L"%i", pOther);
   append(GString(s_sWBuffer)); 
}

void GString::operator +=(const REAL32 pOther)
{
   swprintf(s_sWBuffer, 15, L"%f", pOther);
   append(GString(s_sWBuffer)); 
}

/*
* Direct Comparision operator 
* return true if converted strings are equal
*/
bool GString::operator == (const std::string& pOther) const 
{ 
   return *this == pOther.c_str(); 
}

bool GString::operator == (const GString& pOther) const 
{ 
   return (std::wstring)*this == (std::wstring)pOther; 
}

bool GString::operator == (const WINT8* pOther) const 
{ 
   return (std::wstring)*this == pOther; 
}

bool GString::operator == (const INT8* pOther) const 
{ 
   const WINT8 * cthis = this->c_str();
   INT32 len = (INT32)this->length();
   for(INT32 i = 0; i < len; i++)
      if((INT8)cthis[i] != (INT8)pOther[i]) 
         return false;

   return true;
}

/*
* Inversed Comparision operator 
* return true if converted strings are not equal
*/
bool GString::operator != (const std::string& pOther) const 
{ 
   return !(*this == pOther.c_str()); 
}

bool GString::operator != (const INT8* pOther) const 
{ 
   return !(*this == pOther); 
}

bool GString::operator != (const WINT8* pOther) const 
{ 
   return !(*this == pOther); 
}

bool GString::operator != (const GString& pOther) const 
{ 
   return !(*this == pOther); 
}

// Casting
GString::operator GSString () const 
{ 
   return GSString(*this); 
}

/*
* Transform a GString to uppercase
* return A new GString with former uppercased content
*/
GString GString::ToUpper() const
{
   GString ret(*this);
   for(GString::iterator itr = ret.begin(); itr != ret.end(); itr++)
      *itr = towupper(*itr);

   return ret;
}

/*
* Transform a GString to lowercase
* return A new GString with former lowercased content
*/
GString GString::ToLower() const
{
   GString ret(*this);
   for(GString::iterator itr = ret.begin(); itr != ret.end(); itr++)
      *itr = towlower(*itr);

   return ret;
}

GString GString::RTrim(WINT8 blankchar) const
{
   if(!length()) 
      return *this;

   INT32 i = (INT32)this->length() - 1;

   while((i > 0) && (this->at(i) != blankchar)) 
      i--; 

   return substr(0, i + 1);
}

GString GString::LCropTo(WINT8 blankchar) const
{
   if(!length()) 
      return *this;

   INT32 i = 0;
   while((i < (INT32)length()) && (this->at(i) != blankchar)) 
      i++;
   i++;

   if(i >= (INT32)length())
      return "";

   return substr(i, std::wstring::npos);
}

GString GString::LTrim(WINT8 blankchar) const
{
   if(!length()) 
      return *this;

   INT32 i = 0;
   while((i < (INT32)length()) && (this->at(i) == blankchar)) 
      i++;

   return substr(i, std::wstring::npos);
}

GString GString::Trim(WINT8 blankchar) const
{
   return RTrim(blankchar).LTrim(blankchar); 
}

/*
* Converts the string to numeric
**/
INT32 GString::ToINT32() const 
{ 
   return _wtoi(this->c_str()); 
}

INT64 GString::ToINT64() const 
{ 
   return _wtoi64(this->c_str()); 
}

REAL32 GString::ToREAL32() const 
{ 
   return (float)_wtof(this->c_str()); 
}

REAL64 GString::ToREAL64() const 
{ 
   return (double)_wtof(this->c_str()); 
}

bool GString::ToBool() const 
{ 
   return *this == GString("1") || this->ToLower() == GString("true");
}


/*
* Format the given REAL64 into a string using the specified parameters for
* Delimiters, prefix and suffix ...
* Number to format
* Delimiter used to separe group (333 333 333)
* Delimiter used as decimal (333,333)
* Prefix to prepend to number
* Suffix to append to number
* How many numbers a required to form a group
* How many decimal we keep
* Must abbreviate number (K = 1000, M = 1000000, B = 1000000000)
* return GString containung the formated number
*/
GString GString::FormatNumber(REAL64 in_fNumber,
                              GString in_sNumberDelimiter,
                              GString in_sDecimalDelimiter,
                              GString in_sPrefix,
                              GString in_sSuffix,
                              INT8 in_iGrouping,
                              INT8 in_iPrecision,
                              bool in_iAbbreviate)
{
   if(in_iAbbreviate)
   {
      // fabsf is here so we will abbreviate both + and - values
      if(fabsf((REAL32)in_fNumber) >= 10000000000.f)
      {
         in_fNumber /= 1000000000.f;
         in_sSuffix += GString(L" B");
      }

      if(fabsf((REAL32)in_fNumber) >= 10000000.f)
      {
         in_fNumber /= 1000000.f;
         in_sSuffix += GString(L" M");
      }

      if(fabsf((REAL32)in_fNumber) >= 10000.f)
      {
         in_fNumber /= 1000.f;
         in_sSuffix += GString(L" K");
      }
   }

   //Store number in GString
   GString l_sNumber(in_fNumber);

   l_sNumber = l_sNumber.LTrim();
   l_sNumber = l_sNumber.RTrim();


   //Find the decimal position
   INT32 l_iDecimalPosition = (INT32)l_sNumber.find('.');

   //If the decimal is at the complete end of the number we take off decimal
   if(l_iDecimalPosition == (INT32)(l_sNumber.length() - 1))
   {
      l_sNumber = l_sNumber.substr(0,l_sNumber.length() - 1);
      l_iDecimalPosition;
   }

   //Check if we have the precision needed in the number
   if(l_iDecimalPosition + (INT32)in_iPrecision < (INT32)l_sNumber.length())
   {
      if(in_iPrecision > 0)
         l_sNumber = l_sNumber.substr(0,l_iDecimalPosition) + in_sDecimalDelimiter + l_sNumber.substr(l_iDecimalPosition + 1,in_iPrecision);
      else
      {
         l_sNumber = l_sNumber.substr(0,l_iDecimalPosition + in_iPrecision);
         // -0 doesnt exist ...
         if(l_sNumber == "-0")
            l_sNumber = "0";
      }
   }

   INT8 l_iCurrentPosition = (INT8)l_iDecimalPosition;

   //While we can group number together we group them and insert the delimiter
   //If 0 we do not add any grouping character
   if(in_iGrouping != 0)
   {
      l_iCurrentPosition -= in_iGrouping;
      while(l_iCurrentPosition > 0)
      {
         l_sNumber.insert(l_iCurrentPosition,in_sNumberDelimiter);
         l_iCurrentPosition -= in_iGrouping;
      }
   }

   //Finally append and prepend suffix and prefix
   l_sNumber.insert(0,in_sPrefix);
   l_sNumber.append(in_sSuffix);

   return l_sNumber;
}

GString GString::FormatNumber(REAL64 in_fNumber, INT8 in_iPrecision)
{
   return FormatNumber(in_fNumber,L"",L".",L"",L"",0,in_iPrecision);
}

/*
* Method that formats a string, given a variable amount of arguments, and the arguments
* are not necessarly placed in the same order as they appear in the string.
* The Pattern string must be formatted using %s# where # is the param ID.
* WARNING: Make sure that the passed objects are GString() and not L"" or "" or
* the method will not work.
*
* Example of utilisation:
* GString l_sResult = GString::FormatString(GString("The %s4 is %s2, but the %s3 is %s1."),GString("blue"),GString("red"),GString("Apple"),GString("Car"));
* The result would be : "The Car is red, but the Apple is blue."
* 
* in_sPattern: Pattern GString
* in_sFirst: First Argument
* ...: Variable list of arguments
*/
GString GString::FormatString(const GString& in_sPattern, const GString& in_sFirst, ...)
{
   GString l_sResult= in_sPattern; 
   GString l_sIt = in_sFirst;
   va_list l_Marker;

   UINT32 l_iCounter = 1;
   GString l_sSearchedString = L"%s";
   va_start( l_Marker, in_sFirst ); 
   //As long as there are %s string to replace
   for(;;)
   {
      //Check if there is a String to replace
      INT32 l_iPosition = (INT32)l_sResult.find(GString(l_sSearchedString+GString(l_iCounter)));
      if(l_iPosition < 0)
      {//String not found, 
         break;
      }
      else
      {
         l_sResult = l_sResult.substr(0,l_iPosition) + l_sIt + l_sResult.substr(l_iPosition+3);
         l_sIt = va_arg(l_Marker,GString);
      }

      l_iCounter++;
   }

   va_end(l_Marker);
   return l_sResult;
}

GString GString::ReplaceNextPattern(const GString& in_ReplacingString, const GString& in_sPattern) const
{
   UINT32 l_iStartPosition = (UINT32)find(in_sPattern);
   if(l_iStartPosition != GString::npos)
   {
      GString l_Results;
      l_Results.reserve(size() + in_ReplacingString.size() - in_sPattern.size() );
      l_Results.insert(0, *this, 0, l_iStartPosition);
      l_Results.insert(l_iStartPosition, in_ReplacingString);
      l_Results.insert(l_Results.size(), *this, l_iStartPosition+in_sPattern.size(), size() - in_sPattern.size() - l_iStartPosition);
      return l_Results;
   }

   return *this;
}

/*
* Global addition operator 
* First string
* String to be appended with the first string
* return new object containing appended string
*/
GString operator + (const WINT8 * p1, const GString & p2)
{
   return GString(p1).append(p2);
}

/*
* Global addition operator single-byte characters conversion
* First string
* String to be appended with the first string
* return new object containing appended string
*/
GString operator + (const INT8 * p1, const GString & p2)
{
   return GString(p1).append(p2);
}

GSString operator + (const INT8 * p1, const GSString & p2)
{
   return GSString(p1).append(p2);
}

void GString::BreakString(std::vector<GString> *outVec, INT8 inDelimiter)
{
	GString lStr;
   std::wstring lChar;
	bool lAllowed = true;

	for(INT32 c = 0; c < (INT32)this->length(); c++)
	{
		if(this->at(c) != inDelimiter)
		{
			if(lAllowed)
			{
				lStr = "";
			}

			lAllowed = false;
		}
		else
		{
			if(!lAllowed)
			{
				if(lStr != "")
					(*outVec).push_back(lStr);

				lStr = "";
				lAllowed = true;
			}
		}

      lChar = this->at(c);
		lStr += lChar;
	}

	(*outVec).push_back(lStr);
}