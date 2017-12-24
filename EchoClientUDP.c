#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {

	int cliSock;
	struct sockaddr_in servAdd;
	struct sockaddr_in srcAdd;
	int servPort;
	char *servIP, *toSend;
	char message[1024];
	int msgLen;

	if (argc < 3 || argc > 4) {
		printf("Usage: %s <ServerIP> <ServerPort> <MessageToServer>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	servIP = argv[1];
	servPort = atoi(argv[2]);
	toSend = argv[3];

	if ((cliSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	memset(&servAdd, 0, sizeof(servAdd));
	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = inet_addr(servIP);
	servAdd.sin_port = htons(servPort);

	sendto(cliSock, toSend, sizeof(toSend), 0, (struct sockaddr*) &servAdd, sizeof(servAdd));

	int cliSize = sizeof(srcAdd);
	msgLen = recvfrom(cliSock, message, 1024, 0, (struct sockaddr*) &srcAdd, &cliSize);

	printf("%s\n", message);

	return 0;
}