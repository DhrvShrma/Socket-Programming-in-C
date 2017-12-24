#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

struct clientInfo {
	int id;
	char name[1024];
	struct sockaddr_in cliAdd;
};

struct clientInfo session[1024] = {0, {'\0'}, {0}};

int clientCount = -1, try[5] = {1,2,3,4,5};

void* fun (void * arg) {
	printf("Client %d.\n", clientCount);
	
	char buffer[1024] = {'\0'}, recvMsg[1024] = {'\0'};
	struct clientInfo info = *((struct clientInfo*) arg);
	int cliSock = info.id;

	int n = recv(cliSock, buffer, 1024, 0);
	perror("");

	session[clientCount].id = cliSock;
	session[clientCount].cliAdd = info.cliAdd;
	strcpy(session[clientCount].name, buffer);
	
	printf("%d ", session[clientCount].id);
	printf("%s\n", session[clientCount].name);
	printf("Connected to : %s\n", buffer);
	
	n = recv(cliSock, recvMsg, 1024, 0);
	printf("%s\n", recvMsg);
	if (strcmp(recvMsg, "Bye") == 0) {
		
		close(cliSock);
		exit(0);
	}
	else if ((strcmp(recvMsg, "list")) == 0) {
		send(cliSock, &session, sizeof(session), 0);
		printf(".\n\n");
	}
}

int main (int argc, char * argv[]) {
	int servSock, cliSock;
	struct sockaddr_in servAdd, cliAdd;
	int servPort, msgLen = 0, cliSize = 0, k = 0;
	char *servIP;
	pthread_t servThread;

	if (argc != 2) {
		printf("Usage: %s <SERVER_PORT>\n", argv[0]);
		exit(0);
	}

	servIP = "192.168.1.103";
	servPort = atoi(argv[1]);

	if ((servSock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = inet_addr(servIP);
	servAdd.sin_port = htons(servPort);

	if((bind(servSock, (struct sockaddr*) &servAdd, sizeof(servAdd))) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	printf("Listening...\n");
	if ((listen(servSock, 5))) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	while(1) {
		cliSize = sizeof (cliAdd);
		if ((cliSock = accept(servSock, (struct sockaddr*) &cliAdd, &cliSize)) < 0) {
			perror("Error");
			exit(EXIT_FAILURE);
		}
		char *cliIP = inet_ntoa(cliAdd.sin_addr);
		int cliPort = ntohs(cliAdd.sin_port);
		printf("%s : %d\n", cliIP, cliPort);
		struct clientInfo sess;
		sess.cliAdd = cliAdd;
		sess.id = cliSock;
		clientCount++;
		pthread_create(&servThread, NULL, &fun, (void*) &sess);

	}

	return 0;
}



/*
connect[]; int n;

server:
n->string;
pass string(n) to client
send(socket,&connect,sizeof(connect));

client:

recv string, extract n;
array of structure ar of units n-> type arr[n];
for(int i=0; i<n; i++) recv(socket,&arr[i],sizeof(type))*/