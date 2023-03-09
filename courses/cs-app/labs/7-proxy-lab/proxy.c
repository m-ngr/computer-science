#include "csapp.h"
#include "cache.h"

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

typedef struct{
  char method[100];         // HTTP Method
  char host[MAXLINE];       // Server Host
  char port[10];            // Server Port
  char uri[MAXLINE];        // Resource
  char headers[MAXLINE];    // HTTP Request Headers
} http_request;

cache_t cache;            /* Shared Cache */
int verbose = 0;          /* Verbosity level: 0 , 1 , 2 */

// =========================================== Function prototype ====================================================

void serve(int);
void get_response(int client, http_request* reqhead, cache_object*);
int read_request_line(rio_t*, http_request*);
void read_request_headers(rio_t*, http_request*);
void* proxy_thread (void*);

// =============================================== Functions =========================================================

int main(int argc, char* argv[]){
  if(argc < 2) app_error(1, "usage: %s <port> <verbose>\nverbose = v0 , v1, v2", argv[0]);
  Signal(1, SIGPIPE, SIG_IGN);                /* Don't crash if client disconnected */

  if (argc == 3) sscanf(argv[2], "v%d", &verbose);
  const char* port = argv[1];
  int sock = Open_server(1, port);
  cache_init(&cache);
  printf("Proxy Web Server is listening on port %s\n", port);
  
  while(1) {
    int* connfd = malloc(sizeof(int));
    if (connfd == NULL) continue;
    *connfd = Accept(0, sock, NULL, NULL);
    if(*connfd == -1) continue;
    pthread_t tid;
    Pthread_create(0, &tid, NULL, proxy_thread, connfd);
  }

}

/** Proxy Thread Main */
void* proxy_thread (void* args){
  pthread_detach(pthread_self());
  int connfd = *((int*) args);
  free(args);
  serve(connfd);
  Close(0, connfd);
  return NULL;
}

/**
 * Handle one client
 */
void serve(int sock){
  rio_t rio;
  http_request reqhead;
  cache_object object;
  riob_init(&rio, sock);
  if (verbose > 1) printf("======================================================\n");
  if (read_request_line(&rio, &reqhead) != 0) return;
  if (!streq(reqhead.method, "GET", 0)) {
    if (verbose > 1) printf("Proxy doesn't handle %s Method\n", reqhead.method);
    return;
  }

  cache_setkey(&object, "%s:%s%s", reqhead.host, reqhead.port, reqhead.uri);
  if (cache_get(&cache, &object)) {
    Rio_write(0, sock, object.data, object.size);
    if (verbose) printf("Content served from proxy cache\n");
    return;
  }

  read_request_headers(&rio, &reqhead);

  if (verbose) printf("Sending request to (%s:%s)\n", reqhead.host, reqhead.port);
  object.size = 0;
  get_response(sock, &reqhead, &object);
}

/**
 * read HTTP request line and parse it into method, host, port, uri fields of reqhead
 * return 0 on success, -1 on error
*/
int read_request_line(rio_t* rp, http_request* reqhead){
  char reqline[MAXLINE], url[MAXLINE];
  // reqline: method url version
  if (Riob_readline(0, rp, reqline, MAXLINE) <= 0) return -1;
  sscanf(reqline, "%s %s", reqhead->method, url);
  if (verbose > 1) printf("Request: %s", reqline);

  /* Parse URL: hostname:port/uri */
  // skip (http://) part of the URL
  char* url_start = strstr(url, "//");
  if (url_start) url_start += 2; else url_start = url;
  // get the uri
  char* separator = index(url_start, '/');
  strcpy(reqhead->uri, separator? separator:"/");
  if (separator) *separator = '\0';
  // get the port
  separator = index(url_start, ':');
  strcpy(reqhead->port, separator? (separator+1) : "80");
  if (separator) *separator = '\0';
  // get hostname
  strcpy(reqhead->host, url_start);

  return 0;
}

/**
 * read HTTP request headers into reqhead
 */
void read_request_headers(rio_t *rp, http_request* reqhead){
  char header[MAXLINE];
  int host = 0, agent = 0;
  strcpy(reqhead->headers, "");
  if (verbose > 1) printf("Request Headers:\n");

  while(Riob_readline(0, rp, header, MAXLINE) > 0) {
    if (verbose > 1) printf("%s", header);
    if (streq(header, "\r\n", 1)) break;

    if (strneq(header, "Host:", 5, 0)) host = 1;
    else if (strneq(header, "User-Agent:", 11, 0)) agent = 1;
    else if (strneq(header, "Connection:", 11, 0)) continue;
    else if (strneq(header, "Proxy-Connection:", 17, 0)) continue;

    strcat(reqhead->headers, header);
  }

  if (!host) strncatf(reqhead->headers, 0, "Host: %s\r\n", reqhead->host);
  if (!agent) strcat(reqhead->headers, user_agent_hdr);
  strcat(reqhead->headers, "Connection: close\r\n");
  strcat(reqhead->headers, "Proxy-Connection: close\r\n\r\n");
}

/**
 * Get Response from server to client
 */
void get_response(int client, http_request* reqhead, cache_object* object){
  int server = Open_client(0, reqhead->host, reqhead->port);
  if (server < 0) return;
  // send request to the Server
  Rio_writef(0, server, MAXLINE, "%s %s HTTP/1.0\r\n", reqhead->method, reqhead->uri);
  Rio_write(0, server, reqhead->headers, sizeof(reqhead->headers));
  // get response from the server to client
  char buff[MAXLINE];
  int n;
  while ((n = read(server, buff, MAXLINE))){
    if (n < 0){
      if (errno == EINTR) continue; else break;
    }else if (n == 0) break;

    Rio_write(0, client, buff, n);
    cache_write(object, buff, n);
  }

  Close(0, server);
  cache_set(&cache, object);
}
