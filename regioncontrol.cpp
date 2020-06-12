#include "regioncontrol.h"
#include "ext/ZFile.h"
bool RegionControl::OpenFile(const GString &name)
{
    ZFile zfo;
    if (!zfo.Open(name,EZFOpenMode::BinaryRead))
        return false;

    zfo >> Countries;
    zfo.Close();

    if (!Countries.size())
        return false;


    // Fill out regions
    for (Country& cn : Countries){
           Regions.reserve(Regions.size() + cn.Regions.size());
           for (Region& rgc : cn.Regions){
               Reg ar;
               ar.id = rgc.id;ar.name = rgc.name;
               ar.ownerid = cn.id;

               Regions.push_back(ar);
           }



    }



    return true;

}

bool RegionControl::SaveFile(const GString &outname)
{
    ZFile zfs;
    if (!zfs.Open(outname,EZFOpenMode::BinaryWrite))
        return false;


    zfs << Countries;

    zfs.Close();


    return true;
}

void RegionControl::SetCountries(std::vector<Country> &cts)
{
    Countries = cts;
}

GString RegionControl::GetOwnerName(Reg &ro)
{
    return GetCountryName(ro.id);
}

GString RegionControl::GetCountryName(int cid)
{
    for (Country& ctr : Countries){
        if (ctr.id == cid)
            return ctr.name;
    }

    return L"";

}

RegionControl::RegionControl()
{


}

ZFile &operator<<(ZFile &right, const Region &reg)
{
    right << reg.id;
    right << reg.name;

    return right;


}

ZFile &operator<<(ZFile &right, const Country &cnt)
{

    right << cnt.id;
    right << cnt.name;
    right << cnt.Regions;
    right << cnt.code;
    return right;


}

ZFile &operator>>(ZFile &right, Country &cr)
{
    right >> cr.id;
    right >> cr.name;
    right >> cr.Regions;
    right >> cr.code;

    return right;


}

ZFile &operator>>(ZFile &right, Region &reg)
{
    right >> reg.id;
    right >> reg.name;
    return right;

}
