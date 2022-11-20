#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <headers/common.h>
#include <string.h>
#include <pthread.h>

int choice = 0;

struct Client clients[20];
int sockets[20];
int client_counts = 0;
int sock_counts = 0;
int thread_count = 0;
pthread_t threads[200];
void handle_command(struct Message msg)
{
    write_console_log("message to handle ", 2, msg.command, msg.message);
}
void *handle_client(void *client_sock)
{
    int sock = *(int *)client_sock;
    char client_message[200] = {0};
    char message[100] = {0};
    do
    {
        memset(client_message, '\0', 200);
        memset(message, '\0', 100);
        if (recv(sock, client_message, 200, 0) < 0)
        {
            write_console_log("failed to receive message", 0);
            return NULL;
        }

        if (strcmp(client_message, "") != 0)
        {
            const char s[2] = "\n";
            char *token = strtok(client_message, s);
            short c = 0;
            struct Message msg;
            // while (token != NULL)
            // {
            //     if (c == 0)
            //     {
            //         strcpy(msg.command, token);
            //     }
            //     else if (c == 1)
            //     {
            //         strcpy(msg.from, token);
            //     }
            //     else if (c == 2)
            //     {
            //         strcpy(msg.to, token);
            //     }
            //     else if (c == 3)
            //     {
            //         strcpy(msg.message, token);
            //     }
            //     token = strtok(NULL, s);
            // }
            printf("\nclient_message   %s\n", client_message);
            // write_console_log("Command received from client: ", 2, client_message);
            //  handle_command(msg);
        }
    } while (strcmp(message, "bye\n") != 0 && strcmp(client_message, "") != 0);

    close(sock);
    // sleep(1);
    return NULL;
}

int main(int argc, char **argv)
{
    int sock_fd;
    int bind_val;
    struct sockaddr_in remote = {0};
    struct sockaddr_in client = {0};
    short sock;
    char client_message[200] = {0};
    char message[100] = {0};
    write_console_log("Starting the server....", 0);
    signal(SIGINT, exit_control);
    write_console_log("Creating socket...", 0);
    sock_fd = create_socket();
    write_console_log("Socket has been created successfully", 0);

    // bind process
    remote.sin_family = AF_INET;
    remote.sin_port = ntohs(PORT_NO);
    remote.sin_addr.s_addr = ntohl(INADDR_ANY);
    bind_val = bind_socket(&sock_fd, remote);
    write_console_log("Server is started at", 2, "address: localhost", "port: 12347");
    listen(sock_fd, 50);
    while (1)
    {
        write_console_log("Waiting for connection....", 0);
        int len = sizeof(struct sockaddr_in);
        sockets[sock_counts] = accept(sock_fd, (struct sockaddr *)&client, (socklen_t *)&len);
        if (sock < 0)
        {
            write_console_log("No able to accept the connection", 0);
            return 1;
        }

        write_console_log("connection accepted", 0);
        pthread_create(&threads[thread_count++], NULL, handle_client, (void *)&sockets[sock_counts]);
        sock_counts++;
    }

    return 0;
}
