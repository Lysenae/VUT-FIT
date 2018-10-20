// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "ipv6addr.h"

///
/// \brief Konstruktor
/// \param ifa
///
IPv6Addr::IPv6Addr(ifaddrs *ifa) : IPAddr(IPVer::IPv6, ifa) {}

///
/// \brief Konstruktor
/// \param ip
/// \param mask
///
IPv6Addr::IPv6Addr(std::string ip, std::string mask) :
IPAddr(IPVer::IPv6, ip, mask)
{
    if(mask != "" && mask_n() == OP_FAIL)
        std::cerr << "IPv6Addr Constructor: Invalid subnet mask format\n";
}

///
/// \brief IPv6 adresa z prubu bytov
/// \param bytes
/// \return IPv6 adresu
///
IPv6Addr *IPv6Addr::from_bytes(UchrVect bytes)
{
    std::string a = "";
    if(bytes.size() == BYTES)
    {
        for(uint i=0; i<BYTES; ++i)
        {
            a += str_bytes8(bytes[i]);
            if(i < BYTES-1 && i % 2 == 1)
                a += ":";
        }
    }
    return new IPv6Addr(a);
}

///
/// \see IPv4Addr
///
std::string IPv6Addr::addr_grp(uint idx)
{
    return get_group(m_addr, idx);
}

///
/// \see IPv4Addr
///
std::string IPv6Addr::mask_grp(uint idx)
{
    return get_group(m_mask, idx);
}

///
/// \see IPv4Addr
///
int IPv6Addr::mask_n()
{
    int m = 0;
    for(char c : m_mask)
    {
        switch(c)
        {
            case '0': m += 0; break;
            case '8': m += 1; break;
            case 'C':
            case 'c': m += 2; break;
            case 'E':
            case 'e': m += 3; break;
            case 'F':
            case 'f': m += 4; break;
            case ':': break;
            default: return OP_FAIL;
        }
    }
    return m;
}

///
/// \brief Skonvertuje dvojicu znakov adresy na uchar
/// \return uchar
///
UchrVect IPv6Addr::to_uchar()
{
    UchrVect uc;
    std::string s = "";
    StrVect grps  = split_str(m_addr, ':');
    uint nonempty = 0;
    int empty     = -1;
    uint diff     = 0;
    StrVect newgrps;

    for(uint i=0; i<grps.size(); ++i)
    {
        if( grps[i] != "")
            ++nonempty;
        else
            empty = i;
    }
    for(uint i=0; i<grps.size(); ++i)
    {
        if((int)i == empty)
            for(uint j=0; j<(BLOCKS-nonempty); ++j)
                newgrps.push_back("0000");
        else if(grps[i] != "")
        {
            if(grps[i].size() < GRP_S)
            {
                diff = GRP_S - grps[i].size();
                s    = "";
                for(uint i=0; i<diff; ++i)
                    s += "0";
                s += grps[i];
            }
            else
                s = grps[i];
            newgrps.push_back(s);
        }
    }

    for(std::string s : newgrps)
    {
        uc.push_back(literal_to_uchr(s.substr(0, 2)));
        uc.push_back(literal_to_uchr(s.substr(2, 2)));
    }
    return uc;
}

///
/// \brief Je link local?
/// \return bool
///
bool IPv6Addr::is_ll()
{
    std::string fg = addr_grp(IPv6Addr::BLOCKS-1);
    if(fg == "FE80" || fg == "fe80" || fg == "Fe80" || fg == "fE80")
        return true;
    return false;
}

///
/// \brief Je global?
/// \return bool
///
bool IPv6Addr::is_global()
{
    std::string fg = addr_grp(IPv6Addr::BLOCKS-1);
    if(fg[0] == '2' || fg[0] == '3')
        return true;
    return false;
}

///
/// \brief Porovna 2 IPv6 adresy
/// \param other
/// \return true ak su rovnake
///
bool IPv6Addr::eq(IPv6Addr *other)
{
    UchrVect uc1 = to_uchar();
    UchrVect uc2 = other->to_uchar();
    if(uc1.size() != uc2.size())
        return false;
    for(uint i=0; i<uc1.size(); ++i)
    {
        if(uc1[i] != uc2[i])
            return false;
    }
    return true;
}

///
/// \see IPv4Addr
///
std::string IPv6Addr::get_group(std::string ins, uint idx)
{
    std::string s = "";
    if(idx <= BLOCKS)
    {
        StrVect grps = split_str(ins, ':');
        uint nonempty = 0;
        int empty     = -1;
        for(uint i=0; i<grps.size(); ++i)
        {
            if(grps[i] != "")
                ++nonempty;
            else
                empty = i;
        }
        StrVect newgrps;
        for(uint i=0; i<grps.size(); ++i)
        {
            if((int)i == empty)
                for(uint j=0; j<(BLOCKS-nonempty); ++j)
                    newgrps.push_back("0");
            else if(grps[i] != "")
                newgrps.push_back(grps[i]);
        }
        s = newgrps[BLOCKS - idx - 1];
    }
    return s;
}
