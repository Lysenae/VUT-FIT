// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "macaddr.h"

///
/// \brief Konstruktor
/// \param ifr
///
MACAddr::MACAddr(ifreq *ifr)
{
    for (uint i=0; i < OCTETS; ++i)
        m_mac[i] = ifr->ifr_hwaddr.sa_data[i];
}

///
/// \brief Konstruktor
/// \param oct
///
MACAddr::MACAddr(UchrVect oct)
{
    if(oct.size() == OCTETS)
    {
        for(uint i=0; i<OCTETS; ++i)
            m_mac[i] = oct[i];
    }
    else
    {
        for(uint i=0; i<OCTETS; ++i)
            m_mac[i] = 0x00;
    }
}

///
/// \brief Konstruktor
/// \param mac
///
MACAddr::MACAddr(std::string mac)
{
    for(uint i=0; i<OCTETS; ++i)
        m_mac[i] = 0x00;

    if(mac.find_first_of(':') != std::string::npos)
    {
        StrVect octs = split_str(mac, ':');
        if(octs.size() == OCTETS)
        {
            for(uint i=0; i<OCTETS; ++i)
                m_mac[i] = literal_to_uchr(octs[i]);
        }
    }
    else if(mac.find_first_of('.') != std::string::npos)
    {
        StrVect quads = split_str(mac, '.');
        if(quads.size() == QUADS)
        {
            m_mac[0] = literal_to_uchr(quads[0].substr(0, 2));
            m_mac[1] = literal_to_uchr(quads[0].substr(2, 2));
            m_mac[2] = literal_to_uchr(quads[1].substr(0, 2));
            m_mac[3] = literal_to_uchr(quads[1].substr(2, 2));
            m_mac[4] = literal_to_uchr(quads[2].substr(0, 2));
            m_mac[5] = literal_to_uchr(quads[2].substr(2, 2));
        }
    }
}

///
/// \brief Citatelna podoba
/// \return string
///
std::string MACAddr::to_string() const
{
    char buffer[20];
    sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X",
        m_mac[0],m_mac[1],m_mac[2],m_mac[3],m_mac[4],m_mac[5]);
    return std::string(buffer);
}

///
/// \brief Oktet
/// \param idx
/// \return oktet na indexe
///
uchar MACAddr::octet(uint idx)
{
    if(idx < OCTETS)
        return m_mac[idx];
    return (uchar) 0;
}

///
/// \brief Porovna 2 MAC adresy
/// \param other
/// \return true ak su rovnake
///
bool MACAddr::eq(MACAddr *other)
{
    for(uint i=0; i<OCTETS; ++i)
    {
        if(m_mac[i] != other->octet(i))
            return false;
    }
    return true;
}

///
/// \brief Prazdna
/// \return true ak kazdy oktet je 0
///
bool MACAddr::empty() const
{
    for(uint i=0; i<OCTETS; ++i)
    {
        if(m_mac[i] != 0x00)
            return false;
    }
    return true;
}

///
/// \brief Prevod do formatu xxxx.xxxx.xxxx
/// \param mac
/// \return string
///
std::string MACAddr::to_xml(std::string mac)
{
    MACAddr *tmp = new MACAddr(mac);
    std::string xml_mac = "";
    xml_mac += str_bytes8(tmp->octet(0));
    xml_mac += str_bytes8(tmp->octet(1));
    xml_mac += ".";
    xml_mac += str_bytes8(tmp->octet(2));
    xml_mac += str_bytes8(tmp->octet(3));
    xml_mac += ".";
    xml_mac += str_bytes8(tmp->octet(4));
    xml_mac += str_bytes8(tmp->octet(5));
    delete tmp;
    return xml_mac;
}
