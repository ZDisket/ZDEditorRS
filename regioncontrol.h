#ifndef REGIONCONTROL_H
#define REGIONCONTROL_H
#include "StrTable/GStringTable.h"
#include "ext/ZFile.h"
struct Region{
    int id;
    GString name;
};
struct Reg{

    int ownerid;
    int id;
    GString name;

};
struct Country{
GString name;
GString code;
int id;
std::vector<Region> Regions;

};

class RegionControl
{

private:
    std::vector<Country> Countries;

    std::vector<Reg> Regions;
public:
    bool OpenFile(const GString& name);
    bool SaveFile(const GString& outname);

    inline std::vector<Country>& GetCountries(){return Countries;}
    inline std::vector<Reg>& GetRegions(){return Regions; }

    void SetCountries(std::vector<Country>& cts);
    GString GetOwnerName(Reg& ro);
    GString GetCountryName(int cid);
    RegionControl();
};

ZFile& operator<<(ZFile& right, const Region& reg);
ZFile& operator<<(ZFile& right, const Country& cnt);

ZFile& operator>>(ZFile& right,Region& reg);
ZFile& operator>>(ZFile& right,Country& cr);




#endif // REGIONCONTROL_H
