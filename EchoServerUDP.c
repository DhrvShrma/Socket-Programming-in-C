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

	int servSock, msgLen;
	struct sockaddr_in servAdd;
	struct sockaddr_in cliAdd;
	int servPort;
	char buffer[1024];

	if (argc != 2) {
		perror("Error:\n");
		printf("Usage: %s, <ServerPort>", argv[0]);
	}

	servPort = atoi(argv[1]);

	if ((servSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	memset(&servAdd, 0, sizeof(servAdd));
	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdd.sin_port = htons(servPort);

	if ((bind(servSock, (struct sockaddr*) &servAdd, sizeof(servAdd))) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}
	int cliSize = sizeof (cliAdd);
	while(1) {
		recvfrom(servSock, buffer, 1024, 0, (struct sockaddr*) &cliAdd, &cliSize);
		printf("%s\n", buffer);
		sendto(servSock, buffer, strlen(buffer), 0, (struct sockaddr*) &cliAdd, sizeof(cliAdd));
	}

	return 0;
}