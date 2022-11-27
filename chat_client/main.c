#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <headers/common.h>
#include <headers/constants.h>
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
char selected_user[20];
short is_receive_mode = 1;
char msg_buffer[100][200] = {0};
int buffer_count = 0;
void handle_sigint(int sig);
void render_menu();
void receive_mode();
void handle_server_response(char *msg);
void output_msg(char *msg);

int main()
{

    struct sockaddr_in remote = {0};
    printf("*******************PROCESS(%d)*****************\n", getpid());
    printf("Enter your name: ");
    fgets(name, 20, stdin);
    name[strlen(name) - 1] = '\0';
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
    strcpy(message, COMMAND_INIT);
    strcat(message, "#");
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
    printf("\n***Message receive mode***\n");
    do
    {

        memset(client_message, '\0', sizeof client_message);
        memset(message, '\0', sizeof message);
        if (recv(sock_fd, client_message, 200, 0) < 0)
        {
            write_console_log("failed to receive message ok", 0);
            return 1;
        }
        // printf("%s", client_message);
        handle_server_response(client_message);

    } while (strcmp(client_message, "") != 0);
    // close(sock_fd);
    //   sleep(1);
    return 0;
}

void render_menu()
{
    system("clear");
    // signal(SIGINT, handle_sigint);
    char message[100];
    int choice = 0;
    is_receive_mode = 0;
    do
    {
        printf("###########################################################\n");
        printf("# 1. Press 1 for exit.                                    #\n");
        printf("# 2. Press 2 for send message to selected user.           #\n");
        printf("# 3. Press 3 for go to message receive mode               #\n");
        printf("# 4. Press 4 for list of all connected clients            #\n");
        printf("###########################################################\n");
        printf(":%s> ", selected_user);
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            close(sock_fd);
            exit(EXIT_SUCCESS);
            break;
        case 2:
            if (strlen(selected_user) > 0)
            {
                memset(message, '\0', 100);

                printf("Enter message to send: ");
                while ((getchar()) != '\n')
                    ;
                fgets(message, 100, stdin);

                char msgptr[100];
                strcpy(msgptr, COMMAND_SNTO);
                strcat(msgptr, "#");
                strcat(msgptr, name);
                strcat(msgptr, "#");
                strcat(msgptr, selected_user);
                strcat(msgptr, "#");
                strcat(msgptr, message);

                if (send(sock_fd, msgptr, strlen(msgptr), 0) < 0)
                {
                    write_console_log("failed to send message", 0);
                    return;
                }
                receive_mode();
            }
            else
            {
                write_console_log("Please select user first then try again", 0);
                choice = 0;
            }

            //  choice = 0;
            break;
        case 3:
            // signal(SIGINT, handle_sigint);
            receive_mode();
            break;
        case 4:
            memset(message, '\0', 100);
            sprintf(sock_str, "%d", sock_fd);
            strcpy(message, COMMAND_RQLS);
            strcat(message, "#");
            strcat(message, sock_str);
            strcat(message, "#to#");
            // strcpy(message, "init#from#to#");
            strcat(message, "");
            if (send(sock_fd, message, strlen(message), 0) < 0)
            {
                write_console_log("failed to send message", 0);
                return;
            }
            receive_mode();
            break;
        default:
            break;
        }
    } while (choice < 1 || choice > 4);
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
    system("clear");
    is_receive_mode = 1;
    printf("\n***Message receive mode***\n");
    if (buffer_count > 0)
    {
        printf("\n-------------UNREAD MESSGES---------------\n");
        for (int i = 0; i < buffer_count; i++)
        {
            output_msg(msg_buffer[i]);
        }
        printf("\n-------------UNREAD MESSGES END---------------\n");
        buffer_count = 0;
    }
}
void handle_server_response(char *msg)
{
    if (is_receive_mode == 0)
    {
        strcpy(msg_buffer[buffer_count++], msg);
    }
    else
    {
        output_msg(client_message);
    }
}

void output_msg(char *msg)
{
    char s[2] = "#";
    char *token = strtok(msg, s);
    int c = 0;
    char cmd[5];
    char cmsg[200];
    memset(cmd, '\0', 5);
    memset(cmsg, '\0', 200);
    while (token != NULL)
    {
        if (c == 0)
        {
            strcpy(cmd, token);
        }
        else
        {
            strcpy(cmsg, token);
        }
        c++;
        token = strtok(NULL, s);
    }
    printf("%s", cmsg);
    if (strcmp(cmd, COMMAND_RACK) == 0)
    {
        char cU;
        clear_stream();
        printf("Do you want to select user(Y/N): ");
        scanf("%c", &cU);
        if (cU == 'Y' || cU == 'y')
        {
            printf("Enter name: ");
            while ((getchar()) != '\n')
                ;
            fgets(selected_user, 20, stdin);
            selected_user[strlen(selected_user) - 1] = '\0';
        }
        receive_mode();
    }
    //
}