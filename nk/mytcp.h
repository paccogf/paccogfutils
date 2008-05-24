#ifndef _MYTCP_H_
#define _MYTCP_H_

#pragma interface
#include <stdio.h>
#include <string.h>   // memset
#include <unistd.h>  // write, read

#ifndef WIN32
#include <sys/time.h>     // struct timeval
#include <sys/types.h>   // socket
#include <sys/socket.h>   // bind

#include <netinet/in.h>          // SOCKET AF_INET
#include <arpa/inet.h>
#include <netdb.h>

#else
#include <winsock2.h>  // WINDOWS
#endif

#define BZERO(p) memset(p, 0, sizeof(*p))

// HOST OFFICIAL NAME
#define he_oname(he) ((char *)he->h_name)
// HOST ALIAS LIST
#define he_alias(he, i) ((char *)(he->h_aliases[i]))
// HOST ADDRESSES LIST
// SI he->h_addr_list[i] NO ES PTRO NULO, DEVOLVER SU DIRECCION MEDIANTE inet_ntoa
#define he_addr(he, i) ((char *) ( (he->h_addr_list[i])? inet_ntoa(*((struct in_addr *) (he->h_addr_list[i]))) : NULL))

#define nbwait(fd, secs) vnbwait(&fd, 1, secs)


//int _create_sockaddr_in(struct sockaddr_in *socket_st, char *address, int port); // socket
// CONEXION A SERVIDOR  DEVUELVE -1 SI ERROR
int socket_connect(char *address, int port); // socket, connect

// ESCUCHA DE CLIENTE DEVUELVEN -1 SI ERROR
int socket_bind(char *address, int port); // socket, bind
int accept_client(int listen_socket);// accept

int quick_accept(int port);
struct hostent * host_lookup(int fd);
void uwait(unsigned int msecs);

#endif

/*
 * Structures returned by network data base library.  All addresses are
 * supplied in host order, and returned in network order (suitable for
 * use in system calls).

Different from the linux versions - note the shorts..
struct  hostent {
        const char      *h_name;        //official name of host
        char    **h_aliases;    // alias list
        short   h_addrtype;     // host address type
        short   h_length;       // length of address
        char    **h_addr_list;  // list of addresses from name server
#define h_addr  h_addr_list[0]  // address, for backward compatiblity
};
//////////////////////
struct in_addr
{
  unsigned int s_addr;
};
///////////////////////////////
struct sockaddr_in
{
  short int sin_family;         // Address family
  unsigned short int sin_port;  //Port number
  struct in_addr sin_addr;      //Internet address

  //Pad to size of `struct sockaddr'.
  unsigned char  __pad[__SOCK_SIZE__ - sizeof(short int) - sizeof(unsigned short int) - sizeof(struct in_addr)];
};
*/
