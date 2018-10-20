// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "netitf.h"

///
/// \brief Konstruktor
/// \param interface nazov
///
NetItf::NetItf(std::string interface)
{
    m_interface = interface;
    m_ifaddrs   = nullptr;
    m_ifa_next  = nullptr;
    m_tmp_addr  = nullptr;
    m_ifr       = nullptr;
    m_sock      = new Socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
}

///
/// \brief Destruktor
///
NetItf::~NetItf()
{
    free_ifaddr();
    free_ifr();
    if(m_sock->status() == SocketStatus::Opened)
    {
        m_sock->close();
        delete m_sock;
        m_sock = nullptr;
    }
}

///
/// \brief Nazov
/// \return string
///
std::string NetItf::name()
{
    return m_interface;
}

///
/// \brief Lokalna IPv4 adresa
/// \return lokalnu IPv4 adresu
///
IPv4Addr *NetItf::ipv4()
{
    IPv4Addr *ip = nullptr;
    getifaddrs(&m_ifaddrs);

    for (m_ifa_next = m_ifaddrs; m_ifa_next != nullptr;
    m_ifa_next = m_ifa_next->ifa_next)
    {
        if(!m_ifa_next->ifa_addr)
            continue;

        if(m_ifa_next->ifa_addr->sa_family == AF_INET &&
        m_interface == std::string(m_ifa_next->ifa_name))
        {
            ip = new IPv4Addr(m_ifa_next);
        }
    }
    free_ifaddr();
    return ip;
}

///
/// \brief Lokalne IPv6 adresy
/// \return lokalne IPv6 adresy
///
IPv6Vect NetItf::ipv6()
{
    IPv6Vect v;
    getifaddrs(&m_ifaddrs);

    for (m_ifa_next = m_ifaddrs; m_ifa_next != nullptr;
    m_ifa_next = m_ifa_next->ifa_next)
    {
        if(!m_ifa_next->ifa_addr)
            continue;

        if(m_ifa_next->ifa_addr->sa_family == AF_INET6 &&
        m_interface == std::string(m_ifa_next->ifa_name))
            v.push_back(new IPv6Addr(m_ifa_next));
    }
    free_ifaddr();
    return v;
}

// Ciastocne prebrane z http://stackoverflow.com/a/16712726
///
/// \brief Lokalna MAC adresa
/// \return lokalnu MAC adresu
///
MACAddr *NetItf::mac()
{
    if(m_sock->open() != SocketStatus::Opened)
        return nullptr;
    init_ifr();
    if(ioctl(m_sock->fd(), SIOCGIFHWADDR, m_ifr) == OP_FAIL)
        return nullptr;
    MACAddr *m = new MACAddr(m_ifr);
    free_ifr();
    return m;
}

// Ciastocne prebrane z http://stackoverflow.com/a/16712726
///
/// \brief Index rozhrania
/// \return index
///
int NetItf::index()
{
    int idx = OP_FAIL;
    if(m_sock->open() != SocketStatus::Opened)
        return OP_FAIL;
    init_ifr();
    if(ioctl(m_sock->fd(), SIOCGIFINDEX, m_ifr) == OP_FAIL)
        return OP_FAIL;
    m_sock->close();
    idx = m_ifr->ifr_ifindex;
    free_ifr();
    return idx;
}

///
/// \brief Inicializuje ifreq
///
void NetItf::init_ifr()
{
    m_ifr = new ifreq;
    strncpy(m_ifr->ifr_name, m_interface.c_str(), IFNAMSIZ);
}

///
/// \brief Uvolni ifreq
///
void NetItf::free_ifr()
{
    if(m_ifr != nullptr)
    {
        delete m_ifr;
        m_ifr = nullptr;
    }
}

///
/// \brief Uvolni ifaddr
///
void NetItf::free_ifaddr()
{
    if(m_ifaddrs != nullptr)
    {
        delete m_ifaddrs;
        m_ifaddrs = nullptr;
    }
    if(m_ifa_next != nullptr)
    {
        delete m_ifa_next;
        m_ifa_next = nullptr;
    }
    m_tmp_addr = nullptr;
}
