#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <mytcp.h>
#include <myparser.h>
#include <sys/stat.h>
#include <signal.h>
#include "netkat.h"  // MACROS Y DEMAS

// SI NO HAY NADA EN STDIN Y CORTARON CONEXION DE SOCKET, SALIR DE PROGRAMA

struct nkparams
{
  char *host;
  int port;
  BYTE t;  // TIMEOUT
  BYTE r;  // READ
  BYTE w; // WRITE
  BYTE e;  // EXECUTE
  BYTE v;  // VERBOSE
  BYTE u;  // UNIX2DOS
  BYTE S;  // SERVER
  BYTE s;  // SYNCHRONOUS
} params;


// FUNCIONES DEL PROGRAMA
int read_write(int first[2], fd_set *tinfd, fd_set *infd);
int proccess_buffer(int infd, int outfd, char *buffer); // REALIZA PROCESO ADICIONAL EN BUFFER
void execute_command(int client, int argc, char *argv[]);
void print_host_stats(struct hostent *he);
void help_exit();
void init_params(int argc, char *argv[]);



// MACROS INTERESANTES PARA EL PROGRAMA
#define PARAMINDEX(s) (paramindex(s, argc, argv))
#define IFV(s) if(params.v){ s; fflush(NULL); }


main(int argc, char *argv[])
{
unsigned long int contador;
char *charptr;
int client;
int first[2], second[2];
char hname[200];
struct hostent *he;
int aux;
fd_set infd, tinfd;
struct timeval tv, *ptrtv;

// SACAR TODOS LOS PARAMETROS
init_params(argc, argv);


///////////////////////// SACAR NOMBRE HOST LOCAL  /////////////////////////////
// SI ESTAMOS EN MODO CLIENTE Y (1 SOLO PARAMETRO O -v)
gethostname(hname, 200);
IFV(printf("Local host: %s\n\n", hname));

if(!params.S)
{ // SI ESTAMOS EN MODO CLIENTE SACAR ESTADISTICAS DEL HOST REMOTO
  /*in_addr i;
  he=gethostbyaddres(&i, SIZEOF(i), AF_INET);*/
  he=gethostbyname(params.host);
  if(!he) perror_exit("Host not found");
  IFV(print_host_stats(he));
}

////////// A PARTIR DE AQUI SOLO SE CONECTA SI SE PASO PUERTO OMO ULTIMO PARAMETRO
if(NPARAMS==1) return 0; // SI SOLO HUBO UN PARAMETRO, SOLO QUERIAMOS COMPROBAR EL HOST, ADIOS

// SI LLEGAMOS AQUI, EL Nº DE PUERTO ES EL ULTIMO PARAMETRO

//PRINTV(NPARAMS);

///////////////////////////////////////////////////////////////////////////////////
// AQUI SE DECIDE EL COMPORTAMIENTO PRINCIPAL DEL PROGRAMA CLIENTE/SERVIDOR
if(!params.S) // CLIENTE
{
  charptr=he_addr(he, 0);  // LO INTENTAMOS CON LA 1A DIRECCION
  IFV( printf("\nTrying to connect to %s on port %d... ", charptr, params.port) );
  client=socket_connect(charptr, params.port);
  if(client==-1) perror_exit("Error connecting to server");
  IFV(printf("connected.\n"));
  // CONFIGURACION DE ORDEN DE LECTURA DE CANALES EN MODO CLIENTE
  first[0]=0; first[1]=client;
  second[0]=client; second[1]=1;
}
else   //SERVIDOR //
{
  IFV(printf("Listening on local port %d... ", params.port));
  client=quick_accept(params.port);
  if(client==-1) perror_exit("Cannot bind to specified port");
  IFV(printf("connected.\n"));

  close(aux);
  IFV(printf("Reverse host lookup\n"));
  IFV(print_host_stats(host_lookup(client)));
  
  if(params.e) execute_command(client, argc, argv);  // ESTA ES LA MEJOR OPCION...
  // CONFIGURACION DE ORDEN DE LECTURA DE CANALES EN MODO SERVIDOR
  first[0]=client; first[1]=1; // PRIMERO LEER DE SOCKET Y ESCRIBIR A STDOUT
  second[0]=0; second[1]=client;  // LUEGO "RESPONDER", LEYENDO DE STDIN Y ESCRIBIENDO A SOCKET

  // DAR TIEMPO AL CLIENTE A QUE NOS MANDE ALGO
  uwait(100); // ESPERAR 100 milisegundos
}

// INICIALIZAR LA ESTRUCTURA PARA select()
if(params.t==-1) ptrtv=NULL; // LE PASAREMOS NULL A select
else
{
  BZERO(&tv);
  tv.tv_sec=params.t;
  ptrtv=&tv;
}

// INICIALIZAR VBLES DE CONTROL DEL BUCLE PRINCIPAL
FD_ZERO(&infd);
FD_ZERO(&tinfd);
FD_SET(first[0], &infd); // ANYADIR fd AL CONJUNTO DE DESCRIPTORES
FD_SET(second[0], &infd);

// DESCARTAR CANALES QUE NO NOS INTERESEN
// LEER SOLO DE SOCKET
if(params.r)
  if(params.S) FD_CLR(second[0], &infd); 
  else FD_CLR(first[0], &infd);
// SOLO ESCRIBIR EN SOCKET
if(params.w) 
  if(params.S) FD_CLR(first[0], &infd);  
  else FD_CLR(second[0], &infd);         


////////////////////////////////////////////////////////////////////////////////
////////////// BUCLE PRINCIPAL DE LECTURA/ESCRITURA ////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// SERVIDOR: SOCKET->STDOUT;
//           STDIN->SOCKET;
//
// CLIENTE:  STDIN->SOCKET;
//           SOCKET->STDOUT;
//
// USO MUY IMPORTANTE DE tinfd E infd PARA EL CONTROL DE FLUJO DE DATOS
////////////////////////////////////////////////////////////////////////////////
tinfd=infd;
while(FD_ISSET(first[0], &infd) || FD_ISSET(second[0], &infd))  // MIENTRAS HAY CONEXION EN ALGUN SENTIDO
{
  // PRIMERO UN SELECT DE TANTEO
  
  if(select(MAX(first[0], second[0])+1, &tinfd, NULL, NULL, ptrtv)==-1)
    perror_exit("Fatal error");
  
  // SI NO OCURRIO NADA DE INTERES, SALIR A LO BURRO
  if(!FD_ISSET(first[0], &tinfd) && !FD_ISSET(second[0], &tinfd))
  {
    IFV(printf_exit("\nTimeout!!!"));
    exit(0);
  }
//if(FD_ISSET(first[0], &tinfd)) printf("\nhay algo en first[0]=%d\n", first[0]);
//if(FD_ISSET(second[0], &tinfd)) printf("\nhay algo en second[0]=%d\n", second[0]);

  read_write(first, &tinfd, &infd);  // MODIFICAN infd PARA SABER CUANDO LLEGAMOS A EOF
  fflush(NULL);
  read_write(second, &tinfd, &infd); // SACANDO EL DESCRIPTOR CORRESPONDIENTE DE ENTRADA
  fflush(NULL);
  tinfd=infd;
  
}// DEL WHILE

IFV(printf("\nnk finished gracefully.\n"));
close(client);
return 0;
}

//////////////////////////////////////////////////////////////////////////////


int read_write(int channel[2], fd_set *tinfd, fd_set *infd)
{
int aux;
char buffer;
struct timeval tv;

BZERO(&tv);
// SI YA HABIAMOS SACADO channel[0] DE infd EN EL BUCLE PRINCIPAL NI ENTRAMOS POR AQUI
while(FD_ISSET(channel[0], tinfd) || (params.s && FD_ISSET(channel[0], infd)))
{  // MIENTRAS HAYA QUE LEER EN EL CANAL O HAGAMOS LECTURA DE UN TACADO HASTA FIN DE FICHERO

/*if(channel[1]==1) printf("socket->stdout");
else printf("stdin->socket");*/
  aux=read(channel[0], &buffer, 1);
//printf(" %c-(%d)\n", buffer, buffer);

  switch(aux)  // netcat CIERRA LA CONEXION CUANDO DETECTA UN EOF EN LA ENTRADA DEL SOCKET
  {
    case -1: // CIERRE DE CONEXION, ERROR CUANDO APRETAMOS EL BOTON DESCONECTAR DEL EXPLORER OCURRE ESTO
             IFV(perror("Read error"));
             IFV(printf("\n["); channel[0]>2?printf("remote"):printf("local"); printf(" SHUTDOWN]\n"););
             shutdown(channel[0], 0); // ASI AVISAMOS AL LISTENER DEL OTRO SOCKET ???????
             shutdown(channel[1], 1); // SI ENCONTRAMOS SHUTDOWN DE ENTRADA, CERRAR CANAL EN SENTIDO RED
             FD_CLR(channel[0], infd); 
             FD_CLR(channel[0], tinfd);
             break;

    case 0:  //EOF
             IFV(printf("\n["); channel[0]>2?printf("remote"):printf("local"); printf(" EOF]\n"););
             shutdown(channel[0], 0); // ASI AVISAMOS AL LISTENER DEL OTRO SOCKET ???????
             shutdown(channel[1], 1); // SI ENCONTRAMOS EOF DE ENTRADA, CERRAR CANAL EN SENTIDO RED
             FD_CLR(channel[0], infd); 
             FD_CLR(channel[0], tinfd);
             break;

    default: proccess_buffer(channel[0], channel[1], &buffer);
             write(channel[1], &buffer, 1);

  } // DEL SWITCH

// SI NO LLEGAMOS A FIN DE FICHERO, MIRAR A VER SI QUEDAN MAS CARACTERES
// SI ES ASI, REPETIMOS, SI NO, SALIMOS
  if(select(channel[0]+1, tinfd, NULL, NULL, &tv)==-1)
    perror_exit("Fatal error");
  /*else
  if(!FD_ISSET(channel[0], tinfd)) printf("no queda nada en %d!!!\n", channel[0]);*/

 }
return aux;
}

///////////////////////////////////////////////////////////////////////////////
void print_host_stats(struct hostent *he)
{
int aux;
char *charptr;

if(he==NULL)
{
  perror("Printing host stats");
  return;
}

printf("Remote host: %s\n", he_oname(he));

printf("List of aliases: \n");
for(aux=0; he_alias(he, aux); aux++)
  printf("Alias %d: %s\n", aux+1, he_alias(he, aux));

printf("\nList of addresses:\n");
for(aux=0; charptr=he_addr(he, aux); aux++)
  printf("Address of %d: %s\n",aux+1, charptr);
}


void help_exit()
{
puts("nk - Netcat CopyKat: Redirect I/O from/to net.\n");
puts("Usage: nk [options] <host|-S> [port]\n");
puts("  host:         Remote host name/address to connect.");
puts("  -S:           Server (listen) mode.");
puts("  port:         Port to bind.");

puts("\nAvaliable options:\n");
puts("  -<r|w>:       Net <read|write> only.");
puts("  -t timeout:   Timeout for inbound/outbound connections.");
puts("  -s:           \"Synchronous\" mode - read stdin/net at once.");
puts("  -u:           Unix->DOS CR/LF conversion.");
puts("  -e <command>: (Only w/ -S) Execute command and redirect I/O to client.");
puts("  -v:           On by default if no port specified).");
puts("  -h:           Show this (crappy) help.");

puts("\nNetKat v1.0 by BadPacket\n");
exit(0);
}


void execute_command(int client, int argc, char *argv[])
{
int fd;

close(0);  // REDIRECCIONAR
if(dup(client)==-1) perror_exit("Error redirecting stdin");
close(1);
if(dup(client)==-1) perror_exit("Error redirecting stdout");
close(2);
if(dup(client)==-1) perror_exit("Error redirecting stderr");

close(client);
execl(argv[params.e+1], argv[params.e+1], NULL);
perror_exit("Error on command execution");

}


int proccess_buffer(int infd, int outfd, char *buffer) // REALIZA PROCESO ADICIONAL EN LA ENTRADA
{
if((*buffer==10) && params.u)////CORREGIR CR/LF SOLO SI SE ESPECIFICA
{
  *buffer=13;
  write(outfd, buffer, 1);
  *buffer=10;
}
return 0;
}

void uwait(unsigned int msecs)
{
/*struct itimerval t, a;
BZERO(&t);
t.it_value.tv_usec=msecs;
setitimer(ITIMER_REAL, &t, &a);
pause();*/
struct timeval tv;

BZERO(&tv);
tv.tv_usec=msecs;
select(0, NULL, NULL, NULL, &tv);
}


void init_params(int argc, char *argv[])
{
struct stat fstate;

BZERO(&params);
if(NPARAMS==0) help_exit();    // SI NO INTRODUCIMOS PARAMETROS O PEDIMOS AYUDA

params.v=PARAMINDEX("-v") || (NPARAMS==1);

// SI ESPECIFICAMOS SOLO UN PARAMETRO ASUMIMOS QUE ES UN HOST O PETICION DE AYUDA
// SI HAY MAS PARAMETROS, ASUMIMOS QUE LO ULTIMO ES UN NUMERO DE PUERTO VALIDO
if(NPARAMS==1) // ESTO ES SI: nk <host>
{
  if(PARAMINDEX("-h")) help_exit();
  params.host=argv[1]; // SI SOLO HAY NOMBRE DE MAQUINA LO COGEMOS
  return; // YA MIRAMOS TODO LO QUE HABIA QUE MIRAR
}
else  // AHORA EL HOST SERA EL PENULTIMO (nk ... <host | -S> <port>
  if(params.S=(PARAMINDEX("-S")==(NPARAMS-1))); // SI SOMOS SERVIDOR (nk ... -S <port>)
  else params.host=argv[NPARAMS-1]; // SI NO, *SOMOS* CLIENTE (nk ... <host> <port>)


params.port=atoi(argv[NPARAMS]); // DEBE SER UN NUMERO DE PUERTO VALIDO, SI NO... SALE UN CERO
if((params.port<1) || (params.port>65535)) printf_exit("Port number between [1..65535] expected\n");

if(params.r && params.w) printf_exit("Only one at a time -<r|w>");


// COMPROBAR RESTO PARAMETROS
params.r=PARAMINDEX("-r");
params.u=PARAMINDEX("-u");
params.s=PARAMINDEX("-s");
params.r=PARAMINDEX("-r");
params.w=PARAMINDEX("-w");
params.e=PARAMINDEX("-e");

if(params.e)  // VERIFICAR EXISTENCIA DEL EJECUTABLE ANTES DE NADA
{
  if(params.e>(NPARAMS-2)) printf_exit("Invalid value for -e: Needs value");// ?????????
  
  if(stat(argv[params.e+1], &fstate)==-1) perror_exit("Invalid value for -e");
  if(!S_ISREG(fstate.st_mode)) printf_exit("Invalid value for -e: Not a file");
}

if(params.t=PARAMINDEX("-t"))  // VERIFICAR FORMATO DE -t INMEDIATAMENTE
  params.t=(params.t<(NPARAMS-1))?atoi(argv[params.t+1]):-1;
else
  params.t=-1;  // -1 SIGNIFICA QUE NO SE PASO PARAMETRO VALIDO
return;
}





/*
void es_help_exit() // AYUDA EN ESPAÑOL
{
puts("Uso: nk <host | -S> [puerto] [opciones]");
puts("Opciones disponibles:\n");
puts("host:       Host remoto.");
puts("-S:         Modo servidor.");
puts("puerto:     Puerto al que conectarse.");
puts("-<r|w>:     Unicamente <lectura | escritura> de red");
puts("-t timeout: Tiempo limite de escucha antes de desconexion.");
puts("-s:         Modo \"sincrono\" - escucha de red/stdin hasta llegar a EOF.");
puts("-u:         Conversion de fin de linea Unix->DOS.");
puts("-e:         Ejecuta comando redireccionando su E/S al cliente (Solo con -S).");
puts("-v:         Salida verbosa (si especificamos puerto).");
puts("-h:         Muestra esta ayuda.");
exit(0);
}
*/
