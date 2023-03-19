#include "./lib/mcsapp.h"
#include "./lib/cisbuf.h"
#include "./lib/dtc.h"
#define CIS_SIZE 10           /* Client Info Shared buffer size */
#define MIN_THREADS 10        /* Minimum number of concurrent threads */
#define MAX_THREADS 10000     /* Maximum number of concurrent threads */
#define ADD_THREADS 10        /* Number of threads to add */

cisbuf_t clients;   /* clients shared buffer */
dtc_t dtcontroller; /* dynamic threads controller */

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
    dtc_work(&dtcontroller);
    Getnameinfo(0, (struct sockaddr*) &(info.address), info.length,host, MAXLINE, port, MAXLINE, flags);
    printf("client (%s:%s) connected\n", host, port);
    echo(info.sock, host, port);
    Close(0, info.sock);
    printf("client (%s:%s) disconnected\n", host, port);
    dtc_rest(&dtcontroller);
  }

  return NULL;
}

int main(int argc, char* argv[]){
  if(argc < 2) {
    app_error(1, "usage: %s <port> <min-threads?> <max-threads?> <add-threads?>\n", argv[0]);
  }
  Signal(1, SIGPIPE, SIG_IGN);                /* Don't crash if client disconnected */
  // parse arguments
  const char* port = argv[1];  
  int min_threads = (argc > 2)? atoi(argv[2]) : MIN_THREADS;
  if (min_threads < 1) min_threads = MIN_THREADS;

  int max_threads = (argc > 3)? atoi(argv[3]) : MAX_THREADS;
  if (max_threads < min_threads) max_threads = MAX_THREADS;

  int add_threads = (argc > 4)? atoi(argv[4]) : ADD_THREADS;
  if (add_threads < 1) add_threads = ADD_THREADS;

  // setup server
  int sock = Open_server(1, port);  
  cisbuf_init(&clients, CIS_SIZE);
  dtc_init(&dtcontroller, echo_worker, min_threads, max_threads, add_threads);
  printf("Echo Server is listening on port %s\n", port);
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
