#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <headers/common.h>
#include <string.h>
int choice = 0;

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
    listen(sock_fd, 3);
    while (1)
    {
        write_console_log("Waiting for connection....", 0);
        int len = sizeof(struct sockaddr_in);
        sock = accept(sock_fd, (struct sockaddr *)&client, (socklen_t *)&len);
        if (sock < 0)
        {
            write_console_log("No able to accept the connection", 0);
            return 1;
        }

        write_console_log("connection accepted", 0);
        memset(client_message, '\0', sizeof client_message);
        memset(message, '\0', sizeof message);
        do
        {
            memset(client_message, '\0', sizeof client_message);
            memset(message, '\0', sizeof message);
            printf("Message: ");
            // scanf("%s", message);
            fgets(message, 100, stdin);
            if (send(sock, message, strlen(message), 0) < 0)
            {
                write_console_log("failed to send message", 0);
                return 1;
            }
            if (recv(sock, client_message, 200, 0) < 0)
            {
                write_console_log("failed to receive message", 0);
                return 1;
            }
            write_console_log("Message from client: ", 1, client_message);
        } while (strcmp(message, "bye\n") != 0);
        // strcpy(message, "Hello guest");
        //  if (send(sock, message, strlen(message), 0) < 0)
        //  {
        //      write_console_log("failed to send message", 0);
        //      return 1;
        //  }
        //  if (recv(sock, client_message, 200, 0) < 0)
        //  {
        //      write_console_log("failed to receive message", 0);
        //      return 1;
        //  }
        //  write_console_log("Message from client: ", 1, client_message);

        close(sock);
        sleep(1);
    }

    // sleep(1);
    // render_menu();

    return 0;
}
