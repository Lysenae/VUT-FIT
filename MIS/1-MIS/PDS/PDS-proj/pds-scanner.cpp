// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include <csignal>
#include <libxml/xmlwriter.h>

#include "types.h"
#include "hash.h"
#include "netitf.h"
#include "arppkt.h"
#include "socket.h"
#include "icmpv6pkt.h"

using namespace std;

///
/// \brief Priznak urcujuci, ci sa bude este scanovat siet, SIGINT ho nastavuje
/// na false
///
bool search = true;

void print_usage();
void on_sigint(int signum);
bool search_ipv4_hosts(IPv4Addr *loc_ip, MACAddr *loc_mac, int ifn, Hash *hosts);
bool search_ipv6_hosts(IPv6Addr *loc_ip, MACAddr *loc_mac, int ifn, Hash *hosts);
bool writeHosts(Hash *hosts, string fname);

///
/// \brief main
/// \param argc
/// \param argv
/// \return
///
int main(int argc, char *argv[])
{
    int opt;
    string fname     = "";
    string interface = "";
    bool opts_ok     = true;

    while ((opt = getopt(argc, argv, "i:f:")) != -1)
    {
        switch (opt)
        {
            case 'i':
                interface = std::string(optarg);
                break;
            case 'f':
                fname = std::string(optarg);
                break;
            default:
                opts_ok = false;
                break;
        }
    }

    if(interface.empty() || fname.empty() || !opts_ok)
    {
        print_usage();
        return OP_FAIL;
    }

    signal(SIGINT, on_sigint);

    bool all_ok;
    Hash hosts;
    NetItf      *netitf      = new NetItf(interface);
    MACAddr     *loc_mac     = netitf->mac();
    IPv4Addr    *loc_ipv4    = netitf->ipv4();
    IPv6Vect     loc_ipv6s   = netitf->ipv6();
    IPv6Addr    *loc_ipv6_ll = nullptr;
    IPv6Addr    *loc_ipv6_gl = nullptr;
    int          ifn         = netitf->index();

    for(IPv6Addr *a : loc_ipv6s)
    {
        if(a->is_ll() && loc_ipv6_ll == nullptr)
            loc_ipv6_ll = a;
        if(a->is_global() && loc_ipv6_gl == nullptr)
            loc_ipv6_gl = a;
    }

    if(ifn < OP_SUCC)
    {
        cerr << "Failed to get netinterface ID" << endl;
        all_ok = false;
    }
    if(loc_mac == nullptr || loc_mac->empty())
    {
        cerr << "Failed to get local MAC address" << endl;
        all_ok = false;
    }
    if(loc_ipv4 == nullptr || loc_ipv4->empty())
    {
        cerr << "Failed to get local IPv4 address" << endl;
        all_ok = false;
    }
    if(loc_ipv6_ll == nullptr || loc_ipv6_ll->empty())
    {
        cerr << "Failed to get Link-Local IPv6 address" << endl;
        all_ok = false;
    }
    if(loc_ipv6_gl == nullptr || loc_ipv6_gl->empty())
    {
        cerr << "Failed to get global scope IPv6 address" << endl;
        all_ok = false;
    }

    if(all_ok && search)
    {
        cout << "Searching for IPv4 hosts" << endl;
        all_ok = search_ipv4_hosts(loc_ipv4, loc_mac, ifn, &hosts);
        cout << "Searching for IPv4 hosts copmleted" << endl;
    }

    if(all_ok && search)
    {
        cout << "Searching for Link-Local IPv6 hosts" << endl;
        all_ok = search_ipv6_hosts(loc_ipv6_ll, loc_mac, ifn, &hosts);
        cout << "Searching for Link-Local IPv6 hosts completed" << endl;
    }

    if(all_ok && search)
    {
        cout << "Searching for Global IPv6 hosts" << endl;
        all_ok = search_ipv6_hosts(loc_ipv6_gl, loc_mac, ifn, &hosts);
        cout << "Searching for Global IPv6 hosts completed" << endl;
    }

    if(all_ok)
        writeHosts(&hosts, fname);

    delete netitf;
    delete loc_ipv4;
    delete loc_mac;
    for(IPv6Addr *a : loc_ipv6s)
    {
        delete a;
        a = nullptr;
    }
    loc_ipv6s.clear();
    return all_ok ? OP_SUCC : OP_FAIL;
}

///
/// \brief Vypise pouzitie programu
///
void print_usage()
{
    cout << "Usage: pds-scanner -i interface -f file" << endl;
}

///
/// \brief SIGINT handler
/// \param signum
///
void on_sigint(int signum)
{
    cout << " SIGINT(" << signum << ") detected" << endl;
    search = false;
}

///
/// \brief Vyhlada IPv4 hostov v sieti
/// \param loc_ip IPv4 adresa volajuceho PC
/// \param loc_mac MAC adresa volajuceho PC
/// \param ifn cislo sietoveho rozhrania
/// \param hosts Zoznam najdenych hostov
/// \return false pri chybe
///
bool search_ipv4_hosts(IPv4Addr *loc_ip, MACAddr *loc_mac, int ifn, Hash *hosts)
{
    Socket s4(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if(s4.open() != SocketStatus::Opened)
    {
        cerr << "pds-scanner: Failed to open socket for ARP packets" << endl;
        return false;
    }

    StrVect      v4s       = loc_ip->net_host_ips();
    IPv4Addr    *v4another = nullptr;
    ArpPkt      *apkt      = new ArpPkt(ArpType::Request, loc_ip, loc_mac);
    MACAddr     *tm        = nullptr;
    IPv4Addr    *ipa       = nullptr;
    uchar        buf[ArpPkt::BUFF_LEN];

    cout << "Possible " << v4s.size() << " hosts" << endl;

    int rcvd;
    for(uint rpt=1; rpt<=2; ++rpt)
    {
        cout << "Cycle " << rpt << "/2" << endl;
        for(std::string ip : v4s)
        {
            if(!search)
                break;
            memset(buf, 0, ArpPkt::BUFF_LEN);
            v4another = new IPv4Addr(ip, loc_ip->snmask());
            apkt->set_dst_ip_addr(v4another);
            s4.send(apkt, ifn);
            rcvd = s4.recv_from(buf, ArpPkt::BUFF_LEN-1, 0);
            if(apkt->analyze_pkt(buf, rcvd, &tm, &ipa))
            {
                if(!tm->eq(loc_mac) && !tm->empty() && ipa->addr() == ip)
                {
                    cout << "Host found: " << ipa->addr() << " [" <<
                        tm->to_string() << "]" << endl;
                    hosts->add_value(tm->to_string(), ipa->addr());
                }
            }

            delete tm;
            tm = nullptr;
            delete v4another;
            v4another = nullptr;
        }
    }
    hosts->print();
    s4.close();
    delete apkt;
    return true;
}

///
/// \brief Vyhlada IPv6 hostov v sieti
/// \param loc_ip IPv6 adresa volajuceho PC
/// \param loc_mac MAC adresa volajuceho PC
/// \param ifn cislo sietoveho rozhrania
/// \param hosts Zoznam najdenych hostov
/// \return false pri chybe
///
bool search_ipv6_hosts(IPv6Addr *loc_ip, MACAddr *loc_mac, int ifn, Hash *hosts)
{
    cout << "Using IPv6 address " << loc_ip->addr() << endl;
    Socket s6(PF_PACKET, SOCK_RAW, htons(ETH_P_IPV6));
    if(s6.open() != SocketStatus::Opened)
    {
        cerr << "pds-scanner: Failed to open socket 1 for ICMPv6 packets" << endl;
        return false;
    }
    Socket s6_adv(PF_PACKET, SOCK_RAW, htons(ETH_P_IPV6));
    if(s6_adv.open() != SocketStatus::Opened)
    {
        s6.close();
        cerr << "pds-scanner: Failed to open socket 2 for ICMPv6 packets" << endl;
        return false;
    }

    IcmpV6Pkt *ping  = new IcmpV6Pkt(IcmpV6Type::Ping, loc_ip, loc_mac);
    IcmpV6Pkt *adv   = new IcmpV6Pkt(IcmpV6Type::NS, loc_ip, loc_mac);
    IPv6Addr *dst    = new IPv6Addr("ff02::1");
    ping->set_dst_ip_addr(dst);
    uchar *buf_v6    = new uchar[500];
    uint cnt         = 0;
    uint keys        = hosts->keys().size();
    std::string mac  = "";
    std::string ip6  = "";
    IPv6Addr *tip6   = nullptr;
    MACAddr *tmac    = nullptr;

    adv->set_multicast_flag(false);

    int rcvd;
    for(uint rpt=1; rpt<=2; ++rpt)
    {
        cout << "Cycle: " << rpt << "/2" << endl;
        s6.send(ping, ifn);

        int pl;
        while(true)
        {
            if(!search || keys == 0 || cnt == 50)
                break;
            rcvd = s6.recv_from(buf_v6, 500, 0);
            if(rcvd < (int)(Packet::ETH_HDR_LEN + IcmpV6Pkt::IPV6_HDR_LEN))
                continue; // Nemoze byt IPv6 packet
            pl = (int)buf_v6[19];
            if(pl == IcmpV6Pkt::PING_LEN || pl == IcmpV6Pkt::NS_LEN ||
            pl == IcmpV6Pkt::NA_LEN)
            {
                if(buf_v6[54] == IcmpV6Pkt::PING_RSP_TYPE ||
                buf_v6[54] == IcmpV6Pkt::NS_TYPE ||
                buf_v6[54] == IcmpV6Pkt::NA_TYPE)
                {
                    mac = "";
                    ip6 = "";
                    for(uint i=6; i<12; ++i)
                    {
                        mac += str_bytes8(buf_v6[i]);
                        mac += (i == 11 ? "" : ":");
                    }
                    if(hosts->has_key(mac))
                    {
                        for(uint i=22; i<38; ++i)
                        {
                            ip6 += str_bytes8(buf_v6[i]);
                            ip6 += (i < 37 && i % 2 == 1) ? ":" : "";
                        }
                        cout << "Host found: " << ip6 << " [" << mac << "]" << endl;
                        hosts->add_existing(mac, ip6);
                        tip6 = new IPv6Addr(ip6);
                        tmac = new MACAddr(mac);
                        adv->set_dst_ip_addr(tip6);
                        adv->set_dst_hwa(tmac);
                        s6_adv.send(adv, ifn);
                        delete tip6;
                        delete tmac;
                        tip6 = nullptr;
                        tmac = nullptr;
                        keys--;
                        if(keys == 0)
                            break;
                    }
                    cnt = 0;
                    continue;
                }
            }
            cnt++;
        }
    }
    s6.close();
    s6_adv.close();
    delete ping;
    delete dst;
    delete buf_v6;
    delete adv;
    hosts->print();
    return true;
}

////
/// \brief Zapise najdenych hostov do XML
/// \param hosts zoznam najdenych hostov
/// \param fname cesta k cielovemu suboru
/// \return false pri chybe
///
bool writeHosts(Hash *hosts, string fname)
{
    xmlTextWriterPtr writer;
    StrVect keys = hosts->keys();
    StrVect vals;

    writer = xmlNewTextWriterFilename(fname.c_str(), 0);
    if (writer == nullptr)
    {
        cerr << "Error creating the xml writer" << endl;
        return false;
    }

    if(xmlTextWriterStartDocument(writer, "1.0", "UTF-8", nullptr) < OP_SUCC)
    {
        cerr << "Failed to start XML document" << endl;
        return false;
    }

    if(xmlTextWriterStartElement(writer, BAD_CAST "devices") < OP_SUCC)
    {
         cerr << "Failed to write XML root element" << endl;
         return false;
    }

    for(string k : keys)
    {
        if(xmlTextWriterStartElement(writer, BAD_CAST "host") < OP_SUCC)
        {
            cerr << "Failed to write XML element 'host'" << endl;
            return false;
        }
        if(xmlTextWriterWriteAttribute(writer, BAD_CAST "mac",
        BAD_CAST MACAddr::to_xml(k).c_str()) < OP_SUCC)
        {
            cerr << "Failed to write device's MAC address" << endl;
            return false;
        }

        vals = hosts->values(k);
        for(std::string v : vals)
        {
            if(v.find_first_of(".") != string::npos) // IPv4
            {
                if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "ipv4",
                "%s", v.c_str()) < OP_SUCC)
                {
                    cerr << "Failed to write IPv4" << endl;
                    return false;
                }
            }
            else if(v.find_first_of(":") != string::npos) // IPv6
            {
                if(xmlTextWriterWriteFormatElement(writer, BAD_CAST "ipv6",
                "%s", v.c_str()) < OP_SUCC)
                {
                    cerr << "Failed to write IPv6" << endl;
                    return false;
                }
            }
            else
            {
                cerr << "Skipping invalid IP address" << endl;
                continue;
            }
        } // ipvx

        if(xmlTextWriterEndElement(writer) < OP_SUCC)
        {
            cerr << "Failed to end XML element 'host'" << endl;
            return false;
        }
    } // host

    if(xmlTextWriterEndElement(writer) < OP_SUCC)
    {
        cerr << "Failed to end root XML element" << endl;
        return false;
    }

    xmlFreeTextWriter(writer);
    return true;
}
