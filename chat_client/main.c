#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <headers/common.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/signal.h>

int sock_fd, sock_server_fd;
char client_message[200] = {0};
char message[100] = {0};
char name[50];
char sock_str[25];
void handle_sigint(int sig);
void render_menu();
void receive_mode();

int main()
{

    struct sockaddr_in remote = {0};
    printf("*******************PROCESS(%d)*****************\n", getpid());
    printf("Enter your name: ");
    fgets(name, 20, stdin);
    signal(SIGINT, handle_sigint);
    write_console_log("Creating socket...", 0);
    sock_fd = create_socket();
    write_console_log("Socket creation done", 0);
    remote.sin_port = ntohs(PORT_NO);
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = inet_addr("127.0.0.1");
    write_console_log("Connecting to server...", 0);
    sock_server_fd = connect_socket(&sock_fd, remote);
    write_console_log("Connection done", 0);
    memset(client_message, '\0', sizeof client_message);
    memset(message, '\0', sizeof message);
    // send name to server
    sprintf(sock_str, "%d", sock_fd);
    strcpy(message, "init#");
    strcat(message, sock_str);
    strcat(message, "#to#");
    // strcpy(message, "init#from#to#");
    strcat(message, name);
    printf("Message to send: %s\n", message);
    if (send(sock_fd, message, strlen(message), 0) < 0)
    {
        write_console_log("failed to send message", 0);
        return 1;
    }
    do
    {

        memset(client_message, '\0', sizeof client_message);
        memset(message, '\0', sizeof message);
        if (recv(sock_fd, client_message, 200, 0) < 0)
        {
            write_console_log("failed to receive message ok", 0);
            return 1;
        }
        printf("client msg: %s", client_message);

    } while (strcmp(client_message, "bye\n") != 0);
    //  close(sock_fd);
    //  sleep(1);
    return 0;
}

void render_menu()
{
    system("clear");
    // signal(SIGINT, handle_sigint);
    char message[100];
    int choice = 0;
    do
    {
        printf("###########################################################\n");
        printf("# 1. Press 1 for exit.                                    #\n");
        printf("# 2. Press 2 for send message to selected user.           #\n");
        printf("# 3. Press 3 for go to message receive mode               #\n");
        printf("###########################################################\n");
        printf(":> ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            close(sock_fd);
            exit(EXIT_SUCCESS);
            break;
        case 2:

            memset(message, '\0', 100);

            printf("Enter message to send: ");

            fgets(message, 100, stdin);
            while ((getchar()) != '\n')
                ;
            char msgptr[100];
            strcpy(msgptr, "sendto#from#to#");
            strcat(msgptr, message);
            printf("Message to send: %s\n", msgptr);
            if (send(sock_fd, msgptr, strlen(msgptr), 0) < 0)
            {
                write_console_log("failed to send message", 0);
                return;
            }

            // receive_mode();
            //  choice = 0;
            break;
        case 3:
            // signal(SIGINT, handle_sigint);
            receive_mode();
            break;
        default:
            break;
        }
    } while (choice < 1 || choice > 3);
}

void handle_sigint(int sig)
{
    // printf("Signal handled and re added");
    if (sig == SIGINT)
        signal(SIGINT, handle_sigint);
    render_menu();
}

void receive_mode()
{
    system("clean");
    printf("\n***Message receive mode***\n");
}