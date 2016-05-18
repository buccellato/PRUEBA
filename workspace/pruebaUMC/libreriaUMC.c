#include "libreriaUMC.h"

void cargarContexto()
{
	t_config* configuracion_UMC = config_create("resources/configuracion_UMC.txt");
	t_config* configuracion_Hilos = config_create("resources/configuracion_Hilos.txt");

	// OBTENGO CONFIGURACION DEL ARCHIVO DE configuracion_UMC y del archivo configHilos
	miContexto.puertoUMC = config_get_int_value(configuracion_UMC, "PUERTO_UMC" );
	miContexto.puertoCPU = config_get_int_value(configuracion_UMC, "PUERTO_CPU");
	miContexto.algoritmoPlanificacion = config_get_string_value(configuracion_UMC, "ALGORITMO_PLANIFICACION" );
	miContexto.quantum= config_get_int_value(configuracion_UMC, "QUANTUM" );
	miContexto.puertoSWAP = config_get_int_value(configuracion_UMC, "PUERTO_SWAP");
	miContexto.ipSWAP = config_get_string_value(configuracion_UMC,"IP_SWAP");

	miContexto.cantidadHilosCpus = config_get_int_value(configuracion_Hilos, "CANTIDAD_HILOS");

}

void *escuchar (struct Conexiones* conexion){
	int i =1;
	semEsperaCPU.__align =0; // inicializa semaforo

	//conexion para el comando cpu
	conexion->CPUS[0].socket = accept(conexion->socket_escucha, (struct sockaddr *) &conexion->direccion, &conexion->tamanio_direccion);

	while( i <= miContexto.cantidadHilosCpus ) //hasta q recorra todos los hilos de cpus habilitados (10)
	{
		//guarda las nuevas conexiones para acceder a ellas desde cualquier parte del codigo
		conexion->CPUS[i].socket = accept(conexion->socket_escucha, (struct sockaddr *) &conexion->direccion, &conexion->tamanio_direccion);
		if(conexion->CPUS[i].socket==-1)
		{
			perror("ACCEPT");	//control error
		}
		conexiones.CPUS[i].numCpu = i;

		int socketCliente = conexion->CPUS[i].socket;

		//

		send(socketCliente,"Hola quien sos?",16,0);
			//------------------------------
			char* buffer = malloc(200);

			int bytesRecibidos = recv(socketCliente,buffer,50,0);

			if (bytesRecibidos <= 0) {
				perror("El cliente se ha desconectado o etc");
			}

			buffer[bytesRecibidos] = '\0';

			if (strcmp("Hola soy la CPU", buffer) == 0) {

				send(socketCliente,"Conexion Aceptada",18,0);

				bytesRecibidos = recv(socketCliente,buffer,50,0);

				if (bytesRecibidos <= 0) {
					perror("La CPU se ha desconectado o etc");
				}

				buffer[bytesRecibidos] = '\0';

				printf("Recibi %d bytes con la siguiente orden: %s\n", bytesRecibidos,
						buffer);

			} else {

				send(socketCliente,"Conexion Rechazada",19,0);

				shutdown(socketCliente, SHUT_RDWR);

			}


		//

		conexion->CPUS[i].enUso = false;

		sem_post(&semEsperaCPU); //avisa que hay 1 CPU disponible
		puts("NUEVO HILO ESCUCHA!\n");
		i++;
	}

	return NULL;
}
