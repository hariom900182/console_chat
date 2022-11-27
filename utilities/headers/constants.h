#ifndef CONSTANTS_H
#define CONSTANTS_H

#define COMMAND_INIT "INIT" // Initializes the client client -> server
#define COMMAND_SNTO "SNTO" // Send message client -> server
#define COMMAND_RCFM "RCFM" // Receive message server -> client
#define COMMAND_RQLS "RQLS" // Request client's list client -> server
#define COMMAND_RACK "RACK" // Acknowlge client list request server -> client
#define COMMAND_SYST "SYST" // System command  server -> client
#define COMMAND_SYAC "SYAC" // System command  acknowlegement client -> server
#endif