#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "../headers/common.h"
void write_console_log(char *msg, const int size, ...)
{
    va_list args;
    time_t tm_ptr;
    time(&tm_ptr);
    va_start(args, size);

    struct tm *current = localtime(&tm_ptr);

    printf("<%d-%d-%d, %02d:%02d:%02d>: %s", current->tm_mday, current->tm_mon, (1900 + (current->tm_year)), current->tm_hour, current->tm_min, current->tm_sec, msg);
    for (int i = 0; i < size; i++)
    {
        char *ar = va_arg(args, char *);
        printf(" %s", ar);
        // strcat(msg, " ");
        // strcat(msg, ar);
    }
    printf("\n");
}

void exit_control(int sig)
{
    char c;
    printf("\nAre you sure?(Y/N): ");
    scanf("%c", &c);
    if (c == 'Y' || c == 'y')
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        signal(SIGINT, exit_control);
        // render_menu();
    }
}

char *get_ip(struct sockaddr_in clientfd)
{
    char *addr_string = {0};
    inet_ntop(AF_INET, &clientfd.sin_addr, addr_string, INET_ADDRSTRLEN);
    return addr_string;
}

int create_socket()
{
    short sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        write_console_log("Error in creating socket", 0);
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

int bind_socket(int *socketfd, struct sockaddr_in remote)
{
    short retval = 0;
    retval = bind(*socketfd, (struct sockaddr *)&remote, sizeof(remote));
    if (retval < 0)
    {
        write_console_log("Error in binding socket", 0);
        exit(EXIT_FAILURE);
    }
    return retval;
}

int connect_socket(int *socketfd, struct sockaddr_in remote)
{
    short retval = 0;
    retval = connect(*socketfd, (struct sockaddr *)&remote, sizeof(remote));
    if (retval < 0)
    {
        write_console_log("Error in connecting socket", 0);
        exit(EXIT_FAILURE);
    }
    return retval;
}