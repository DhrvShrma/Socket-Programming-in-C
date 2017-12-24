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

void * connection (void * arg);
void * request (void* arg);
void * acceptCon (void* arg);

char name[1024] = {'\0'};

int main(int argc, char* argv[]) {
	int servSock, cliSock;
	struct sockaddr_in servAdd;
	int servPort, i;
	char *servIP;
	char message[1024], buffer[1024] = {'\0'};

	if (argc != 3) {
		printf("Usage: %s <SERVER_IP> <SERVER_PORT>\n", argv[0]);
		exit(0);
	}

	servIP = argv[1];
	servPort = atoi(argv[2]);

	if ((cliSock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = inet_addr(servIP);
	servAdd.sin_port = htons(servPort);

	if ((connect(cliSock, (struct sockaddr*) &servAdd, sizeof(servAdd))) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}
	else {
		printf("Connected.\n");
	}

	printf("Please complete your registration:\nType a name: ");
	scanf("%s", name);
	int msgLen = send(cliSock, (void*)name, 1024, 0);

	printf("Type something: ");
	scanf("%s", message);
	msgLen = send(cliSock, (void*)message, strlen(message), 0);
	struct clientInfo session[1024];
	msgLen = recv(cliSock, &session, sizeof(session), 0);

	if (strcmp(message, "list") == 0) {
		for (i = 0; session[i].id != 0; i++) {
			printf("%s\n", session[i].name);
		}
	}

	int j = 0;
	
	for(j = 0; strcmp(name, session[j].name)!=0; j++) {	}
	pthread_t myThread;
	pthread_create(&myThread, NULL, &connection, (void*)&session[j]);
	perror("myThread");

	pthread_t requestThread;
	char username[1024];int found = 0;
	printf("Type a username to connect : ");
	scanf("%s", username);
	for(i = 0; strcmp(username, session[i].name)!=0; i++) {
		if (i >= 5) {
			printf("User not found.\n");
			break;
		}
	}
	if (strcmp(username, session[i].name)==0) {
		found = 1;
		pthread_create(&requestThread, NULL, &request, (void*)&session[i]);
		perror("requestThread\n");
	}

}

void * request (void * arg) {
	struct clientInfo user = *((struct clientInfo*) arg);
	struct sockaddr_in userAdd = user.cliAdd;
	int reqSock;
	if((reqSock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}
	if (connect(reqSock,(struct sockaddr*)&userAdd,sizeof(userAdd)) < 0) {
		perror("Error");
		exit(EXIT_FAILURE);
	}

	printf("Connected to : %s\n", user.name);
	send(reqSock, name, 1024, 0);
	printf("Type Message: ");
	char message[1024] = {'\0'};
	scanf("%s", message);
	send(reqSock, message, 1024, 0);
}

void * connection (void * arg) {
	struct clientInfo session = *((struct clientInfo*)arg);
	struct sockaddr_in myself, requestAdd;
	int requestSize;
	int j = 0;
	pthread_t acceptThread;
	char userName[1024] = {'\0'};
	int acceptSock = socket(PF_INET, SOCK_STREAM, 0);
	perror("");
	myself = session.cliAdd;
	bind(acceptSock, (struct sockaddr*) &myself, sizeof(myself));
	listen(acceptSock, 5);
	perror("Listening");
	while(1) {
		requestSize = sizeof(requestAdd);
		int reqSock = accept (acceptSock, (struct sockaddr*) &requestAdd, &requestSize);
		struct clientInfo userSess;
		userSess.id = reqSock;
		recv(reqSock, userName, 1024, 0);
		strcpy(userSess.name, userName);
		userSess.cliAdd = requestAdd;
		pthread_create(&acceptThread, NULL, &acceptCon, (void*)&userSess);
	}
}

void * acceptCon (void * arg) {
	struct clientInfo user = *((struct clientInfo*)arg);
	int userSock = user.id;
	char buffer[1024] = {'\0'};
	int n = recv(userSock, buffer, 1024, 0);
	printf("%s : %s\n", user.name, buffer);
}
/*	for (i = 0; session[i].id != 0; i++) {
//		printf("%d ", try[i]);
		printf("%d : ", session[i].id);
		printf("%s\n", session[i].name);
		char *IP = inet_ntoa(session[i].cliAdd.sin_addr);
		int port = ntohs(session[i].cliAdd.sin_port);
		//printf("%s : ", IP);
		//printf("%d\n", port);
	}
	printf("\n");
*/