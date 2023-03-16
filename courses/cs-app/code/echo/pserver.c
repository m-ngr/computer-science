#include "./lib/mcsapp.h"

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

void sigchld_handler(int sig){
  int olderrno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0);
  errno = olderrno;
}

int main(int argc, char* argv[]){
  if(argc != 2) app_error(1, "usage: %s <port>\n", argv[0]);
  Signal(1, SIGPIPE, SIG_IGN);                /* Don't crash if client disconnected */
  Signal(1, SIGCHLD, sigchld_handler);

  const char* port = argv[1];
  int sock = Open_server(1, port);
  struct sockaddr_storage client_addr;        /* Enough space for any address */
  socklen_t client_len;
  printf("Echo Server is listening on port %s\n", port);

  while(1){
    client_len = sizeof(client_addr);
    int connfd = Accept(0, sock, (struct sockaddr*) &client_addr, &client_len);
    if(connfd == -1) continue;

    if (Fork(0) == 0){ // Child
      Close(0, sock);
      char client_host[MAXLINE], client_port[MAXLINE];
      int flags = NI_NUMERICHOST | NI_NUMERICSERV;
      Getnameinfo(0, (struct sockaddr*) &client_addr, client_len,client_host, MAXLINE, client_port, MAXLINE, flags);
      printf("client (%s:%s) connected\n", client_host, client_port);
      echo(connfd, client_host, client_port);
      Close(0, connfd);
      printf("client (%s:%s) disconnected\n", client_host, client_port);
      exit(0);
    }
    
    Close(0, connfd);
  }
}
