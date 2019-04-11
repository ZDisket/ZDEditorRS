/**************************************************************
*
* golem_string.h
*
* Description
* ===========
*  The GString class is a container that enables the use of 
*  strings as normal types, such as using comparison and 
*  concatenation operations, iterators, and STL algorithms 
*  and copying and assigning with class allocator managed memory.
*  The GString, in combination with the GSString, automates the 
*  wchar-to-char and inverse conversions.
*
* Owner
* =====
*  Nicolas Hatier
*
* Copyright  (C) 2003, Laboratoires Golemlabs Inc.
***************************************************************/

#pragma once

#ifndef _GOLEMLAB_STRING___H
#define _GOLEMLAB_STRING___H

#include <stdarg.h>
#include <math.h>
#include <string>
#include <vector>
#include <Windows.h>

/*

#ifndef INT8
#define INT8  char
#define WINT8 wchar_t
#define INT16 short
#define INT32 int
#ifdef WIN32
#define INT64 __int64
#endif

#define UINT8  unsigned char
#define UINT16 unsigned short
#define UINT32 unsigned int
#ifdef WIN32
#define UINT64 unsigned __int64
#endif

#define REAL32  float
#define REAL64  double
#endif

#define INT64 __int64
#define UINT32 unsigned int
#define UINT8  unsigned char
#define INT32 int

#define WINT8 wchar_t
#define REAL32  float
#define REAL64  double
*/
#define REAL64  double

#define WINT8 wchar_t

#define REAL32  float
#define INT8  char

class GString;
/*
* Casting class for GString. Represents a single-byte-character string
*/   
class GSString : public std::string
{
   static INT8 s_sCBuffer[]; 

public:
   GSString();
   GSString(const std::string &);
   GSString(const std::wstring &);
   GSString(const char *);
   explicit GSString(int);
   explicit GSString(__int64);
   explicit GSString(float);
   explicit GSString(double);
   explicit GSString(unsigned int);


   GSString operator + (const GSString&) const;      
   GSString operator + (const std::string&) const;
   GSString operator + (const char*) const;      
   void     operator +=(const GSString&);
   void     operator +=(const std::string&);
   void     operator +=(const char*);
   bool     operator ==(const GSString&) const;      
   bool     operator ==(const std::string&) const;      
   bool     operator ==(const INT8*) const;
   bool     operator !=(const GSString&) const;                
   bool     operator !=(const std::string&) const;
   bool     operator !=(const INT8*) const;

   operator const INT8 *() const;
   operator const signed INT8 *() const;
   operator const UINT8 *() const;
   operator GString() const;

   GSString ToUpper() const;
   GSString ToLower() const;
};

/*
* Main String class for Ginger
* Represents a Unicode (double-byte) character string, with many 
* support functions and operators
*/
class GString : public std::wstring
{ 
   static WINT8 s_sWBuffer[]; 
   static  INT8   s_sCBuffer[]; 

public: 
   GString();      

   GString(const std::wstring&);
   GString(const WINT8*);
   GString(const std::string&);
   GString(const INT8*);
   explicit GString(INT32);
   explicit GString(INT64);
   explicit GString(REAL32);
   explicit GString(REAL64);
   explicit GString(UINT32);

   INT32  ToINT32()  const;
   INT64  ToINT64()  const;
   REAL32  ToREAL32() const;
   REAL64 ToREAL64() const;
   bool ToBool() const;

   GString operator +(const GString&) const;      
   GString operator +(const std::string&) const;
   GString operator +(const INT8*) const;
   GString operator +(const WINT8*) const;
   GString operator +(const INT32) const;
   GString operator +(const REAL32) const;

   void operator +=(const GString&);
   void operator +=(const std::string&);
   void operator +=(const INT8*);      
   void operator +=(const WINT8*);
   void operator +=(const INT32);      
   void operator +=(const REAL32);      

   bool operator ==(const GString&) const;      
   bool operator ==(const std::string&) const;
   bool operator ==(const WINT8*) const;
   bool operator ==(const INT8*) const;

   bool operator !=(const GString&) const;      
   bool operator !=(const WINT8*) const;    
   bool operator !=(const std::string&) const;
   bool operator !=(const INT8*) const;

   operator GSString() const;

   GString ToUpper() const;
   GString ToLower() const;
   GString RTrim(WINT8 blankchar = ' ') const;
   GString LTrim(WINT8 blankchar = ' ') const;
   GString Trim(WINT8 blankchar = ' ') const;
   GString LCropTo(WINT8 blankchar = ' ') const;

   void BreakString(std::vector<GString> *outVec, INT8 inDelimiter);

   GString ReplaceNextPattern(const GString& in_ReplacingString, const GString& in_sPattern) const;

   // This method is used to format a String
   static GString FormatString(const GString& in_sPattern, const GString& in_sFirst, ...);

   static GString FormatNumber(REAL64 in_fNumber,
      GString in_sNumberDelimiter = L" ",
      GString in_sDecimalDelimiter = L".",
      GString in_sPrefix = L"",
      GString in_sSuffix = L"",
      INT8    in_iGrouping = 3,
      INT8    in_iPrecision = 2,
      bool    in_iAbbreviate = false);

   static GString FormatNumber(REAL64 in_fNumber, INT8 in_iPrecision);
};

GString operator + (const WINT8 *, const GString &);
GString operator + (const INT8 *, const GString &);
GSString operator + (const INT8 *, const GSString &);

#endif