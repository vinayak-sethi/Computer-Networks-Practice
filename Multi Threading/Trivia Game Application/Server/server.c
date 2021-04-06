#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

#define PORT 9015
#define MAX 1024

struct info{
	char name[30];
    int clientSocket;
    struct sockaddr_in clientAddr;
	float totalTime;
	bool done;
	bool qualified;
};

struct QA{
	char question[1024];
	char choice[4][30];
	int answer;
};
struct QA qa[8];
struct info Info[10];
int noOfClient = 0;
void initQA();
bool allDone();
bool didIWin(struct info* data);
void initInfo();

void* chatLoop(void* args){
	int recvMsg;
	char sendMsg[MAX];
	float qw;
	struct info* data = args;

	recv(data->clientSocket, data->name, sizeof(data->name), 0);

    printf("\033[1;32m");
    printf("Client connected.\nName: %s", data->name);
    printf("IP Address: %s\n", inet_ntoa(data->clientAddr.sin_addr));
    printf("Port No: %d\n\n", ntohs(data->clientAddr.sin_port));
    printf("\033[0m");

    for(int i = 0; i < 5; i++){
        send(data->clientSocket, &qa[i], sizeof(qa[i]), 0);

        recv(data->clientSocket, &recvMsg, sizeof(int), 0);

		if(recvMsg == -1){
			printf("\033[1;31m");
			data->totalTime = 600.0;
			printf("Timeout..!Time: \n");
			printf("Client disconnected.\nName: %s", data->name);
			printf("IP Address: %s\n", inet_ntoa(data->clientAddr.sin_addr));
			printf("Port No: %d\n\n", ntohs(data->clientAddr.sin_port));
			printf("\033[0m");
			data->done = true;
			data->qualified = false;
			close(data->clientSocket);
			return NULL;
		}

        if(recvMsg != qa[i].answer){
			send(data->clientSocket, "lost", strlen("lost"), 0);
			printf("\033[1;31m");
			data->totalTime = 600.0;
			printf("Wrong answer..!Time: \n");
			printf("Client disconnected.\nName: %s", data->name);
			printf("IP Address: %s\n", inet_ntoa(data->clientAddr.sin_addr));
			printf("Port No: %d\n\n", ntohs(data->clientAddr.sin_port));
			printf("\033[0m");
			data->done = true;
			data->qualified = false;
			close(data->clientSocket);
			return NULL;
		}
		else
			send(data->clientSocket, "stay", strlen("stay"), 0);
		sleep(2);
	}
	data->done = true;
	data->qualified = true;
	recv(data->clientSocket, &recvMsg, sizeof(int), 0);
	data->totalTime = recvMsg/5.0;
	while(allDone() == false);
	sleep(2);
	if(didIWin(data) == true){
		send(data->clientSocket, "win", sizeof("win"), 0);
		printf("\033[1;35m");
		printf("************************************WINNER************************************\n");
		printf("Name: %s", data->name);
		printf("Score: 50/50\n");
		printf("Average time taken per question: %g\n", data->totalTime);
		printf("IP Address: %s\n", inet_ntoa(data->clientAddr.sin_addr));
		printf("Port No: %d\n\n", ntohs(data->clientAddr.sin_port));
		printf("\033[0m");
	}
	else
		send(data->clientSocket, "lose", sizeof("lose"), 0);
	printf("\033[1;31m");
	printf("Client disconnected.\nName: %s", data->name);
	printf("IP Address: %s\n", inet_ntoa(data->clientAddr.sin_addr));
	printf("Port No: %d\n\n", ntohs(data->clientAddr.sin_port));
	printf("\033[0m");
	close(data->clientSocket);
}

int main() {
	time_t t;
	time(&t);
	printf("\033[1;33m");
	printf("%s\n", ctime(&t));
	printf("\033[0m");

	int serverSocket;
	struct sockaddr_in serverAddr;
    pthread_t tid[10];
    int x = 0, a = 0;
	initQA();
	initInfo();
	
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

    while(1){
        if((Info[a].clientSocket = accept(serverSocket, (struct sockaddr *)&Info[a].clientAddr, &len)) < 0){
            printf("\033[1;31m");
            printf("Could not connect to the client.\n");
            printf("\033[0m");
            exit(EXIT_FAILURE);
        }
		noOfClient++;
		pthread_create(&tid[x], NULL, chatLoop, &Info[a]);
		pthread_detach(tid[x]);
		a++;
		x++;
    }

	close(serverSocket);
	return 0;
}

void initQA(){
	strcpy(qa[0].question, "How are the players called in Valorant?");
	strcpy(qa[0].choice[0], "Players");
	strcpy(qa[0].choice[1], "Soldiers");
	strcpy(qa[0].choice[2], "Agents");
	strcpy(qa[0].choice[3], "Merchs");
	qa[0].answer = 3;
	strcpy(qa[1].question, "Which Valorant gun has the highest magazine capacity?");
	strcpy(qa[1].choice[0], "Frenzy");
	strcpy(qa[1].choice[1], "Ghost");
	strcpy(qa[1].choice[2], "Shorty");
	strcpy(qa[1].choice[3], "Sheriff");
	qa[1].answer = 2;
	strcpy(qa[2].question, "Whose ultimate ability lasts the longest?");
	strcpy(qa[2].choice[0], "Omen");
	strcpy(qa[2].choice[1], "Jett");
	strcpy(qa[2].choice[2], "Brimstone");
	strcpy(qa[2].choice[3], "Phoenix");
	qa[2].answer = 1;
	strcpy(qa[3].question, "Which class is Renya?");
	strcpy(qa[3].choice[0], "Initiator");
	strcpy(qa[3].choice[1], "Sentinel");
	strcpy(qa[3].choice[2], "Duelist");
	strcpy(qa[3].choice[3], "Controller");
	qa[3].answer = 3;
	strcpy(qa[4].question, "How many credits does the Operator cost?");
	strcpy(qa[4].choice[0], "4500");
	strcpy(qa[4].choice[1], "3600");
	strcpy(qa[4].choice[2], "3200");
	strcpy(qa[4].choice[3], "2900");
	qa[4].answer = 1;
	strcpy(qa[5].question, "Which rifle has the lowest damage value to the head?");
	strcpy(qa[5].choice[0], "Vandal");
	strcpy(qa[5].choice[1], "Guardian");
	strcpy(qa[5].choice[2], "Phantom");
	strcpy(qa[5].choice[3], "Bulldog");
	qa[5].answer = 4;
	strcpy(qa[6].question, "How much range does Sova’s Ultimate have?");
	strcpy(qa[6].choice[0], "Infinite");
	strcpy(qa[6].choice[1], "50m");
	strcpy(qa[6].choice[2], "24m");
	strcpy(qa[6].choice[3], "4m");
	qa[6].answer = 1;
	strcpy(qa[7].question, "What does Viper say when she uses her Ultimate ability?");
	strcpy(qa[7].choice[0], "I'll leave you breathless");
	strcpy(qa[7].choice[1], "Welcome to my zone");
	strcpy(qa[7].choice[2], "Don't get in my way");
	strcpy(qa[7].choice[3], "You destroyed my home");
	qa[7].answer = 3;
}

void initInfo(){
	for(int i = 0; i < 10; i++){
		Info[i].done = false;
		Info[i].qualified = false;
	}
}

bool allDone(){
	for(int i = 0; i < noOfClient; i++)
		if(Info[i].done == false)
			return false;
	return true;
}

bool didIWin(struct info* data){
	float x = 300;
	for(int i = 0; i < noOfClient; i++)
		if(Info[i].qualified == true)
			if(x > Info[i].totalTime)
				x = Info[i].totalTime;
	if(data->totalTime == x)
		return true;
	return false;
}
