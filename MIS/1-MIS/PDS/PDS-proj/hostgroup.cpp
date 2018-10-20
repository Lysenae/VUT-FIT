// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "hostgroup.h"

///
/// \brief Konstruktor
/// \param h1
/// \param h2
///
HostGroup::HostGroup(StrVect h1, StrVect h2)
{
    m_id    = find_prop(h1, "group");
    m_mac1  = new MACAddr(find_prop(h1, "mac"));
    m_mac2  = new MACAddr(find_prop(h2, "mac"));
    m_ip4_1 = new IPv4Addr(find_prop(h1, "ipv4"));
    m_ip4_2 = new IPv4Addr(find_prop(h2, "ipv4"));
    StrVect h1_v6 = find_ipv6s(h1);
    StrVect h2_v6 = find_ipv6s(h2);

    for(std::string ip : h1_v6)
        m_ip6s_1.push_back(new IPv6Addr(ip));
    for(std::string ip : h2_v6)
        m_ip6s_2.push_back(new IPv6Addr(ip));
}

///
/// \brief Id skupiny
/// \return id
///
std::string HostGroup::id()
{
    return m_id;
}

///
/// \brief MAC adresa hosta 1
/// \return MACadresu hosta 1
///
MACAddr *HostGroup::mac1()
{
    return m_mac1;
}

///
/// \brief MAC adresa hosta 2
/// \return MACadresu hosta 2
///
MACAddr *HostGroup::mac2()
{
    return m_mac2;
}

///
/// \brief IPv4 adresa hosta 1
/// \return IPv4 hosta 1
///
IPv4Addr *HostGroup::ipv4_1()
{
    return m_ip4_1;
}

///
/// \brief IPv4 adresa hosta 2
/// \return IPv4 hosta 2
///
IPv4Addr *HostGroup::ipv4_2()
{
    return m_ip4_2;
}

///
/// \brief N-ta IPv6 adresa hosta 1
/// \return IPv6 hosta 1
///
IPv6Addr *HostGroup::ipv6_1(uint idx)
{
    if(idx < m_ip6s_1.size())
        return m_ip6s_1[idx];
    return nullptr;
}

///
/// \brief N-ta IPv6 adresa hosta 2
/// \return IPv6 hosta 2
///
IPv6Addr *HostGroup::ipv6_2(uint idx)
{
    if(idx < m_ip6s_2.size())
        return m_ip6s_2[idx];
    return nullptr;
}

///
/// \brief Pocet IPv6 adries hosta 1
/// \return pocet IPv6 adries hosta 1
///
uint HostGroup::len_ipv6s_2()
{
    return m_ip6s_2.size();
}

///
/// \brief Pocet IPv6 adries hosta 2
/// \return pocet IPv6 adries hosta 2
///
uint HostGroup::len_ipv6s_1()
{
    return m_ip6s_1.size();
}

///
/// \brief Vypise obsah
///
void HostGroup::print()
{
    std::cout << "Group: " << m_id << std::endl;
    std::cout << "  MAC 1: " << m_mac1->to_string() << std::endl;
    std::cout << "    IPv4: " << m_ip4_1->addr() << std::endl;
    for(IPv6Addr *a : m_ip6s_1)
        std::cout << "    IPv6: " << a->addr() << std::endl;
    std::cout << "  MAC 2: " << m_mac2->to_string() << std::endl;
    std::cout << "    IPv4: " << m_ip4_2->addr() << std::endl;
    for(IPv6Addr *a : m_ip6s_2)
        std::cout << "    IPv6: " << a->addr() << std::endl;
    std::cout << std::endl;
}

///
/// \brief Vyhlada IPv6 adresy v zozname
/// \param inv zoznam
/// \return zoznam IPv6 adries
///
StrVect HostGroup::find_ipv6s(StrVect inv)
{
    StrVect v6;
    for(std::string s : inv)
    {
        StrVect vt = split_str(s, '@');
        if(vt.size() == 2 && vt[0] == "ipv6")
            v6.push_back(vt[1]);
    }
    return v6;
}

///
/// \brief Najde polozku v zozname
/// \param inv zoznam
/// \param what polozka
/// \return najdenu polozku alebo prazdny retazec
///
std::string HostGroup::find_prop(StrVect inv, std::string what)
{
    std::string p = "";
    for(std::string s : inv)
    {
        StrVect vt = split_str(s, '@');
        if(vt.size() == 2 && vt[0] == what)
            p = vt[1];
    }
    return p;
}
