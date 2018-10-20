// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef IPV4ADDR_H
#define IPV4ADDR_H

#include "ipaddr.h"

///
/// \brief IPv4 adresa
///
class IPv4Addr : public IPAddr
{
public:
    static const uint OCTETS = 4;

    IPv4Addr(struct ifaddrs *ifa);
    IPv4Addr(std::string ip, std::string mask = "");
    static IPv4Addr *from_bytes(UchrVect bytes);
    ~IPv4Addr() {}
    virtual int mask_n() override;
    StrVect net_host_ips();
    virtual std::string addr_grp(uint idx) override;
    virtual std::string mask_grp(uint idx) override;
    uchar octet(uint idx);
    bool eq(IPv4Addr *other);

private:
    virtual std::string get_group(std::string ins, uint idx) override;
    int get_addr_n(uint idx);
    int uchb(uchar uc);
    int maxval(uint bits);
    std::string addr_part(uint until_octet);
    StrVect expand_ips(StrVect ips, int max, int base, int octet);
    StrVect remove_bc_net(StrVect ips);
};

#endif // IPV4ADDR_H
