/*******************************************************************************
 * SERVIDOR no porto 9000, à escuta de novos clientes.  Quando surgem
 * novos clientes os dados por eles enviados são lidos e descarregados no ecran.
 *******************************************************************************/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SERVER_PORT     9000
#define BUF_SIZE	1024

void process_client(struct sockaddr_in client_addr, int client_number, int client_fd);
void erro(char *msg);

int main() {
  int fd, client;
  struct sockaddr_in addr, client_addr;
  int client_addr_size;
  int client_number = 0;

  bzero((void *) &addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(SERVER_PORT);

  if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	erro("na funcao socket");
  if ( bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
	erro("na funcao bind");
  if( listen(fd, 5) < 0)
	erro("na funcao listen");
  client_addr_size = sizeof(client_addr);
  while (1) {
    //clean finished child processes, avoiding zombies
    //must use WNOHANG or would block whenever a child process was working
    while(waitpid(-1,NULL,WNOHANG)>0);
    //wait for new connection
    client = accept(fd,(struct sockaddr *)&client_addr,(socklen_t *)&client_addr_size);
    client_number ++;
    if (client > 0) {
      if (fork() == 0) {
        close(fd);
        process_client(client_addr, client_number, client);

        exit(0);
      }
    close(client);
    }
  }
  return 0;
}

void process_client(struct sockaddr_in client_addr, int client_number, int client_fd)
{
	int nread = 0;
	char buffer[BUF_SIZE], message[100];
  char client_ip_address[INET_ADDRSTRLEN];

  nread = read(client_fd, buffer, BUF_SIZE-1);
  buffer[nread] = '\0';
  printf("** New message received **\n");
  inet_ntop(AF_INET, &client_addr.sin_addr, client_ip_address, INET_ADDRSTRLEN);
  printf("Client %d connecting from (IP:port) %s:%d says \"%s\"\n", client_number, client_ip_address, client_addr.sin_port, buffer);
  snprintf(message, 100, "Server received connection from (IP:port) %s:%d; already received %d connections", client_ip_address, client_addr.sin_port, client_number);
  write(client_fd, message, 1 + strlen(message));
  fflush(stdout);
	close(client_fd);
}

void erro(char *msg){
	printf("Erro: %s\n", msg);
	exit(-1);
}
