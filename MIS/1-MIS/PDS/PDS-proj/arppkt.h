// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef ARPPKT_H
#define ARPPKT_H

#include <arpa/inet.h>

#include "packet.h"
#include "types.h"
#include "ipv4addr.h"
#include "macaddr.h"

///
/// \enum ArpType
/// \brief Type ARP sprav
/// \var ArpType::Request je ARP Request
/// \var ARPType::Response je ARP Response
///
enum class ArpType
{
    Request,
    Response
};

///
/// \brief Trieda ArpPkt
///
class ArpPkt : public Packet
{
public:
    static const uint BUFF_LEN   = 60;
    static const uint LEN        = 42;
    static const uchar ARP_REPLY = 0x02;
    static const uchar ARP_REQ   = 0x01;

    ArpPkt(ArpType t, IPv4Addr *ip, MACAddr *mac);
    void set_src_ip_addr(uint octet, uchar value);
    void set_src_ip_addr(uint octet, std::string value);
    void set_src_ip_addr(IPv4Addr *ipv4);
    void set_dst_ip_addr(uint octet, uchar value);
    void set_dst_ip_addr(uint octet, std::string value);
    void set_dst_ip_addr(IPv4Addr *ipv4);
    virtual sockaddr_ll sock_addr(int if_idx) override;
    virtual uchar *serialize() override;
    virtual uint pktlen() override;
    bool analyze_pkt(uchar *pkt, int len, MACAddr **mac, IPv4Addr **ip);

private:
    enum class ArpField
    {
        HW_TYPE,
        PROT_TYPE,
        HW_ADDLN,
        PROT_ADDLN,
        OPCODE,
        SRC_HWA,
        SRC_IPA,
        DST_HWA,
        DST_IPA
    };

    ArpType  m_type;
    uint16_t m_hw_t;
    uint16_t m_prot_t;
    uchar    m_hw_addl;
    uchar    m_prot_addl;
    uint16_t m_op;
    uchar    m_src_ip[4];
    uchar    m_dst_ip[4];

    static uint offs(ArpField f);
};

#endif // ARPPKT_H
