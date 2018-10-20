// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "packet.h"

const uchar Packet::MAC_BCV4[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uchar Packet::MAC_BCV6[] = {0x33, 0x33, 0x00, 0x00, 0x00, 0x01};

///
/// \brief Konstruktor
/// \param src_mac MAC adresa odosielatela
///
Packet::Packet(MACAddr *src_mac)
{
    m_src_hwa = src_mac;
    m_dst_hwa = nullptr;

    for(uint i=0; i<MACAddr::OCTETS; ++i)
    {
        m_src_hwa_o[i] = m_src_hwa->octet(i);
        m_dst_hwa_o[i] = 0x00;
    }
    m_eth_prot = 0;
}

///
/// \brief Nastavi cielovu MAC adresu
/// \param dst_mac MACAddr
///
void Packet::set_dst_hwa(MACAddr *dst_mac)
{
    m_dst_hwa = dst_mac;
    for(uint i=0; i<MACAddr::OCTETS; ++i)
        m_dst_hwa_o[i] = m_dst_hwa->octet(i);
}

///
/// \brief Nastavi zdrojovu MAC adresu
/// \param src_mac MACAddr
///
void Packet::set_src_hwa(MACAddr *src_mac)
{
    for(uint i=0; i<MACAddr::OCTETS; ++i)
        m_src_hwa_o[i] = src_mac->octet(i);
}

///
/// \brief Zostavi Ethernetovu hlavicku
/// \param dest urcuje ci sa jedna o broadcast alebo unicast
/// \return ethernetovu hlavicku
///
uchar *Packet::eth_header(EthDest dest)
{
    uchar *hdr = new uchar[ETH_HDR_LEN];
    memset(hdr, 0, ETH_HDR_LEN);
    // Serialize Ethernet header
    for(uint i=0; i<MACAddr::OCTETS; ++i)
    {
        if(dest == EthDest::BC)
            hdr[i] = MAC_BCV4[i];           // Broadcast
        else if(dest == EthDest::MC)
            hdr[i] = MAC_BCV6[i];
        else
            hdr[i] = m_dst_hwa_o[i]; // Unicast
        hdr[MACAddr::OCTETS + i] = m_src_hwa_o[i];
    }
    memcpy(hdr+2*MACAddr::OCTETS, &m_eth_prot, S_USHORT);
    return hdr;
}
