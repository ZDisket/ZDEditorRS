#pragma once

#define MAXTENTH 10
#define MAXSEC   60
#define MAXMIN   60
#define MAXHOUR  24
#define MAXDAY   31
#define MAXMONTH 12

   const int TENTHSINSEC   = MAXTENTH;
   const int TENTHSINMIN   = MAXSEC   * TENTHSINSEC;
   const int TENTHSINHOUR  = MAXMIN   * TENTHSINMIN;
   const int TENTHSINDAY   = MAXHOUR  * TENTHSINHOUR;
   const int TENTHSINMONTH = MAXDAY   * TENTHSINDAY;   // not exact, only a maximum for serialization
   const int TENTHSINYEAR  = MAXMONTH * TENTHSINMONTH; // not exact, only a maximum for serialization
#ifndef UINT64
#define UINT64 unsigned long long
#endif

   class GTimeSpan;
   class GDateTime
   {
      static const INT8 MONTHMAX[];

      struct GMyTime 
      {
         INT64 year;
         INT64 month;
         INT64 day;
         INT64 hour;
         INT64 minute;
         INT64 second;
         INT64 tenth;
      } m_data;    

      void         DateSerial(INT64 year, INT64 month, INT64 day, INT64 hour, INT64 min, INT64 sec, INT64 tenth);
      static INT64 GetMonthMax(INT64 year, UINT64 month);
      static void  FitTime(INT64 & big, INT64 & little, INT64 little_max);
      static void  FitDay(INT64 & year, INT64 & month, INT64 & day);
      static bool  IsBissextile(INT64 year);
      void         FitTime();

   public:
      static const GString m_Months[];
      static const GString m_WeekDays[];

      static const GString etfhhmmss;
      static const GString etfhhmmsst;
      static const GString etfYYMMDD;
      static const GString etfYYYYMMDD;
      static const GString etfDDMMYY;
      static const GString etfDDMMYYYY;
      static const GString etfMMDDYY;
      static const GString etfMMDDYYYY;
      static const GString etfYYMMDDhhmmss;
      static const GString etfYYYYMMDDhhmmss;
      static const GString etfDDMMYYhhmmss;
      static const GString etfDDMMYYYYhhmmss;
      static const GString etfMMDDYYhhmmss;
      static const GString etfMMDDYYYYhhmmss;
      static const GString etfMMMMDDYYYYhhmm;
      static const GString etfYYMMDDhhmmsst;
      static const GString etfYYYYMMDDhhmmsst;
      static const GString etfDDMMYYhhmmsst;
      static const GString etfDDMMYYYYhhmmsst;
      static const GString etfMMDDYYhhmmsst;      

      static GDateTime Now();
      static bool isDateTime(GString);
      GDateTime();
      GDateTime(const GDateTime &);
      GDateTime(const tm &, short tenths = 0);   
      GDateTime(INT64 year, INT64 month, INT64 day, INT64 hours, INT64 minutes, INT64 seconds, INT64 tenths);
      GDateTime(INT64 serialized);

      virtual ~GDateTime(void);    

      bool Parse(GString time);   
      static GString FormatNumber(__int64 number, int count);
      GString ToString(const GString & format = etfYYYYMMDDhhmmsst) const;
      GString TokenToDate(wchar_t token, int & count) const;

      GDateTime & operator = (const GDateTime &);

      GDateTime operator + (const GTimeSpan &) const;
      GDateTime operator - (const GTimeSpan &) const;
      GTimeSpan operator - (const GDateTime &) const;
      void operator += (const GTimeSpan &);
      void operator -= (const GTimeSpan &);

      bool      operator <  (const GDateTime &) const;
      bool      operator >  (const GDateTime &) const;
      bool      operator <= (const GDateTime &) const;
      bool      operator >= (const GDateTime &) const;
      bool      operator == (const GDateTime &) const;
      bool      operator != (const GDateTime &) const;

      INT64 Year() const;
      UINT64 Month() const;
      UINT64 Day() const;
      UINT64 Hours() const;
      UINT64 Minutes() const;
      UINT64 Seconds() const;
      UINT64 Tenths() const;

      void Year(INT64 value);
      void Month(INT64 value);
      void Day(INT64 value);
      void Hours(INT64 value);
      void Minutes(INT64 value);
      void Seconds(INT64 value);
      void Tenths(INT64 value);        

      INT64 Serialize() const;
   };


