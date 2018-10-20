// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef PACKET_H
#define PACKET_H

#include <linux/if_ether.h>
#include <linux/if_packet.h>

#include "types.h"
#include "macaddr.h"
#include "ipaddr.h"

///
/// \brief Packet
///
class Packet
{
public:
    static const uint  ETH_HDR_LEN = 14;
    static const uint  ETH_HW_TYPE = 1;

    Packet(MACAddr *src_mac);
    virtual ~Packet() {}
    void set_dst_hwa(MACAddr *dst_mac);
    void set_src_hwa(MACAddr *src_mac);
    virtual sockaddr_ll sock_addr(int if_idx) = 0;
    virtual uchar *serialize() = 0;
    virtual uint pktlen() = 0;

protected:
    static const uchar MAC_BCV4[];
    static const uchar MAC_BCV6[];

    MACAddr *m_src_hwa;                    // Zdrojova MAC
    MACAddr *m_dst_hwa;                    // Cielova MAC
    uchar    m_src_hwa_o[MACAddr::OCTETS]; // Oktety zdrojovej MAC
    uchar    m_dst_hwa_o[MACAddr::OCTETS]; // Oktety cielovej MAC
    uint16_t m_eth_prot;                   // Ethernetovy protokol

    ///
    /// \enum EthDest
    /// \brief Rozlisuje ci sa pouzije BC, MC alebo UC MAC adresa
    /// \var EthDest::UC je unicast
    /// \var EthDest::BC je broadcast nad IPv4
    /// \var EthDest::MC je multicast nad IPv6
    ///
    enum class EthDest
    {
        UC,
        BC,
        MC
    };

    uchar *eth_header(EthDest dest);
};

#endif // PACKET_H
