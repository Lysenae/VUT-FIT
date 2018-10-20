// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "ipaddr.h"

///
/// \brief Konstruktor
/// \param v IP verzia
/// \param ifa
///
IPAddr::IPAddr(IPVer v, ifaddrs *ifa)
{
    sockaddr_in  *in4 = nullptr;
    sockaddr_in6 *in6 = nullptr;

    m_itf_name = ifa->ifa_name;
    char *addr = new char[INET6_ADDRSTRLEN];
    char *mask = new char[INET6_ADDRSTRLEN];
    memset(addr, 0, INET6_ADDRSTRLEN);
    memset(mask, 0, INET6_ADDRSTRLEN);

    if(v == IPVer::IPv4)
    {
        in4 = (struct sockaddr_in*) ifa->ifa_addr;
        inet_ntop(AF_INET, &in4->sin_addr, addr, INET_ADDRSTRLEN);
        in4 = (struct sockaddr_in*) ifa->ifa_netmask;
        inet_ntop(AF_INET, &in4->sin_addr, mask, INET_ADDRSTRLEN);
    }
    else if(v == IPVer::IPv6)
    {
        in6 = (struct sockaddr_in6*) ifa->ifa_addr;
        inet_ntop(AF_INET6, &in6->sin6_addr, addr, INET6_ADDRSTRLEN);
        in6 = (struct sockaddr_in6*) ifa->ifa_netmask;
        inet_ntop(AF_INET6, &in6->sin6_addr, mask, INET6_ADDRSTRLEN);
    }

    m_addr = std::string(addr);
    m_mask = std::string(mask);
}

///
/// \brief Konstruktor
/// \param v
/// \param ip
/// \param mask
///
IPAddr::IPAddr(IPVer v, std::string ip, std::string mask)
{
    sockaddr_in  sa4;
    sockaddr_in6 sa6;
    int rslt;

    if(v == IPVer::IPv4)
        rslt = inet_pton(AF_INET, ip.c_str(), &(sa4.sin_addr));
    else if(v == IPVer::IPv6)
        rslt = inet_pton(AF_INET6, ip.c_str(), &(sa6.sin6_addr));
    if(rslt != 0)
        m_addr = ip;
    else
    {
        std::cerr << "IPAddr constructor: Invalid IP address" << std::endl;
        m_addr = "";
    }

    if(mask != "")
    {
        if(v == IPVer::IPv4)
            rslt = inet_pton(AF_INET, mask.c_str(), &(sa4.sin_addr));
        else if(v == IPVer::IPv6)
        {
            rslt = inet_pton(AF_INET6, mask.c_str(), &(sa6.sin6_addr));
        }
        if(rslt != 0)
            m_mask = mask;
        else
        {
            std::cerr << "IPAddr constructor: Invalid subnet mask" << std::endl;
            m_addr = "";
        }
    }
    else
    {
        m_mask = mask;
    }

    m_itf_name = "";
}

///
/// \brief Nazov sietoveho rozhrania
/// \return nazov sietoveho rozhrania
///
std::string IPAddr::interface()
{
    return m_itf_name;
}

///
/// \brief IP adresa
/// \return IP adresu
///
std::string IPAddr::addr()
{
    return m_addr;
}

///
/// \brief Maska podsiete
/// \return masku podsiete
///
std::string IPAddr::snmask()
{
    return m_mask;
}

///
/// \brief Zisti, ci je IP adresa zadana
/// \return false ak je IP adresa prazdna
///
bool IPAddr::empty()
{
    return m_addr == "";
}

///
/// \brief Zisti verziu IP adresy
/// \param addr
/// \return IPv4, IPv6 alebo Undef ak nerozpozna typ
///
IPVer IPAddr::get_version(std::string addr)
{
    if(split_str(addr, '.').size() == 4)
        return IPVer::IPv4;
    else if(split_str(addr, ':').size() >= 3)
        return IPVer::IPv6;

    return IPVer::Undef;
}
