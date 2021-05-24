#ifndef SERVER_H
#define SERVER_H
#include <signal.h>
#include "global.h"
#include "file.h"
#include <stdlib.h>


#define IP_SERVER_PRIVATE "10.90.0.1"
#define MAX_CLIENTS 4

//* ------------------ SERVER_CONFIG ------------------

#define GROUPS_FILE "groups.bin"
void server_config(char *);

typedef struct group
{
    char group_name[20];
    char multicast_address[38];
    char port[10];
    user_t owner;
} group_t;

//* ------------------ SERVER_CONFIG ------------------

#define GROUPS_FILE "groups.bin"
#define MAX_GROUPS 16

#define GROUP_PORT 4000
#define P2P_PORT
void server_to_clients(char *port_clients);


char mult_address[16][INET_ADDRSTRLEN] = {
    "224.0.0.1",
    "224.0.0.2",
    "224.0.0.3",
    "224.0.0.4",
    "224.0.0.5",
    "224.0.0.6",
    "224.0.0.7",
    "224.0.0.8",
    "224.0.0.9",
    "224.0.0.10",
    "224.0.0.11",
    "224.0.0.12",
    "224.0.0.13",
    "224.0.0.14",
    "224.0.0.15",
    "224.0.0.16"};

#endif