// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "arppkt.h"

///
/// \brief Konstruktor
/// \param t typ ARP paketu
/// \param ip IPv4 adresa zdroja
/// \param mac MAC adresa zdroja
///
ArpPkt::ArpPkt(ArpType t, IPv4Addr *ip, MACAddr *mac) : Packet(mac)
{
    m_type      = t;
    m_hw_t      = htons(ETH_HW_TYPE);
    m_prot_t    = htons(ETH_P_IP);
    m_op        = htons(m_type == ArpType::Request ? ARP_REQ : ARP_REPLY);
    m_hw_addl   = (uchar) MACAddr::OCTETS;
    m_prot_addl = (uchar) IPv4Addr::OCTETS;
    m_eth_prot  = htons(ETH_P_ARP);

    for(uint i=0; i<IPv4Addr::OCTETS; ++i)
        m_dst_ip[i] = 0;
    set_src_ip_addr(ip);
}

///
/// \brief Zmeni oktet IPv4 adresy zdroja
/// \param octet
/// \param value
///
void ArpPkt::set_src_ip_addr(uint octet, uchar value)
{
    if(octet < IPv4Addr::OCTETS)
        m_src_ip[IPv4Addr::OCTETS - octet - 1] = value;
}

///
/// \brief Zmeni oktet IPv4 adresy zdroja
/// \param octet
/// \param value
///
void ArpPkt::set_src_ip_addr(uint octet, std::string value)
{
    set_src_ip_addr(octet, str_to_uch(value));
}

///
/// \brief Zmeni IPv4 adresu zdroja
/// \param ipv4 adresa
///
void ArpPkt::set_src_ip_addr(IPv4Addr *ipv4)
{
    for(uint i=0; i<IPv4Addr::OCTETS; ++i)
    {
        if(ipv4 == nullptr)
            m_dst_ip[i] = 0;
        else
            set_src_ip_addr(i, ipv4->addr_grp(i));
    }
}

///
/// \brief Zmeni oktet IPv4 adresy ciela
/// \param octet
/// \param value
///
void ArpPkt::set_dst_ip_addr(uint octet, uchar value)
{
    if(octet < IPv4Addr::OCTETS)
        m_dst_ip[IPv4Addr::OCTETS - octet - 1] = value;
}

///
/// \brief Zmeni oktet IPv4 adresy ciela
/// \param octet
/// \param value
///
void ArpPkt::set_dst_ip_addr(uint octet, std::string value)
{
    set_dst_ip_addr(octet, str_to_uch(value));
}

///
/// \brief Zmeni IPv4 adresu ciela
/// \param ipv4
///
void ArpPkt::set_dst_ip_addr(IPv4Addr *ipv4)
{
    for(uint i=0; i<IPv4Addr::OCTETS; ++i)
    {
        if(ipv4 == nullptr)
            m_dst_ip[i] = 0;
        else
            set_dst_ip_addr(i, ipv4->addr_grp(i));
    }
}

///
/// \brief Vytvori sockaddr_ll pre, potrebne pre odosielanie
/// \param if_idx cislo sietoveho rozhrania
/// \return sockaddr_ll
///
sockaddr_ll ArpPkt::sock_addr(int if_idx)
{
    sockaddr_ll sock_addr;
    sock_addr.sll_family   = AF_PACKET;
    sock_addr.sll_protocol = htons(ETH_P_ARP);
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
/// \brief Vytvori bytovu reprezentaciu paketu
/// \return pole uchar
///
uchar *ArpPkt::serialize()
{
    uchar *buff = new uchar[BUFF_LEN];
    uchar *ehdr = eth_header(m_type == ArpType::Request ? EthDest::BC : EthDest::UC);
    memset(buff, 0, BUFF_LEN);
    for(uint i=0; i<ETH_HDR_LEN; ++i)
        buff[i] = ehdr[i];

    // Serialize ARP header
    memcpy(buff+offs(ArpField::HW_TYPE),    &m_hw_t,      S_USHORT); // HW type
    memcpy(buff+offs(ArpField::PROT_TYPE),  &m_prot_t,    S_USHORT); // Protocol type
    memcpy(buff+offs(ArpField::HW_ADDLN),   &m_hw_addl,   S_UCHAR);  // HW add length
    memcpy(buff+offs(ArpField::PROT_ADDLN), &m_prot_addl, S_UCHAR);  // Ptcl add ln
    memcpy(buff+offs(ArpField::OPCODE),     &m_op,        S_USHORT); // Opcode
    for(uint i=0; i<MACAddr::OCTETS; ++i) // Src/Dest HW addr
    {
        buff[offs(ArpField::SRC_HWA) + i] = m_src_hwa_o[i];
        buff[offs(ArpField::DST_HWA) + i] = m_dst_hwa_o[i];
    }
    for(uint i=0; i<IPv4Addr::OCTETS; ++i) // Src/Dest IP addr
    {
        buff[offs(ArpField::SRC_IPA) + i] = m_src_ip[i];
        buff[offs(ArpField::DST_IPA) + i] = m_dst_ip[i];
    }
    return buff;
}

///
/// \brief Dlzka paketu vratane Ethernet hlavicky
/// \return dlzku paketu
///
uint ArpPkt::pktlen()
{
    return LEN;
}

///
/// \brief Ziska z paketu MAC a IP adresu
/// \param pkt paket
/// \param len dlzka paketu
/// \param mac vystupny parameter pre MAC adresu
/// \param ip vystupny parameter pre IPv4 adresu
/// \return false pri chybe
///
bool ArpPkt::analyze_pkt(uchar *pkt, int len, MACAddr **mac, IPv4Addr **ip)
{
    if(m_type == ArpType::Response) // Len pre odpovede na requesty
        return false;

    UchrVect mac_u;
    if(len > 0 && (uint)len >= offs(ArpField::SRC_HWA) + MACAddr::OCTETS)
    {
        std::string ips = "";
        uint16_t t;
        uchar p[2];
        p[0] = pkt[12];
        p[1] = pkt[13];
        bool valid_pkt = false;
        memcpy(&t, p, S_USHORT);
        if(htons(t) == ETH_P_ARP && pkt[21] == ARP_REPLY) // ARP Reply only
        {
            valid_pkt = true;
            for(uint i=0; i<MACAddr::OCTETS; ++i)
            {
                if(pkt[i] != m_src_hwa_o[i])
                {
                    valid_pkt = false;
                    break;
                }
            }

            if(valid_pkt)
            {
                for(uint i=0; i<MACAddr::OCTETS; ++i)
                    mac_u.push_back(pkt[offs(ArpField::SRC_HWA) + i]);
                for(uint i=0; i<IPv4Addr::OCTETS; ++i)
                {
                    ips += std::to_string(
                        (int)pkt[offs(ArpField::SRC_IPA) + i]);
                    if(i < IPv4Addr::OCTETS-1)
                        ips += ".";
                }
            }
            if(ips != "")
                *ip = new IPv4Addr(ips);
            *mac = new MACAddr(mac_u);
        }
    }
    return *mac != nullptr && *ip != nullptr;
}

///
/// \brief Vrati polohu polozky v pakete
/// \param f polozka paketu
/// \return offset polozky
///
uint ArpPkt::offs(ArpField f)
{
    uint o = 0;
    switch(f)
    {
        case ArpField::HW_TYPE:    o = 0;  break;
        case ArpField::PROT_TYPE:  o = 2;  break;
        case ArpField::HW_ADDLN:   o = 4;  break;
        case ArpField::PROT_ADDLN: o = 5;  break;
        case ArpField::OPCODE:     o = 6;  break;
        case ArpField::SRC_HWA:    o = 8;  break;
        case ArpField::SRC_IPA:    o = 14; break;
        case ArpField::DST_HWA:    o = 18; break;
        case ArpField::DST_IPA:    o = 24; break;
        default: break;
    }
    return o + ETH_HDR_LEN;
}
