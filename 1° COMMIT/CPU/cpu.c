#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

int main(void)
 {
	struct sockaddr_in direccionNucleo;
 	direccionNucleo.sin_family = AF_INET;
	 direccionNucleo.sin_addr.s_addr = inet_addr("127.0.0.1");
	 direccionNucleo.sin_port = htons(8080);

	 int nucleo = socket(AF_INET, SOCK_STREAM, 0);

 	if (connect(nucleo, (void*) &direccionNucleo, sizeof(direccionNucleo)) != 0) 
	{
 		 perror("No se pudo conectar al Nucleo");
 		 return 1;
	 }
 
 	char* buffer = malloc(50);

 	int bytesRecibidos = recv(nucleo, buffer, 10, 0); 
 	buffer[bytesRecibidos]='\0';

 	printf("Me conecté a %d y recibí %d bytes con el mensaje: %s\n", nucleo,bytesRecibidos,buffer);

 	bytesRecibidos= recv(nucleo, buffer, 50, 0); 
 	buffer[bytesRecibidos]='\0';
 
 	printf("Tengo la siguiente orden: %s\n", buffer);
	 
	//Me conecto a la UMC
	struct sockaddr_in direccionUMC;
	direccionUMC.sin_family = AF_INET;
	direccionUMC.sin_addr.s_addr = inet_addr("127.0.0.1");
	direccionUMC.sin_port = htons(8888);

	int umc = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(umc, (void*) &direccionUMC, sizeof(direccionUMC)) != 0) 
	{
		perror("No se pudo conectar a la UMC");
		return 1;
	}

	send(umc, "Hola UMC soy la CPU", 20, 0);
	
	send(umc, buffer,strlen(buffer),0);

	return 0;

}

