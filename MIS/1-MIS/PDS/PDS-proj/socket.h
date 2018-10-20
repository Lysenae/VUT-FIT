// Projekt: PDS - L2 MitM
// Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <unistd.h>

#include "types.h"
#include "packet.h"

///
/// \enum SocketStatus
/// \brief Stav socketu
/// \var SocketStatus::New je novo vytvoreny socket
/// \var SocketStatus::Opened je otvoreny socket
/// \var SocketStatus::Closed je zatvoreny socket
///
enum class SocketStatus
{
    New,
    Opened,
    Closed
};

///
/// \brief Socket
///
class Socket
{
public:
    Socket(int domain, int type, int protocol);
    int fd() const;
    SocketStatus status() const;
    SocketStatus open();
    SocketStatus close();
    int send(const void *buf, size_t len, int flags,
        const struct sockaddr *dest_addr, socklen_t addrlen);
    int send(Packet *p, int ifn, int flags = 0);
    int recv_from(void *buf, size_t len, int flags,
        struct sockaddr *src_addr = nullptr, socklen_t *addrlen = nullptr);
    void setopt(int level, int opt_name, std::string opt_value,
        uint opt_len);

private:
    int m_domain;
    int m_type;
    int m_protocol;
    int m_fd;
    SocketStatus m_status;
};

#endif // SOCKET_H
