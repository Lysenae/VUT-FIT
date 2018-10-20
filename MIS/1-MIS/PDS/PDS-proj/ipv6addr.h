// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef IPV6ADDR_H
#define IPV6ADDR_H

#include <iostream>

#include "ipaddr.h"
#include "types.h"

///
/// \brief IPv6 adresa
///
class IPv6Addr : public IPAddr
{
public:
    static const uint BYTES    = 16;
    static const uint BLOCKS   = 8;
    static const uint GRP_S    = 4;
    static const uint HDRS_LEN = 54;

    IPv6Addr(ifaddrs *ifa);
    IPv6Addr(std::string ip, std::string mask = "");
    static IPv6Addr *from_bytes(UchrVect bytes);
    ~IPv6Addr() {}
    virtual std::string addr_grp(uint idx) override;
    virtual std::string mask_grp(uint idx) override;
    virtual int mask_n() override;
    UchrVect to_uchar();
    bool is_ll();
    bool is_global();
    bool eq(IPv6Addr *other);

private:
    virtual std::string get_group(std::string ins, uint idx) override;
};

///
/// \brief IPv6Vect je alias pre std::vector<IPv6Addr *>
///
typedef std::vector<IPv6Addr *> IPv6Vect;

#endif // IPV6ADDR_H
