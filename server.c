/*
porto UDP: 3200
porto TCP: 3100
*/

#include "header.h"
int config;

void server_config();
void server_to_clients();
void process_config(int fd_config, struct sockaddr_in config_addr);
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("server <client port> <config port> <register file>");
        exit(-1);
    }
    pid_t config_id, clients_id;

    if ((config_id = fork()) == 0)
    {
        server_config(argv[2]);
        exit(0);
    }
    if ((clients_id = fork()) == 0)
    {
        server_to_clients(argv[1]);
        exit(0);
    }
}
//para tratar da administracao do servidor
void server_config(char *port_config)
{
    /*************************TCP****************************/
    struct sockaddr_in addr_server_config, config_addr;
    char *endServer;
    int config_addr_size = sizeof(config_addr);
    strcpy(endServer, IP_SERVER);
    int fd_config_server, config;
    struct hostent *hostPtr;

    if ((hostPtr = gethostbyname(endServer)) == 0)
        erro("Nao consegui obter endereço");

    bzero((void *)&addr_server_config, sizeof(addr_server_config));

    addr_server_config.sin_family = AF_INET;
    addr_server_config.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    addr_server_config.sin_port = htons((short)atoi(port_config));

    if ((fd_config_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("socket");

    if (bind(fd_config_server, (struct sockaddr *)&addr_server_config, sizeof(addr_server_config)) < 0)
        error("bind");

    if (listen(fd_config_server, 5) < 0)
        error("listen");

    //clean finished child processes, avoiding zombies
    //must use WNOHANG or would block whenever a child process was working
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
    //wait for new connection
    config = accept(fd_config_server, (struct sockaddr *)&config_addr, (socklen_t *)&config_addr_size);
    if (config > 0)
    {
        process_config(config, (struct sockaddr_in)config_addr);
        close(config);
        exit(0);
    }
    close(fd_config_server);
}
void process_config(int fd_config, struct sockaddr_in config_addr)
{
    int nread = 0;
    char command[MESSAGE_LEN];
    char send[MESSAGE_LEN];
    int go = TRUE;

    do
    {

        nread = read(fd_config, command, MESSAGE_LEN - 1);
        command[nread] = '\0';
        if (!strcmp(command, "QUIT"))
            go = FALSE;
        else if (!strcmp(command, "LIST"))
        {
            printf("list users\n");
        }
        else
        {
            char *token;
            const char delim[2] = " ";
            token = strtok(command, delim);
            if (!strcmp(token, "ADD"))
            {
                char *userId;
                char *IP;
                char *password;
                char *client_server, p2p, group;
                token = strtok(NULL, delim);
                strcpy(userId, token);
                token = strtok(NULL, delim);
                strcpy(IP, token);
                token = strtok(NULL, delim);
                strcpy(password, token);
                token = strtok(NULL, delim);
                strcpy(client_server, token);
                token = strtok(NULL, delim);
                strcpy(p2p, token);
                token = strtok(NULL, delim);
                strcpy(group, token);
            }
            else if (!strcmp(token, "DEL"))
            {
                char *userId;
                token = strtok(NULL, delim);
                strcpy(userId, token);
            }
            else
            {
                //invalid command
            }
        }

        write(fd_config, send, 1+strlen(send));
    } while (go = TRUE && nread > 0);
}
//para tratar dos clientes
void server_to_clients(char *port_clients)
{
    /*************************UDP****************************/
    printf("Server to clients [%d]\n", getpid());
}

char *input_menu()
{
    return "Choose a type of communication:\n1 - Client/Server\n2 - P2P\n3 - Group\n4 - Exit";
}