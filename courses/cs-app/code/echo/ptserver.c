#include "./lib/mcsapp.h"
#include "./lib/cisbuf.h"
#define CIS_SIZE 10       /* Client Info Shared buffer size */
#define N_THREADS 10      /* Default number of thread */

cisbuf_t clients;  /* clients shared buffer */

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

void* echo_worker(void* args){
  Pthread_detach(0, pthread_self());
  client_info info;
  char host[MAXLINE], port[MAXLINE];
  int flags = NI_NUMERICHOST | NI_NUMERICSERV;

  while(1){
    cisbuf_remove(&clients, &info);
    Getnameinfo(0, (struct sockaddr*) &(info.address), info.length,host, MAXLINE, port, MAXLINE, flags);
    printf("client (%s:%s) connected\n", host, port);
    echo(info.sock, host, port);
    Close(0, info.sock);
    printf("client (%s:%s) disconnected\n", host, port);
  }

  return NULL;
}

int main(int argc, char* argv[]){
  if(argc < 2) app_error(1, "usage: %s <port> <threads?>\n", argv[0]);
  Signal(1, SIGPIPE, SIG_IGN);                /* Don't crash if client disconnected */
  // parse arguments
  const char* port = argv[1];  
  int nthreads = (argc > 2)? atoi(argv[2]) : 0;
  if (nthreads < 1) nthreads = N_THREADS;
  // setup server
  int sock = Open_server(1, port);  
  cisbuf_init(&clients, CIS_SIZE);
  printf("Echo Server is listening on port %s\n", port);
  // run thread workers
  pthread_t tid;
  for(int i = 0; i < nthreads; ++i){
    Pthread_create(0, &tid, NULL, echo_worker, NULL);
  }
  // start accepting connections
  client_info info;
  while(1){
    info.length = sizeof(info.address);
    info.sock = Accept(0, sock, (struct sockaddr*) &(info.address), &(info.length));
    if(info.sock == -1) continue;
    cisbuf_insert(&clients, &info);
  }
  // optional step: destroy allocated memory and exit
  cisbuf_destroy(&clients);
  exit(0);
}
