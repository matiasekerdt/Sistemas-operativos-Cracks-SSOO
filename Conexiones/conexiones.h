#ifndef SOCKET_H_
#define SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <commons/string.h>

typedef enum{
	ACCEPTED=0,
	ENTRENADOR,
	MAPA,
	POKEDEX_CLIENT,
	POKEDEX_SERVER
} enum_procesos;

typedef enum{
	NUEVO = 0,
	LISTO,
	EJECUTANDO,
	BLOQUEADO,
	SUSPENDIDO
} enum_EstadoProceso;

typedef struct {
	int socketServer;
	int socketClient;
} t_datosConexion;

typedef struct{
	int PID;
	int operacion;
} t_MensajeMapa_Entrenador;

typedef struct{
	int PID;
	int operacion;
} t_MensajeEntrenador_Mapa;

typedef struct{
	int PID;
	int operacion;
} t_MensajePokedexClient_PokedexServer;

typedef struct{
	int PID;
	int operacion;
} t_MensajePokedexServer_PokedexClient;

//IMPORTANTE --> Nomeclatura de Serializadores y Deserealizadores
//1) serializar<DesdeProceso>_<HastaProceso> ()
//2) deserialiar<HastaProceso>_<DesdeProceso> ()

void serializarEntrenador_Mapa(t_MensajeEntrenador_Mapa *value, char *buffer, int valueSize);
void deserializarMapa_Entrenador(t_MensajeEntrenador_Mapa *value, char *bufferReceived);

void serializarMapa_Entrenador(t_MensajeMapa_Entrenador *value, char *buffer, int valueSize);
void deserializarEntrenador_Mapa(t_MensajeMapa_Entrenador *value, char *bufferReceived);

void serializarPokedexClient_PokedexServer(t_MensajePokedexClient_PokedexServer *value, char *buffer, int valueSize);
void deserializarPokedexServer_PokedexClient(t_MensajePokedexClient_PokedexServer *value, char *bufferReceived);

void serializarPokedexServer_PokedexClient(t_MensajePokedexServer_PokedexClient *value, char *buffer, int valueSize);
void deserializarPokedexCliente_PokedexServer(t_MensajePokedexServer_PokedexClient *value, char * bufferReceived);

int ponerAEscuchar(int sockfd,int puertoServidor);
int enviar(char* envio,int socketAlQueEnvio,int tamanioDelEnvio);
int recibir(char* bufferReceptor,int socketReceptor,int tamanioQueRecibo);
int conectarseA(char* ipDestino,int puertoDestino);
int escucharMultiplesConexiones(int socketEscucha,int puertoEscucha);

#endif /*SOCKET_H_*/
