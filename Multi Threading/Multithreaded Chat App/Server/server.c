#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9005
#define MAX 1024

struct info{
    int clientSocket;
    struct sockaddr_in clientAddr;
};

pthread_mutex_t lock;

void* chatLoop(void* args){
	char recvMsg[MAX];
	char sendMsg[MAX];
    struct info* data = args;

    printf("\033[1;32m");
    printf("Thread ID: %ld\n", pthread_self());
    printf("Client connected. Waiting for a message.......\n");
    printf("IP Address: %s\n", inet_ntoa(data->clientAddr.sin_addr));
    printf("Port No: %d\n\n", ntohs(data->clientAddr.sin_port));
    printf("\033[0m");

    while(1){
        pthread_mutex_lock(&lock);
        int n = recv(data->clientSocket, (char *)recvMsg, sizeof(recvMsg), 0);
        if(strncmp(recvMsg, "quit", 4) == 0){
            printf("\033[1;31m");
            printf("Client disconnected.\n");
            printf("IP Address: %s\n", inet_ntoa(data->clientAddr.sin_addr));
            printf("Port No: %d\n\n", ntohs(data->clientAddr.sin_port));
            printf("\033[0m");
            break;
        }
        recvMsg[n] = '\0';
        printf("\033[1;36m");
        printf("Client[%s, %d]: ", inet_ntoa(data->clientAddr.sin_addr), ntohs(data->clientAddr.sin_port));
        printf("\033[0m");
        printf("%s", recvMsg);
        printf("\033[1;36m");
        printf("Server[%s, %d]: ", inet_ntoa(data->clientAddr.sin_addr), ntohs(data->clientAddr.sin_port));
        printf("\033[0m");
        fgets(sendMsg, sizeof(sendMsg), stdin);
        send(data->clientSocket, (char *)sendMsg, sizeof(sendMsg), 0);
        printf("\n");
        pthread_mutex_unlock(&lock);
        sleep(1);
	}
    close(data->clientSocket);
    pthread_exit(NULL);
}

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr;
    struct info data[10];
    pthread_t tid[10];
    int x = 0, a = 0;
	
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\033[1;31m");
		printf("Socket could not be created.\n");
        printf("\033[0m");
		exit(EXIT_FAILURE);
	}
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);
	
	if(bind(serverSocket, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        printf("\033[1;31m");
		printf("Could not bind the server.\n");
        printf("\033[0m");
		exit(EXIT_FAILURE);
	}

    socklen_t len = sizeof(serverAddr);
    listen(serverSocket, 10);

    printf("Waiting for a client to connect......\n\n");

    if(pthread_mutex_init(&lock, NULL) != 0){
        printf("Mutex init has failed\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        if((data[a].clientSocket = accept(serverSocket, (struct sockaddr *)&data[a].clientAddr, &len)) < 0){
            printf("\033[1;31m");
            printf("Could not connect to the client.\n");
            printf("\033[0m");
            exit(EXIT_FAILURE);
        }
        pthread_create(&tid[x], NULL, chatLoop, &data[a]);
        a++;
        pthread_detach(tid[x]);
        x++;
    }

	close(serverSocket);
	return 0;
}

