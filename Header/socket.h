#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define IPV4 AF_INET
#define UDP SOCK_DGRAM
#define DEFAULT 0
#define PORT 12681


//This header is for use in linux systems.

//This Prototype is to request a socket from the OS.
int socket( int domain, int type, int protocol);

//This Prototype is for the Client to connect to a server.
int connect( int socket, const struct sockaddr *addr, int addrlen );

//This Prototype is to bind an adress to a service
int bind( int s, const struct sockaddr name, int namelen );

//Returns the ip adress for host name
struct hostent *gethostbyname(const char *name)


#endif