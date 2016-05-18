#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <commons/config.h>
#include "/home/utnso/tp-2016-1c-Syscall/sockets.h"
#include <pthread.h>
#include "libreriaUMC.h"
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>


int main() {

	//cargo la informacion de los archivos a la estructura contexto

	cargarContexto();


	//-------------CREAR UN SERVIDOR AL CUAL CONECTAR LA CPU------------------------------
	conexiones.socket_escucha = crearSocketDeEscucha(miContexto.puertoUMC);


	//permite al programa escuchar para detectar nuevas conexiones
	int L = listen(conexiones.socket_escucha, BACKLOG);
	if (L == -1)
		perror("LISTEN");

	//Se calcula el tamaño de la direccion del cliente
	conexiones.tamanio_direccion = sizeof(conexiones.direccion);

	/*
		 * Se crea un hilo nuevo que se queda a la espera de nuevas conexiones del CPU
		 * y almacena los sockets de las nuevas conexiones en la variable conexiones.CPU[]
		 */

		// alojo memoria dinamicamente en tiempo de ejecucion
		conexiones.CPUS= (t_cpu*)malloc(sizeof(t_cpu) * ((miContexto.cantidadHilosCpus) +1));
		if (conexiones.CPUS == NULL) puts("ERROR MALLOC 1");
		pthread_t hilo_conexiones;
		if(pthread_create(&hilo_conexiones, NULL, (void*)escuchar,&conexiones)<0)
			perror("Error HILO ESCUCHAS!");
		conexiones.CPUS[0].enUso = true;

		puts("PLANIFICADOR!\n ESTOY ESPERANDO CONEXIONES....\n\n\n");
		sem_wait(&semEsperaCPU); //semaforo espera conexiones


		close(conexiones.socket_escucha);
		int i=1;
		while(i<=miContexto.cantidadHilosCpus)
		{
			close(conexiones.CPUS[i++].socket);
		}
		free(conexiones.CPUS);


//-------------------------------CONEXION AL SWAP-------------------------------------

	 	struct sockaddr_in direccionSwap;

	 	direccionSwap.sin_family = AF_INET;

	 	direccionSwap.sin_addr.s_addr = inet_addr(miContexto.ipSWAP);

	 	direccionSwap.sin_port = htons(miContexto.puertoSWAP);

	 	int socketSwap = socket(AF_INET, SOCK_STREAM, 0);

	 	if (connect(socketSwap, (void*) &direccionSwap, sizeof(direccionSwap))

	 			!= 0) {

	 		perror("No se pudo conectar");

	 		return 1;

	 	}

	 	socketSwap= conectarAServidor(miContexto.ipSWAP,miContexto.puertoSWAP);

	 	char* buffer = malloc(200);

	  	char* bufferSwap = malloc(200);

	  	int bytesRecibidos = recv(socketSwap, bufferSwap, 50, 0);

	 	bufferSwap[bytesRecibidos] = '\0';

	 	printf("Recibi %d bytes con el siguiente mensaje: %s\n", bytesRecibidos, bufferSwap);

	 	send(socketSwap, "Hola soy la UMC", 16, 0);

	 	bytesRecibidos = recv(socketSwap, bufferSwap, 50, 0);

	 	if (bytesRecibidos <= 0) {

	 				perror("La UMC se ha desconectado o etc");

	 			}

	 	bufferSwap[bytesRecibidos] = '\0';

	 	printf("Respuesta: %s\n", bufferSwap);

	 	send(socketSwap, buffer, strlen(buffer), 0);

	 	free(buffer);

	 	free(bufferSwap);

	return 0;

}
