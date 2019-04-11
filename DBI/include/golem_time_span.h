#pragma once

class GDateTime;

   class GTimeSpan
   {
   private:
      INT64 m_data;    
   public: 
      static bool isTimeSpan(GString);

      static const GString esfDD;
      static const GString esfhh;
      static const GString esfmm;
      static const GString esfss;
      static const GString esft;
      static const GString esfhhmm;
      static const GString esfhhmmss;
      static const GString esfhhmmsst;
      static const GString esfDDhh;
      static const GString esfDDhhmm;
      static const GString esfDDhhmmss;
      static const GString esfDDhhmmsst;

      GTimeSpan(INT64 days, INT64 hours, INT64 minutes, INT64 seconds, INT64 tenths);
      GTimeSpan(const GTimeSpan &);
      GTimeSpan(INT64 tenths);
      GTimeSpan();

      REAL64 Days() const;
      REAL64 Hours() const;
      REAL64 Minutes() const;
      REAL64 Seconds() const;
      INT64  Tenths() const;
      REAL64 Total_Hours() const;
      REAL64 Total_Minutes() const;
      REAL64 Total_Seconds() const;
      INT64  Total_Tenths() const;

      INT64 Serialize() const;

      GTimeSpan operator + (const GTimeSpan &) const;    
      GDateTime operator + (const GDateTime &) const;
      GTimeSpan operator - (const GTimeSpan &) const;
      GTimeSpan operator * (int) const;
      GTimeSpan operator / (int) const;
      void operator += (const GTimeSpan &);    
      void operator -= (const GTimeSpan &);
      void operator *= (int);
      void operator /= (int);

      bool Parse(GString timespan);
      static GString FormatNumber(__int64 number, int count);
      GString ToString(const GString & format = esfDDhhmmsst) const;
      GString TokenToDate(wchar_t token, int & count) const;
   };
