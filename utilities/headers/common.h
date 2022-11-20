#ifndef COMMON_H
#define COMMON_H

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT_NO 12345 // server port number

// Clients structure
struct Client
{
    char name[25];
    int sockid;
};

// Message structure
struct Message
{
    char *command;
    char *from;
    char *to;
    char *message;
};

// Write to console with timestamp
void write_console_log(char *msg, const int size, ...);
// Exit control on intrupt SIGINT
void exit_control(int sig);

// returns  IP address of client as char *
char *get_ip(struct sockaddr_in clientfd);

// create socket
int create_socket();

// bind socket
int bind_socket(int *socketfd, struct sockaddr_in remote);

// connect to socket
int connect_socket(int *socketfd, struct sockaddr_in remote);

#endif