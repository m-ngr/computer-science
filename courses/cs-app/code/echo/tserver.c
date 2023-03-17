#include "./lib/mcsapp.h"
#include "./lib/cisbuf.h"

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

void* echo_thread (void* arg){
  Pthread_detach(0, pthread_self());
  client_info* info = arg;
  char host[MAXLINE], port[MAXLINE];
  int flags = NI_NUMERICHOST | NI_NUMERICSERV;
  Getnameinfo(0, (struct sockaddr*) &(info->address), info->length,host, MAXLINE, port, MAXLINE, flags);
  printf("client (%s:%s) connected\n", host, port);
  echo(info->sock, host, port);
  Close(0, info->sock);
  printf("client (%s:%s) disconnected\n", host, port);
  free(info);
  return NULL;
}

int main(int argc, char* argv[]){
  if(argc != 2) app_error(1, "usage: %s <port>\n", argv[0]);
  Signal(1, SIGPIPE, SIG_IGN);                /* Don't crash if client disconnected */

  const char* port = argv[1];
  int sock = Open_server(1, port);  
  printf("Echo Server is listening on port %s\n", port);

  while(1){
    client_info* info = Malloc(0, sizeof(client_info));
    info->length = sizeof(info->address);
    info->sock = Accept(0, sock, (struct sockaddr*) &(info->address), &(info->length));
    if(info->sock == -1) {
      free(info);
      continue;
    }
    pthread_t tid;
    Pthread_create(0, &tid, NULL, echo_thread, info);
  }
}
