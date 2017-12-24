#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
	int cliSock;
	struct sockaddr_in servAdd;
	int servPort;
	char* servIP;
	char* message;
	char buffer[1024];

	if (argc < 3 || argc > 4) {
		printf("Usage: %s <ServerIP> <ServerPort> <Message>\n", argv[0]);
	}

	servIP = argv[1];
	servPort = atoi(argv[2]);
	message = argv[3];

	if ((cliSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	memset(&servAdd, 0, sizeof(servAdd));
	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = inet_addr(servIP);
	servAdd.sin_port = htons(servPort);

	if ((connect(cliSock, (struct sockaddr*)&servAdd, sizeof(servAdd))) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}
	else {
		printf("Connected.");
	}
	int msgLen = strlen(message);	
//	printf("%s\n", message);
//	send(cliSock, message, msgLen, 0);
	int bytesRecv = recv(cliSock, buffer, 1024, 0);
	printf(">%s\n", buffer);

	return 0;
} 