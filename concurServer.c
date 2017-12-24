#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void* fun(void * arg) {
	int newSock = *((int*) arg);
	int msgLen = 0;
	char message[1024];
	while(1) {
		msgLen = recv(newSock, message, 1024, 0);
		message[strlen(message)] = '\0';
		printf("%s\n", message);
	}	
	close(newSock);
}

int main(int argc, char* argv[]) {
	int servSock, cliSock;
	struct sockaddr_in servAdd, cliAdd;
	char *servIP;
	char message[1024], buffer[1024];
	int servPort, msgLen = 0, cliSize;
	pthread_t servThread;

	if (argc != 2) {
		printf("Usage: %s <SERVER_PORT>", argv[0]);
	}

	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	servIP = "127.0.0.1";
	servPort = atoi(argv[1]);

	memset(&servAdd, 0, sizeof(servAdd));
	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = inet_addr(servIP);
	servAdd.sin_port = htons(servPort);

	if ((bind(servSock, (struct sockaddr*) &servAdd, sizeof(servAdd))) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	if ((listen(servSock, 5)) < 0) {perror("Error"); exit(EXIT_FAILURE);}
	else {printf("Listening...\n");}



	while(1) {

		cliSize = sizeof(cliAdd);
		if ((cliSock = accept(servSock, (struct sockaddr*) &cliAdd, &cliSize)) < 0) {perror("Error"); exit(EXIT_FAILURE);}

		pthread_create(&servThread, NULL, &fun, (void*) &cliSock);
	
	}
	return 0;
}