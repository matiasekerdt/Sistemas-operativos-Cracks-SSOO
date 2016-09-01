/*
 * entrenador.h
 *
 */

#ifndef ENTRENADOR1_ENTRENADOR_H_
#define ENTRENADOR1_ENTRENADOR_H_

#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "commons/log.h"
#include "commons/config.h"
#include "commons/collections/queue.h"
#include "conexiones.h"

typedef struct {
	int port_Mapa;
	char* ip_Mapa;
} t_conexion;

typedef struct {
	t_list* objetivosPorMapa;
} t_hojaDeViaje;

typedef struct {
	char simbolo;
	char* nombre;
	char* rutaPokedex;
	int PID;
	int cantVidas;
	t_list * hojaDeViaje;
	t_list* vidas;
	t_list* objetivos;//En duda
} t_entrenador;

//Logger
t_log* logEntrenador;

//Configuracion
t_config configEntrenador;


#endif /* ENTRENADOR1_ENTRENADOR_H_ */
