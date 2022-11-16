#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <headers/common.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int sock_fd, sock_server_fd;
    struct sockaddr_in remote = {0};
    char client_message[200] = {0};
    char message[100] = {0};
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
    do
    {
        memset(client_message, '\0', sizeof client_message);
        memset(message, '\0', sizeof message);
        if (recv(sock_fd, client_message, 200, 0) < 0)
        {
            write_console_log("failed to receive message ok", 0);
            return 1;
        }
        write_console_log("Message from server: ", 1, client_message);
        printf("Message: ");
        fgets(message, 100, stdin);

        if (send(sock_fd, message, strlen(message), 0) < 0)
        {
            write_console_log("failed to send message", 0);
            return 1;
        }
    } while (strcmp(message, "bye\n") != 0);

    // if (recv(sock_fd, client_message, 200, 0) < 0)
    // {
    //     write_console_log("failed to receive message ok", 0);
    //     return 1;
    // }
    // write_console_log("Message from server: ", 1, client_message);
    // strcpy(message, "Hi!!!");
    // if (send(sock_fd, message, strlen(message), 0) < 0)
    // {
    //     write_console_log("failed to send message", 0);
    //     return 1;
    // }

    close(sock_fd);
    sleep(1);
    return 0;
}