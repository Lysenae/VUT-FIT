// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "icmpv6pkt.h"

///
/// \brief Kontruktor
/// \param type typ ICMPv6 paketu
/// \param ip IPv6 adresa zdroja
/// \param mac MAC adresa zdroja
///
IcmpV6Pkt::IcmpV6Pkt(IcmpV6Type type, IPv6Addr *ip, MACAddr *mac) :
Packet(mac)
{
    m_type       = type;
    m_src_ip     = ip;
    m_dst_ip     = nullptr;
    m_eth_prot   = htons(ETH_P_IPV6);
    m_echo_id[0] = 0x00;
    m_echo_id[1] = 0x00;
    m_multicast  = true;
    srand(time(nullptr));

    // NA specific
    m_na_flag_override  = false;
    m_na_flag_solicited = false;
    m_na_flag_router    = false;
}

///
/// \brief Nastavi IPv6 adresu ciela
/// \param ipv6 IPv6 adresa
///
void IcmpV6Pkt::set_dst_ip_addr(IPv6Addr *ipv6)
{
    m_dst_ip = ipv6;
}

///
/// \brief Dlzka paketu vratane Ethernet hlavicky
/// \return dlzku paketu
///
uint IcmpV6Pkt::pktlen()
{
    return ETH_HDR_LEN + IPV6_HDR_LEN + payload_length();
}

///
/// \brief Velkost ICMPv6 hlavicky
/// \return velkoct ICMPv6 hlavicky
///
uint IcmpV6Pkt::payload_length() const
{
    switch(m_type)
    {
        case IcmpV6Type::NS:   return NS_LEN;
        case IcmpV6Type::NA:   return NA_LEN;
        case IcmpV6Type::Ping: return PING_LEN;
        default:               return 0;
    }
}

///
/// \brief Vytvori prud bytov
/// \return pole uchar
///
uchar *IcmpV6Pkt::serialize()
{
    uchar *buff    = new uchar[pktlen()];
    uchar *ehdr    = eth_header(m_multicast ? EthDest::MC : EthDest::UC);
    uchar *ihdr    = ipv6_hdr();
    uchar *icmphdr = icmp_body();
    memset(buff, 0, pktlen());
    for(uint i=0; i<ETH_HDR_LEN; ++i)
        buff[i] = ehdr[i];
    for(uint i=0; i<IPV6_HDR_LEN; ++i)
        buff[i+ETH_HDR_LEN] = ihdr[i];
    for(uint i=0; i<payload_length(); ++i)
        buff[i+ETH_HDR_LEN+IPV6_HDR_LEN] = icmphdr[i];
    return buff;
}

///
/// \brief Vrati sockaddr_ll pre socket
/// \param if_idx cislo sietoveho rozhrania
/// \return sockaddr_ll
///
sockaddr_ll IcmpV6Pkt::sock_addr(int if_idx)
{
    sockaddr_ll sock_addr;
    sock_addr.sll_family   = AF_PACKET;
    sock_addr.sll_protocol = htons(ETH_P_IPV6);
    sock_addr.sll_ifindex  = if_idx;
    sock_addr.sll_hatype   = htons(ETH_HW_TYPE);
    sock_addr.sll_pkttype  = (PACKET_BROADCAST);
    sock_addr.sll_halen    = MACAddr::OCTETS;
    sock_addr.sll_addr[6]  = 0x00;
    sock_addr.sll_addr[7]  = 0x00;
    for(uint i=0; i<MACAddr::OCTETS; ++i)
        sock_addr.sll_addr[i] = m_src_hwa_o[i];
    return sock_addr;
}

///
/// \brief Nastavi S flag v NA
/// \param flag
///
void IcmpV6Pkt::set_na_flag_solicited(bool flag)
{
    m_na_flag_solicited = flag;
}

///
/// \brief Nastavi O flag v NA
/// \param flag
///
void IcmpV6Pkt::set_na_flag_override(bool flag)
{
    m_na_flag_override = flag;
}

///
/// \brief Nastavi R flag v NA
/// \param flag
///
void IcmpV6Pkt::set_na_flag_router(bool flag)
{
    m_na_flag_router = flag;
}

///
/// \brief Nastavi priznak, ci sa bude posielat multicast alebo unicast
/// \param flag
///
void IcmpV6Pkt::set_multicast_flag(bool flag)
{
    m_multicast = flag;
}

///
/// \brief Vytvori IPv6 hlavicku
/// \return IPv6 hlavicku
///
uchar *IcmpV6Pkt::ipv6_hdr()
{
    uchar *hdr        = new uchar[IPV6_HDR_LEN];
    uint16_t pl       = htons(payload_length());
    UchrVect src_ip_u = m_src_ip->to_uchar();
    UchrVect dst_ip_u = m_dst_ip->to_uchar();

    memset(hdr, 0, IPV6_HDR_LEN);
    hdr[0] = 0x60;                // Version
    // Traffic Class a Flow label [1-3] v memset
    memcpy(hdr+4, &pl, S_USHORT); // Payload length
    hdr[6] = TYPE_NR;         // Next header
    hdr[7] = 0xFF;                // Hop limit, pre ND povinne 255
    for(uint i=0; i<IPv6Addr::BYTES; ++i)
    {
        hdr[8 + i]                   = src_ip_u[i];
        hdr[8 + i + IPv6Addr::BYTES] = dst_ip_u[i];
    }
    return hdr;
}

///
/// \brief Vytbori ICMPv6 hlavicku
/// \return ICMPv6 hlavicku
///
uchar *IcmpV6Pkt::icmp_body()
{
    switch(m_type)
    {
        case IcmpV6Type::NS: return serialize_ns();
        case IcmpV6Type::NA: return serialize_na();
        case IcmpV6Type::Ping: return serialize_echo();
        default: return nullptr;
    }
}

///
/// \brief Vytvori NS hlavicku
/// \return NS hlavicku
///
uchar *IcmpV6Pkt::serialize_ns()
{
    uchar *hdr        = new uchar[payload_length()];
    UchrVect dst_ip_u = m_dst_ip->to_uchar();
    memset(hdr, 0, payload_length());
    hdr[0] = NS_TYPE;              // Type 135 (0x87)
    // Code, Checksum, Reserved nastavene v memset, byty 1-7
    for(uint i=0; i<dst_ip_u.size(); ++i) // Target IP addr
        hdr[8+i] = dst_ip_u[i];
    hdr[24] = 0x01;                       // Opts type
    hdr[25] = 0x01;                       // Opts length
    for(uint i=0; i<MACAddr::OCTETS; ++i) // Source MAC
        hdr[26+i] = m_src_hwa_o[i];
    uint16_t chks = checksum(hdr);
    memcpy(hdr+2, &chks, S_USHORT);
    return hdr;
}

///
/// \brief Vytvori NA hlavicku
/// \return NA hlavicku
///
uchar *IcmpV6Pkt::serialize_na()
{
    uchar *hdr        = new uchar[payload_length()];
    UchrVect src_ip_u = m_src_ip->to_uchar();
    memset(hdr, 0, payload_length());
    hdr[0] = NA_TYPE;              // Type 136 (0x88)
    // Code, Checksum nastavene v memset, byty 1-3
    hdr[4] = na_flags();                  // Flags
    // Reserved nastavene v memset, byty 5-7
    for(uint i=0; i<src_ip_u.size(); ++i) // Target IP addr
        hdr[8+i] = src_ip_u[i];
    hdr[24] = 0x02;                       // Opts type
    hdr[25] = 0x01;                       // Opts length
    for(uint i=0; i<MACAddr::OCTETS; ++i) // Source MAC
        hdr[26+i] = m_src_hwa_o[i];
    uint16_t chks = checksum(hdr);
    memcpy(hdr+2, &chks, S_USHORT);
    return hdr;
}

///
/// \brief Vytvori Echo Ping hlavicku
/// \return Echo Ping hlavicku
///
uchar *IcmpV6Pkt::serialize_echo()
{
    uchar *hdr        = new uchar[payload_length()];
    UchrVect src_ip_u = m_src_ip->to_uchar();
    memset(hdr, 0, payload_length());
    hdr[0] = PING_REQ_TYPE;            // Type 128 (0x80)
    // Code a checksum [1-3] v memset
    m_echo_id[0] = rand() % 255;          // Identifier
    m_echo_id[1] = rand() % 255;
    hdr[4] = m_echo_id[0];
    hdr[5] = m_echo_id[1];
    hdr[7] = 0x01;                        // Sequence
    for(uint i=0; i<src_ip_u.size(); ++i) // Source IP addr ako data
        hdr[8+i] = src_ip_u[IPv6Addr::BYTES -1 - i];
    uint16_t chks = checksum(hdr);
    memcpy(hdr+2, &chks, S_USHORT);
    return hdr;
}

///
/// \brief Vypocita kontrolny sucet ICMPv6 hlavicky
/// \param icmp ICMPv6 hlavicka
/// \return checksum
///
uint16_t IcmpV6Pkt::checksum(uchar *icmp)
{
    uint16_t tmp16;
    uchar    tmp8[2];
    UchrVect src_ip_u = m_src_ip->to_uchar();
    UchrVect dst_ip_u = m_dst_ip->to_uchar();
    uint32_t sum      = 0;

    // IPv6 Pseudo-header
    for(uint i=0; i<src_ip_u.size(); i+=2)
    {
        tmp8[0] = src_ip_u[i];
        tmp8[1] = src_ip_u[i+1];
        memcpy(&tmp16, tmp8, S_USHORT);
        sum += tmp16;
    }

    for(uint i=0; i<dst_ip_u.size(); i+=2)
    {
        tmp8[0] = dst_ip_u[i];
        tmp8[1] = dst_ip_u[i+1];
        memcpy(&tmp16, tmp8, S_USHORT);
        sum += tmp16;
    }
    sum += htons(payload_length());
    sum += htons(TYPE_NR);

    // ICMPv6 Body
    for(uint i=0; i<payload_length(); i += 2)
    {
        tmp8[0] = icmp[i];
        tmp8[1] = icmp[i+1];
        memcpy(&tmp16, tmp8, S_USHORT);
        sum += tmp16;
    }

    sum += sum >> 16;
    return (uint16_t)~sum;
}

///
/// \brief Spocita flags byte pre NA
/// \return flags byte
///
uchar IcmpV6Pkt::na_flags()
{
    // 7 6 5 4 3 2 1 0
    // R S O 0 0 0 0 0
    uchar rslt = 0x00;
    if(m_na_flag_router)    // R
        rslt |= 1 << 7;
    if(m_na_flag_solicited) // S
        rslt |= 1 << 6;
    if(m_na_flag_override)  // O
        rslt |= 1 << 5;
    return rslt;
}
