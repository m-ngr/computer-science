#include "mcsapp.h"

void echo(int sock, const char* host, const char* port){
  rio_t rio;
  char buf[MAXLINE];
  int bytes = 0;
  riob_init(&rio, sock);

  while ((bytes = Riob_readline(0, &rio, buf, MAXLINE)) > 0){
    printf("Server received %d bytes from (%s:%s)\n", bytes, host, port);
    Rio_write(0, sock, buf, bytes);
  }
}

int main(int argc, char* argv[]){
  if(argc != 2) app_error(1, "usage: %s <port>\n", argv[0]);
  Signal(1, SIGPIPE, SIG_IGN);                /* Don't crash if client disconnected */

  const char* port = argv[1];
  int sock = Open_server(1, port);
  char client_host[MAXLINE], client_port[MAXLINE];
  struct sockaddr_storage client_addr;        /* Enough space for any address */
  socklen_t client_len;
  int flags = NI_NUMERICHOST | NI_NUMERICSERV;
  printf("Echo Server is listening on port %s\n", port);

  while(1){
    client_len = sizeof(client_addr);
    int connfd = Accept(0, sock, (struct sockaddr*) &client_addr, &client_len);
    if(connfd == -1) continue;

    Getnameinfo(0, (struct sockaddr*) &client_addr, client_len,client_host, MAXLINE, client_port, MAXLINE, flags);
    printf("client (%s:%s) connected\n", client_host, client_port);
    echo(connfd, client_host, client_port);
    Close(0, connfd);
    printf("client (%s:%s) disconnected\n", client_host, client_port);
  }
}
