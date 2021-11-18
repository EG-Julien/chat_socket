#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <errno.h>

#include "../libs/utils.h"

#define LENGTH 2048

// Global variables
volatile sig_atomic_t flag = 0;
int sockfd = 0;
char name[32];
pid_t server_pid = -1;

int flag_reply = 0;
char reply_username[30];

void catch_ctrl_c_and_exit(int sig) {
    flag = 1;
}

void send_msg_handler() {
    char message[LENGTH] = {};
	char buffer[LENGTH + 32] = {};
    char to[30] = { 0 };
    char msg[LENGTH] = { 0 };

    unsigned int is_command = 0;

    while(1) {
  	    str_overwrite_stdout();
        fgets(message, LENGTH, stdin);

        if (sscanf(message, "/msg %s %[^\n]", to, msg) > 0) {
            sprintf(buffer, "private_msg=%s %s", to, msg);
            stdout_purple();
            printf("Message privé à %s : %s\n", to, msg);
            send(sockfd, buffer, strlen(buffer), 0);
            is_command = 1;
        }

        if (sscanf(message, "/r %[^\n]", msg) > 0) {
            if (!flag_reply) {
                stdout_red();
                printf("Vous n'avez reçu aucun message pour le moment ...\n");
            } else {
                sprintf(buffer, "private_msg=%s %s", reply_username, msg);
                send(sockfd, buffer, strlen(buffer), 0);
            }
            is_command = 1;
        }

        if (message[0] != '\n') {
            str_trim_lf(message, LENGTH);

            if (strcmp(message, "/help") == 0) {
                is_command = 1;

                stdout_white();
                printf("=== CHAT SOCKET HELP MENU ===\n");
                printf(" - /exit -> permet de fermer le client.\n - /help -> permet d'afficher cette page d'aide.\n");
                printf(" - /stop -> permet d'envoyer un signal pour stopper le serveur.\n - /r -> permet de répondre à un message privé.\n");
            } 

            if (strcmp(message, "/exit") == 0) {
                server_pid = -1;
                break;
            } else if (strcmp(message, "/stop") == 0) {
                sprintf(buffer, "/stop");
                send(sockfd, buffer, strlen(buffer), 0);
                break;
            } else {
                if (!is_command) {
                    sprintf(buffer, "%s: %s\n", name, message);
                    if (send(sockfd, buffer, strlen(buffer), 0) == -1)
                        break;
                }
            }
        }

        is_command = 0;
		bzero(message, LENGTH);
        bzero(buffer, LENGTH + 32);
        bzero(to, 30);
        bzero(buffer, LENGTH);
        bzero(msg, LENGTH);
    }
    catch_ctrl_c_and_exit(2);
}

void recv_msg_handler() {
	char message[LENGTH] = {};
    char buffer[LENGTH]  = {}; 
    while (1) {
        int receive = recv(sockfd, message, LENGTH, 0);
        if (receive > 0) {

            time_t rawtime;
            struct tm * timeinfo;

            time ( &rawtime );
            timeinfo = localtime ( &rawtime );


            stdout_yellow();
            if (sscanf(message, "[MSG] - %s vous a envoye un message privé : %[^\n]", reply_username, buffer) > 0) {
                flag_reply = 1;
            }
            printf("[%d:%d:%d] %s", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);
            str_overwrite_stdout();
        } else if (receive == 0) {
            break;
        } else {
			// -1
	    }
	    memset(message, 0, sizeof(message));
    }
}

int main(int argc, char **argv){
	if(argc != 3){
        stdout_red();
		printf("Usage: %s <server_addr> <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *ip = argv[1];
	int port = atoi(argv[2]);

	signal(SIGINT, catch_ctrl_c_and_exit);

    stdout_white();
	printf("Please enter your name: ");

    fgets(name, 32, stdin);
    str_trim_lf(name, strlen(name));

	if (strlen(name) > 32 || strlen(name) < 2){
        stdout_red();
		printf("Name must be less than 30 and more than 2 characters.\n");
		return EXIT_FAILURE;
	}

    int try = 1;

    do {
        struct sockaddr_in server_addr;

        /* Socket settings */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(ip);
        server_addr.sin_port = htons(port);
        // Connect to Server
        int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (err == -1) {
            stdout_red();
            printf("ERROR: can't connect to the server, code : %d ... Trying to launch it ! Try %d/5 \n", errno, try);
            close(sockfd);
        } else {
            break;
        }

        server_pid = skeleton_daemon(argv[1]);

        if (server_pid == -1) {
            stdout_red();
            printf("ERROR: unable to start server ... retrying ... Try %d/5 \n", try);
        } else {
            stdout_green();
            printf("SUCCESS: server started successfully with pid %d\n", server_pid);
        }

        sleep(1);

        if (try >= 5)
            break;

        try++;
    } while(1);

    stdout_white();

    if (try >= 5) {
        stdout_red();
        printf("ERROR: Unable to start/connect server\n");
        return EXIT_FAILURE;
    }

    // Send name
    send(sockfd, name, 32, 0);

    stdout_green();
    printf("=== WELCOME TO THE CHATROOM ===\n");

    pthread_t send_msg_thread;

    if (pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
        stdout_red();
        printf("ERROR: pthread\n");
        return EXIT_FAILURE;
    }

    pthread_t recv_msg_thread;
    if (pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
        stdout_red();
        printf("ERROR: pthread\n");
        return EXIT_FAILURE;
    }

    while (1){
        if(flag){
            if (server_pid == -1) {
                break;
            } else {
                if (kill(server_pid, SIGINT) == -1) {
                    stdout_red();
                    printf("ERROR: unable to kill server with pid : %d ...\n", server_pid);
                    break;
                } else {
                    stdout_green();
                    printf("SUCCESS: server stoped.\n");
                }
            }
            break;
        }
    }

    stdout_red();
    printf("\nBye\n");

	close(sockfd);

	return EXIT_SUCCESS;
}