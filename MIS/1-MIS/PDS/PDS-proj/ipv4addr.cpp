// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "ipv4addr.h"

////
/// \brief Konstruktor
/// \param ifa
///
IPv4Addr::IPv4Addr(ifaddrs *ifa) : IPAddr(IPVer::IPv4, ifa) {}

///
/// \brief Konstruktor
/// \param ip
/// \param mask
///
IPv4Addr::IPv4Addr(std::string ip, std::string mask) :
IPAddr(IPVer::IPv4, ip, mask)
{
}

///
/// \brief Zostavi IPv4 adresu z bytov
/// \param bytes
/// \return IPv4 adresu
///
IPv4Addr *IPv4Addr::from_bytes(UchrVect bytes)
{
    std::string a = "";
    if(bytes.size() == OCTETS)
    {
        for(uint i=0; i<OCTETS; ++i)
        {
            a += std::to_string((int)bytes[i]);
            if(i<OCTETS-1)
                a += ".";
        }
    }
    return new IPv4Addr(a);
}

///
/// \brief Maska podsiete ako dekadicke cislo
/// \return masku podsiete
///
int IPv4Addr::mask_n()
{
    int mask = 0;
    for(uint i=0; i<OCTETS; ++i)
    {
        size_t ptr;
        std::string grp = mask_grp(i);
        uchar octet = (uchar)std::stoi(grp, &ptr);
        if(ptr == grp.size())
            mask += uchb(octet);
        else
        {
            mask = -1;
            break;
        }
    }
    return mask;
}

///
/// \brief Zostavi zoznam vsetkych IPv4 adries podsiete s vynimkou routeru a BC
/// \return zoznam IPv4 adries podsiete
///
StrVect IPv4Addr::net_host_ips()
{
    StrVect v;
    int mask = mask_n();
    if(mask > 0)
    {
        int  octet = OCTETS - (mask / BITS +1);
        uint bits  = BITS - mask;
        bool whole = mask % BITS == 0;
        int  ov    = 0;
        int  max   = 0;

        v.push_back(addr_part(octet));
        max = maxval(whole ? BITS : bits % BITS);
        if(max < 0)
        {
            std::cerr << "Negative maxval result in net_host_ips\n";
            v.clear();
            return v;
        }

        if(!whole)
        {
            ov = get_addr_n(octet);
            if(ov < 0)
            {
                std::cerr << "Negative get_addr_n result in net_host_ips\n";
                v.clear();
                return v;
            }
        }

        v = expand_ips(v, max, ov, octet);
        v = remove_bc_net(v);
    }
    return v;
}

///
/// \brief IPv4Addr::addr_grp
/// \param idx
/// \return skupinu (oktet) z IPv4 adresy
///
std::string IPv4Addr::addr_grp(uint idx)
{
    return get_group(m_addr, idx);
}

///
/// \brief IPv4Addr::mask_grp
/// \param idx
/// \return skupinu (oktet) z masky adresy
///
std::string IPv4Addr::mask_grp(uint idx)
{
    return get_group(m_mask, idx);
}

///
/// \brief IPv4Addr::octet
/// \param idx
/// \return oktet adresy
///
uchar IPv4Addr::octet(uint idx)
{
    return str_to_uch(addr_grp(idx));
}

///
/// \brief Porvona IPv4 adresy
/// \param other
/// \return true ak su zhodne
///
bool IPv4Addr::eq(IPv4Addr *other)
{
    for(uint i=0; i<OCTETS; ++i)
    {
        if(octet(i) != other->octet(i))
            return false;
    }
    return true;
}

///
/// \brief IPv4Addr::get_group
/// \param ins
/// \param idx
/// \return vrati skupinu (oktet) z IP adresy alebo masky
///
std::string IPv4Addr::get_group(std::string ins, uint idx)
{
    std::string s = "";
    if(idx < OCTETS)
    {
        StrVect grps = split_str(ins, '.');
        s = grps[OCTETS - idx - 1];
    }
    return s;
}

///
/// \brief IPv4Addr::get_addr_n
/// \param idx
/// \return ciselu hodnotu oktetu
///
int IPv4Addr::get_addr_n(uint idx)
{
    size_t ptr;
    std::string grp = addr_grp(idx);
    int octet = std::stoi(grp, &ptr);
    return ptr == grp.size() ? octet : -1;
}

///
/// \brief Oktet masky podsiete
/// \param uc
/// \return pocet 1 bitov oktetu
///
int IPv4Addr::uchb(uchar uc)
{
    switch(uc)
    {
        case 255: return 8;
        case 254: return 7;
        case 252: return 6;
        case 248: return 5;
        case 240: return 4;
        case 224: return 3;
        case 192: return 2;
        case 128: return 1;
        case 0:   return 0;
        default:  return -1;
    }
}

///
/// \brief IPv4Addr::maxval
/// \param bits
/// \return
///
int IPv4Addr::maxval(uint bits)
{
    switch(bits)
    {
        case 0:  return 0;
        case 1:  return 1;
        case 2:  return 3;
        case 3:  return 7;
        case 4:  return 15;
        case 5:  return 31;
        case 6:  return 63;
        case 7:  return 127;
        case 8:  return 255;
        default: return -1;
    }
}

///
/// \brief IPv4Addr::addr_part
/// \param until_octet
/// \return cast IP adresy po zadany oktet
///
std::string IPv4Addr::addr_part(uint until_octet)
{
    std::string addr = "";
    for(uint i=OCTETS-1; i>until_octet; --i)
    {
        addr += addr_grp(i);
        addr += ".";
    }
    return addr;
}

///
/// \brief IPv4Addr::expand_ips
/// \param ips
/// \param max
/// \param base
/// \param octet
/// \return rozsiri zoznam IPv4 adries o dalsi oktet
///
StrVect IPv4Addr::expand_ips(StrVect ips, int max, int base, int octet)
{
    if(octet < 0)
        return ips;

    std::string ipaddr;

    StrVect v;
    for(std::string ip : ips)
    {
        for(int i=0; i<=max; ++i)
        {
            ipaddr = std::string(ip + std::to_string(base + i) +
                ((octet == 0) ? "" : "."));
            if(ipaddr != m_addr)
                v.push_back(ipaddr);
        }
    }
    return expand_ips(v, B_UCHAR_MAX, 0, octet-1);
}

///
/// \brief IPv4Addr::remove_bc_net
/// \param ips
/// \return odstrani zo zoznamu adresu routera, siete a BC
///
StrVect IPv4Addr::remove_bc_net(StrVect ips)
{
    std::string net = ips[0];
    std::string rt  = ips[1];
    std::string bc  = ips[ips.size()-1];
    StrVect v;
    for(std::string ip : ips)
    {
        if(ip != net && ip != rt && ip != bc)
        {
            v.push_back(ip);
        }
    }
    return v;
}
