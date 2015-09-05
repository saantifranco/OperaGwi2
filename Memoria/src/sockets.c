#include "sockets.h"

int Conectar(char* IP, int port) {
	//printf("Conctando... \n");
	char puerto[5];
	sprintf(puerto, "%d", port);

	int status;
	struct addrinfo host_info;
	struct addrinfo *host_info_list;

	memset(&host_info, 0, sizeof(host_info));
	host_info.ai_family = AF_INET;
	host_info.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(IP, puerto, &host_info, &host_info_list);
	if (status != 0) {
		//printf("getaddrinfo error");
	}

	int socketID = socket(host_info_list->ai_family,
			host_info_list->ai_socktype, host_info_list->ai_protocol);
	if (socketID == -1) {
		//log_error(logs, "Error de conexion");
		//puts("Error de conexion");
		return -1;
	}

	status = connect(socketID, host_info_list->ai_addr,
			host_info_list->ai_addrlen);
	if (status == -1) {
		//log_error(logs, "socket error");
		//puts("socket error");
		return -1;
	}

	return socketID;

}

int Aceptar(int socketID){
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int clientSocket = accept(socketID, (struct sockaddr *) &addr, &addrlen);
	printf("Se ha conectado un nuevo cliente\n");
	return clientSocket;
}

int Enviar(int socketID, char* contenido) {
	//int BytesEnviados = 0;
	int ConnectionStatus = 0;
	int size = strlen(contenido);
	//write(socketID, &size, sizeof(size));
	int n = send(socketID, &size, sizeof(size), 0);

	//ConnectionStatus = write(socketID, contenido, size);
	ConnectionStatus = send(socketID, contenido, size, 0);

	return ConnectionStatus;
}

int EnviarInt(int socketID, int contenido) {
	char* numero = malloc(20);
	bzero(numero, 19);
	sprintf(numero, "%d", contenido);
	//itoa(contenido, numero, 10);
	//printf("numero a enviar %s... \n", numero);
	int res = Enviar(socketID, numero);
	free(numero);
	return res;
}

int Escuchar(int puerto_escucha) {

	printf("Escuchando en puerto %d... \n", puerto_escucha);

	struct sockaddr_in address;
	int listenSocket;
	if ((listenSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		printf("error socket()\n");
		return -1;
	}

	int yes = 1;
	if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
			== -1) {
		printf("error setsockopt()\n");
		return -1;
	}
	int enable = 1;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(puerto_escucha);
	if (bind(listenSocket, (struct sockaddr*) &address, sizeof(address))
			== -1) {
		printf("error bind()\n");
		return -1;
	}

	if (listen(listenSocket, 10) < 0) {
		printf("error listen()\n");
		return -1;
	}

	return listenSocket;
}

int Desconectar(int socketID) {
	close(socketID);
	printf("Se cerro el socket %d\n", socketID);
	return 0;
}

char* Recibir(int socketID) {

	int datasize = 0;
	int read_size = 0;
	//int n = read(socketID, &datasize, sizeof(datasize));
	int n = recv(socketID, &datasize, sizeof(datasize), 0);
	//char mensaje [datasize];
	char* mensaje = malloc(datasize+1);
	//bzero(mensaje, datasize);
	//while(read_size > 0){
	//status = read(socketID, mensaje, datasize);
	//read_size = read(socketID, mensaje, datasize);
	//if(datasize>0){
//	printf("DATASIZE %d \n", datasize);
	while(read_size < datasize){
		read_size += recv(socketID, mensaje+read_size, datasize, 0);
		if(read_size < 1){
			printf("El socket se ha cerrado \n");
			exit(0);
		}
		//printf("%d BYTES LEIDOS DE %d \n", read_size, datasize);
	}
	mensaje[datasize] = '\0';
	//mensaje[datasize + 1] = '\0';
	//}
	//}
	//char* p = &mensaje[0];
	return mensaje;
}

Origen QuienSos(int socketID) {
	char* proc = Recibir(socketID);
	Origen origen;
	if (proc[0] == '0'){
		origen = PLANIFICADOR;
	} else if (proc[0] == '1'){
		origen = CPU;
	} else if (proc[0] == '2'){
		origen = MEMORIA;
	} else if (proc[0] == '3'){
		origen = SWAP;
	}else {
		origen = DESCONOCIDO;
	}
	free(proc);
	return (origen);
}
