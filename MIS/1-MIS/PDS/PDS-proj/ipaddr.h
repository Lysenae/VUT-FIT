// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef IPADDR_H
#define IPADDR_H

#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "types.h"

///
/// \enum IPVer
/// \brief Verzie IP protokolu
/// \var IPVer::IPv4 je IPv4
/// \var IPVer::IPv6 je IPv6
/// \var IPVer::Undef je neznamy typ
///
enum class IPVer
{
    IPv4,
    IPv6,
    Undef
};

///
/// \brief Abstraktna trieda pr IP adresy
///
class IPAddr
{
public:
    IPAddr(IPVer v, ifaddrs *ifa);
    IPAddr(IPVer v, std::string ip, std::string mask = "");
    virtual ~IPAddr() {}
    std::string interface();
    std::string addr();
    std::string snmask();
    virtual int mask_n() = 0;
    virtual std::string addr_grp(uint idx) = 0;
    virtual std::string mask_grp(uint idx) = 0;
    bool empty();

    static IPVer get_version(std::string addr);

protected:
    std::string m_itf_name;
    std::string m_addr;
    std::string m_mask;


    virtual std::string get_group(std::string ins, uint idx) = 0;
};

#endif // IPADDR_H
