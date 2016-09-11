/*
 * entrenador.c
 *
 */

#include "entrenador.h"

int main(int argc, char **argv) {
	//assert(("ERROR - No se pasaron argumentos", argc > 1)); // Verifica que se haya pasado al menos 1 parametro, sino falla

	//Parametros
	int i;
	for (i = 0; i < argc; i++) {
		if (i == 0) {
			datosEntrenador.nombre = argv[i + 1];
			printf("Nombre Entrenador: '%s'\n", datosEntrenador.nombre);
		}
		if (i == 1) {
			datosEntrenador.rutaPokedex = argv[i + 1];
			printf("Ruta Pokedex: '%s'\n", datosEntrenador.rutaPokedex);
		}
	}

	//assert(("ERROR - No se paso el nombre del entrenador como argumento", datosEntrenador.nombre != NULL));
	//assert(("ERROR - No se paso la ruta del pokedex como argumento", datosEntrenador.rutaPokedex != NULL));

	//Creo el archivo de Log
	logEntrenador = log_create("logEntrenador", "ENTRENADOR", 0, LOG_LEVEL_TRACE);

	datosEntrenador.hojaDeViaje = list_create();

	//Levanto los datos del metadata de Entrenador
	getMetadataEntrenador();
	int socketMapa;
	socketMapa=conectarseA("10.0.2.15",1982);
	//chequearObjetivos(datosEntrenador,'M');
	int bytesRecibidos;
	char* mensaje="holaE";
	char* respuesta=malloc(6);
	fflush(stdin);
	bytesRecibidos=recibir(&socketMapa,respuesta,6);

	printf("Mensaje recibido del mapa: %s \n", respuesta);
	enviar(&socketMapa, mensaje, 6);
	free(respuesta);
	close(socketDeMapa);

	//CONFIGURACION DEL ENTRENADOR


	//faltan los objetivos

	return EXIT_SUCCESS;
}


//Funcion que levanta los datos del entrenador

void getMetadataEntrenador() {

	t_config* configEntrenador = malloc(sizeof(t_config));
	configEntrenador->path = string_from_format("/home/utnso/Pokedex/Entrenadores/%s/metadata","Red");
	configEntrenador = config_create(configEntrenador->path);

	datosEntrenador.nombre = config_get_string_value(configEntrenador, "nombre");
	datosEntrenador.simbolo = config_get_string_value(configEntrenador, "simbolo");
	datosEntrenador.cantVidas = config_get_int_value(configEntrenador, "vidas");
	char** hojaDeViaje = config_get_array_value(configEntrenador, "hojaDeViaje");

	printf("El nombre del Entrenador es: %s \n", datosEntrenador.nombre);
	printf("El simbolo que representa al Entrenador es: %s \n",datosEntrenador.simbolo);
	printf("La cantidad de vidas del Entrenador es: %d \n", datosEntrenador.cantVidas);

	int i = 0;
	while (hojaDeViaje[i] != NULL) {
		t_mapa* mapa=malloc(sizeof(t_mapa));
		mapa->nombreMapa = hojaDeViaje[i];

		printf("El mapa que debe recorrer el datosEntrenador: %s \n",
				mapa->nombreMapa);

		char* strConcat = string_new();
		string_append(&strConcat, "obj[");
		string_append(&strConcat, mapa->nombreMapa);
		string_append(&strConcat, "]");

		//entrenador->mapa->objetivos=config_get_array_value(configEntrenador,"obj[PuebloPaleta]");

		mapa->objetivos = config_get_array_value(configEntrenador, strConcat);
		int j = 0;
		while (mapa->objetivos[j] != NULL) {

			if (mapa->objetivos[j + 1] != NULL) {
				printf("%s, ", mapa->objetivos[j]);

			} else {
				printf("%s \n", mapa->objetivos[j]);
			}

			j++;

		}

		t_config* configMapa = malloc(sizeof(t_config));

		configMapa->path = string_from_format("/home/utnso/Pokedex/Mapas/%s/metadata",mapa->nombreMapa);
		configMapa = config_create(configMapa->path);
		mapa->ip = config_get_string_value(configMapa, "IP");
		mapa->puerto = config_get_int_value(configMapa,"Puerto");
		printf("La IP del mapa %s es: %s \n", mapa->nombreMapa,mapa->ip);
		printf("El puerto del mapa %s es: %d \n", mapa->nombreMapa,mapa->puerto);

		list_add(datosEntrenador.hojaDeViaje, (void*)mapa);

		i++;
	}

	printf("La cantidad de mapas a recorrer es: %d \n", datosEntrenador.hojaDeViaje->elements_count);

	recorrerEPrintearLista(datosEntrenador.hojaDeViaje);

}

void recorrerEPrintearLista(t_list* unaLista){
	int i;
	t_mapa* unMapa=malloc(sizeof(t_mapa));
	for(i=0;i<unaLista->elements_count;i++){
		unMapa=(t_mapa*)list_get(unaLista,i);
		printf("%s \n",unMapa->nombreMapa);
		printf("%s \n",unMapa->ip);
		printf("%i \n",unMapa->puerto);
	}
}
//Cambia la posicion del entrenador segun determine el mapa.


void chequearVidas(t_entrenador* unEntrenador){
	unEntrenador->cantVidas--;
	if(unEntrenador->cantVidas==0){
		printf("Te quedaste sin vidas \n");
		//borrarDirectorioDeBill();
		//borra sus medallas
		shutdown(socketEntrenador,2);
	}else{
		printf("Perdiste una vida, te queda:%i \n",unEntrenador->cantVidas);
	}
}

void chequearObjetivos(t_entrenador* unEntrenador,char pokemon){
	t_mapa* mapaEnElQueEstoy=(t_mapa*)list_get(unEntrenador->hojaDeViaje,unEntrenador->mapaActual);

		int j=0;
		char objetivo;
		while(!(objetivo==pokemon) && mapaEnElQueEstoy->objetivos[j]!=NULL){
		memcpy(&objetivo, mapaEnElQueEstoy->objetivos[j], sizeof(char));
		j++;
		}
		mapaEnElQueEstoy->objetivos[j]=0;

		if(mapaEnElQueEstoy->objetivos[j+1]==NULL){
			if(list_size(unEntrenador->hojaDeViaje)==unEntrenador->mapaActual){
			//copiarMedallaDelMapa();
			printf("Eres un maestro pokemon completaste la aventura");
			//GenerarReporteDeAventura();
			}else{
			//copiarMedallaDelMapa();
			//conectarseConElSiguienteMapa();
			}
		}else{
			//AvisarAlMapaQueDeboSeguirAtrapandoPokemon();
		}
}

void avanzarPosicion(int* actualX, int* actualY, int destinoX, int destinoY){
	int posicionX = *actualX;
	int posicionY = *actualY;
	if (!alternateFlag) { //si esta en false entra
		if (posicionX > destinoX) {
			posicionX--;
		} else if (posicionX < destinoX) {
			posicionX++;
		}
		if (posicionY != destinoY) alternateFlag = true;

	} else if (alternateFlag) { //si esta en true entra
		if (posicionY > destinoY) {
			posicionY--;
		} else if (posicionY < destinoY) {
			posicionY++;
		}
		if (posicionX != destinoX) alternateFlag = false;
	}
	*actualX = posicionX;
	*actualY = posicionY;
}


void conectarseAlMapa(t_mapa* unMapa){

	if(conectarseA(unMapa->ip,unMapa->puerto) == 0){
		printf("Conexion realizada con exito \n");
	}
	else{
		printf("Conexion fracaso \n");
		exit(-1);
	}
}

void solicitarUbicacionPokenest(int* posx, int* posy){
	int posicionX;
	int posicionY;
	int bytesRecibidosX = recibir(&socketEntrenador, &posicionX,sizeof(int));
	int bytesRecibidosY = recibir(&socketEntrenador, &posicionY,sizeof(int));

	if(bytesRecibidosX > 0 && bytesRecibidosY > 0){
		printf("Se recibio el tamanio correctamente \n");
	}else{
		printf("Se recibio un tamanio distinto al esperado \n");
	}
	*posx = posicionX;
	*posy = posicionY;
}


void avanzarHastaPokenest(t_entrenador* unEntrenador, int posicionXPokenest, int posicionYPokenest){
	//todo no es necesario enviar el flag, ya lo conoce el entrenador
	//int flagMovimiento = 0;
	//enviar(&socketDeMapa, &flagMovimiento, sizeof(int));
	//recibir(&socketEntrenador, &flagMovimiento, sizeof(int));
	avanzarPosicion(&unEntrenador->posicion[0], &unEntrenador->posicion[1],
			posicionXPokenest, posicionYPokenest);
}

void pedirAvanzarUnaPosicion(){
	//Enviar lo que sea que maneje guido para las posiciones
}

void atraparUnPokemon(char pokemon,t_entrenador* unEntrenador){
	int resolucionCaptura;
	//Envio el pokemon que necesito el mapa me confirma la resolucion
	enviar(&socketDeMapa,&pokemon,sizeof(char));
	//Recibo lo que me responde
	recibir(&socketEntrenador,&resolucionCaptura,sizeof(int));
	if(resolucionCaptura==0){
		chequearObjetivos(unEntrenador,pokemon);
	}else{
		chequearVidas(unEntrenador);
	}
}

void interactuarConMapa(t_entrenador* unEntrenador){

	t_mapa* mapa = malloc(sizeof(t_mapa));
	mapa = list_get(unEntrenador->hojaDeViaje,unEntrenador->mapaActual);
	conectarseAlMapa(mapa);
	int i = 0;
	while(mapa->objetivos[i] != NULL){
		//Solicito la posicion de la pokenest de mi proximo objetivo
		//Envio el pokemon a atrapar al mapa
		int posicionX;
		int posicionY;
		if(enviar(&socketDeMapa, &mapa->objetivos[i], sizeof(char))!= -1){
				printf("Datos enviados satisfactoriamente \n");
			}
			else{
				printf("No se han podido enviar todos los datos \n");
			}
		//Se recibe la pos de la pokenest pasando por referencia y ya trae la pos
		solicitarUbicacionPokenest(&posicionX, &posicionY);
		//Confirmo no haber llegado a la pokenest
		if(unEntrenador->posicion[0]==posicionX && unEntrenador->posicion[1]==posicionY){
			//Solicito atrapar al pokemon ¡Llegue a la pokenest!
			char pokemon;
			memcpy(&pokemon, mapa->objetivos[i], sizeof(char));
			atraparUnPokemon(pokemon,unEntrenador);
		}else{
			//No llegue pido para seguir avanzando
			//Pido autorizacion para avanzar una posicion;
			avanzarHastaPokenest(unEntrenador, posicionX, posicionY);

		}

	}
}



