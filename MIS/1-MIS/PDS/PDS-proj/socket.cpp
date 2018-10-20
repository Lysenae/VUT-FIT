// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "socket.h"

///
/// \brief Kontruktor
/// \param domain
/// \param type
/// \param protocol
///
Socket::Socket(int domain, int type, int protocol)
{
    m_domain   = domain;
    m_type     = type;
    m_protocol = protocol;
    m_fd       = OP_FAIL;
    m_status   = SocketStatus::New;
}

///
/// \brief Deskriptor
/// \return deskriptor
///
int Socket::fd() const
{
    return m_fd;
}

///
/// \brief Stav
/// \return stav
///
SocketStatus Socket::status() const
{
    return m_status;
}

///
/// \brief Otvori socket
/// \return aktualny stav, pri spravnom otvoreni SocketStatus::Opened
///
SocketStatus Socket::open()
{
    if(m_status != SocketStatus::Opened)
    {
        m_fd     = socket(m_domain, m_type, m_protocol);
        if(m_fd == OP_FAIL)
            perror("Socket::open failed");
        else
            m_status = SocketStatus::Opened;
    }
    return m_status;
}

///
/// \brief Zatvori socket
/// \return aktualny stav, pri spravnom zatvoreni SocketStatus::Closed
///
SocketStatus Socket::close()
{
    int r = OP_FAIL;
    if(m_status == SocketStatus::Opened)
    {
        r = ::close(m_fd);
        if(r == OP_FAIL)
            perror("Socket::close");
        else
            m_status = SocketStatus::Closed;
    }
    return m_status;
}

///
/// \see sendto
///
int Socket::send(const void *buf, size_t len, int flags,
const sockaddr *dest_addr, socklen_t addrlen)
{
    if(m_status == SocketStatus::Opened)
        return sendto(m_fd, buf, len, flags, dest_addr, addrlen);
    return OP_FAIL;
}

///
/// \brief Odosle data
/// \param p Packet
/// \param ifn cislo rozhrania
/// \param flags
/// \return mnozstvo odoslanych dat, OP_FAIL pri chybe
///
int Socket::send(Packet *p, int ifn, int flags)
{
    if(m_status == SocketStatus::Opened)
    {
        sockaddr_ll sa = p->sock_addr(ifn);
        return sendto(m_fd, p->serialize(), p->pktlen(), flags, (sockaddr*)&sa,
            sizeof(sa));
    }
    return OP_FAIL;
}

///
/// \see recvfrom
///
int Socket::recv_from(void *buf, size_t len, int flags, sockaddr *src_addr,
socklen_t *addrlen)
{
    if(m_status == SocketStatus::Opened)
        return recvfrom(m_fd, buf, len, flags, src_addr, addrlen);
    return OP_FAIL;
}

///
/// \see setsockopt
//
void Socket::setopt(int level, int opt_name,  std::string opt_value,
uint opt_len)
{
    if(m_status == SocketStatus::Opened)
    {
        setsockopt(m_fd, level, opt_name, opt_value.c_str(), opt_len+1);
    }
}
