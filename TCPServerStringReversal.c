#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
	int servSock, cliSock;
	struct sockaddr_in servAdd;
	struct sockaddr_in cliAdd;
	int servPort;
	char message[1024] = {0}, buffer[1024] = {0};
	int cliSize, msgLen;

	if(argc < 2) {
		printf("Usage: %s <ServerPort>\n", argv[0]);
	}

	servPort = atoi(argv[1]);

	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	memset(&servAdd, 0, sizeof(servAdd));
	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdd.sin_port = htons(servPort);

	if ((bind(servSock, (struct sockaddr*)&servAdd, sizeof(servAdd))) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	if(listen(servSock, 5) < 0) {
		perror ("Error");
		exit(EXIT_FAILURE);
	}
	else {
		printf("Listening...\n");
	}

	cliSize = sizeof(cliAdd);
	if ((cliSock = accept(servSock, (struct sockaddr*) &cliAdd, &cliSize)) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}
	msgLen = recv(cliSock, message, 1024, 0);
	int i = 0;
	for(i = 0; i < 10; i++) {
		printf("%c", message[i]);
	}
	printf("\n");
	message[strlen(message)] = '\0';
	printf("%s\n", message);
	printf("\n");

	char reverse[1024];
	int k = 0, n = strlen(message);
	for (i = n-1; i >= 0; i--) {
		reverse[k++] = message[i];
	}
	printf("%s\n", reverse);
	
//	printf(">%s\n", message);
//		printf("Type a message for client: ");
//		scanf("%s", buffer);
//		send(cliSock, buffer, strlen(buffer), 0);


	return 0;
}