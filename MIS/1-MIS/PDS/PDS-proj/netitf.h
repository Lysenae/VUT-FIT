// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef NETITF_H
#define NETITF_H

#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <iostream>

#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>

#include "ipv4addr.h"
#include "ipv6addr.h"
#include "socket.h"
#include "macaddr.h"

///
/// \brief Sietove rozhranie
///
class NetItf
{
public:
    NetItf(std::string interface);
    ~NetItf();
    std::string name();
    IPv4Addr *ipv4();
    IPv6Vect ipv6();
    MACAddr *mac();
    int index();

private:
    std::string m_interface;
    ifaddrs *m_ifaddrs;
    ifaddrs *m_ifa_next;
    ifreq *m_ifr;
    void *m_tmp_addr;
    Socket *m_sock;

    void init_ifr();
    void free_ifr();
    void free_ifaddr();
};

#endif // NETITF_H
