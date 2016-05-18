#ifndef LIBRERIAUMC_H_
#define LIBRERIAUMC_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include </home/utnso/so-commons-library/src/commons/config.h>
#include </home/utnso/so-commons-library/src/commons/string.h>
#include <unistd.h>
#include <semaphore.h>
#include </home/utnso/so-commons-library/src/commons/collections/queue.h>
#include <fcntl.h>           /* para las constantes O_* de los semaforos ipc  */
#include <sys/stat.h>        /* para las constantes de modo de los semaforos ipc */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <errno.h>
#define FLAG 10
#define BACKLOG 10



//void recibirCPU();

//Estructura que almacena los datos del archivo de configuracion
typedef struct{
	int puertoUMC;
	int puertoCPU;
	char* algoritmoPlanificacion;
	int quantum;
	int cantidadHilosCpus;
	int puertoSWAP;
	char* ipSWAP;
} contexto;

contexto miContexto;

void cargarContexto();


typedef struct
{
 int tipo_de_ejecution;
 int tamanio_del_msj;
} t_instruccion;

typedef struct
{
	int numCpu;
	int socket;
	bool enUso;
} t_cpu;

//struct para conexiones
struct Conexiones {
	int socket_escucha;					// Socket de conexiones entrantes
	struct sockaddr_in direccion;		// Datos de la direccion del servidor
	socklen_t tamanio_direccion;		// Tamaño de la direccion
	t_cpu* CPUS;                        //apunta al primer elemento del vector dinamico
} conexiones;

void *escuchar (struct Conexiones* conexion);

//void comunicarCPU(int socketCliente);

struct addrinfo hints_server;
struct addrinfo *serverInfo_server;

//Flag
typedef bool flag;
sem_t semSalir;
sem_t semEsperaCPU;
sem_t *semProduccionMsjs;
sem_t ordenIngresada;
sem_t semConsola;
sem_t semCpuLibre;
sem_t semEnvioPcb;
sem_t semBloqueados;


#endif /* LIBRERIAUMC_H_ */
