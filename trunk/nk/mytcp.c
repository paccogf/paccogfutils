#include <mytcp.h>
#pragma implementation

/***********************************/

int _create_sockaddr_in(struct sockaddr_in *socket_st, char *address, int port)
{
memset((char *)socket_st, 0, sizeof(struct sockaddr_in));
socket_st->sin_family = AF_INET;
socket_st->sin_port = htons (port);

#ifndef WIN32
inet_aton(address, &(socket_st->sin_addr));
#else
socket_st->sin_addr.s_addr=inet_addr(address);
#endif

return socket(AF_INET, SOCK_STREAM, 0); //SE DEVUELVE ERROR DE socket
}

/******************************************************************/

int socket_connect(char *address, int port)
{
int socket_desc, _aux;
struct sockaddr_in aux;

socket_desc=_create_sockaddr_in(&aux, address, port);
if(socket_desc==-1) return -1;
// DEVUELVE -1 SI HAY FALLO
_aux=connect(socket_desc, (struct sockaddr *)&aux, sizeof(struct sockaddr_in));
if(_aux==-1) return _aux;

return socket_desc;
}

/*****************************************************************/

int socket_bind(char *address, int port)
{
int listen_socket, _aux;
struct sockaddr_in server_sockaddr;

listen_socket=_create_sockaddr_in(&server_sockaddr, address, port);
if(listen_socket==-1) return -1;

_aux=bind(listen_socket, (struct sockaddr *)&server_sockaddr, sizeof(struct sockaddr_in));
if(_aux==-1) return -1;

return listen_socket;  // COMO SABER RESULTADO DESDE LA LLAMADA?????????
}

/*****************************************************************************/

int accept_client(int listen_socket)
{
int socksize;
//int client_socket;
struct sockaddr_in aux_sockaddr;

socksize=sizeof(struct sockaddr_in);

//client_socket=
// SI PASAMOS NULL, USAR EL NUESTRO

return accept(listen_socket, (struct sockaddr *)&aux_sockaddr, &socksize);

//return client_socket;
}

/**************************************************************************/
// ACEPTAR UNA CONEXION Y CERRAR PUERTO DE ESCUCHA
// RETORNA -1 SI ERROR, DESCRIPTOR DEL SOCKET SI NO
int quick_accept(int port)
{
int aux;
int fd=socket_bind("0.0.0.0", port);
if(fd==-1) return -1;

aux=listen(fd, 0); // ESCUCHAR
if(aux==-1)  // SI ERROR, A JODERSE
{
  close(fd);
  return -1;
}

aux=accept_client(fd);  // ACEPTAR 1ª PETICION Y CERRAR
close(fd); // NO QUEREMOS ESCUCHAR MAS CONEXIONES ENTRANTES

if(aux==-1) return -1;

return aux;
}


// A PARTIR DE UN DESCRIPTOR DE SOCKET NOS DA UNA ESTRUCTURA HOSTENT
struct hostent * host_lookup(int fd)
{
struct sockaddr_in sockinfo;
int aux;

aux=sizeof(sockinfo);

if(getsockname(fd, (struct sockaddr *)&(sockinfo), &aux)==-1) return NULL; // ERROR EN errno

return gethostbyaddr((char *)&(sockinfo.sin_addr), sizeof(struct in_addr), AF_INET);
}
