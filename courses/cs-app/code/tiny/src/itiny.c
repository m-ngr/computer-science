#include "./lib/mcsapp.h"
#define ROOT_PATH "./dist"            /* HTTP Serve Root Path */
#define DEFAULT_FILE "index.html"

typedef struct{
  char method[100];         // HTTP Method
  char filename[MAXLINE];   // Resource Path
  char cgiargs[MAXLINE];    // CGI Arguments
  int is_dynamic;
  int content_length;
} request_header;

int verbose = 0;

// =========================================== Function prototype ====================================================

void sigchld_handler(int);
int check_method(const char*);
const char* http_status(int);
const char* mime_type(const char*);
const char* file_ext(const char*);
void serve(int);
int read_request_line(rio_t*, request_header*);
void read_request_headers(rio_t*, request_header*);
int write_response(int, request_header*);
void write_error(int, int, request_header*);
int serve_static(int, request_header*, int);
int serve_dynamic(int, request_header*);
void error_response(int, const char*, const char*, int, const char*);

// =============================================== Functions =========================================================

int main(int argc, char* argv[]){
  if(argc < 2) app_error(1, "usage: %s <port> <verbose?>\n", argv[0]);

  Signal(1, SIGCHLD, sigchld_handler);
  Signal(1, SIGPIPE, SIG_IGN);                /* Don't crash if client disconnected */

  const char* port = argv[1];
  if (argc > 2) verbose = atoi(argv[2]);

  int sock = Open_server(1, port);
  char client_host[MAXLINE], client_port[MAXLINE];
  struct sockaddr_storage client_addr;        /* Enough space for any address */
  socklen_t client_len;
  int flags = NI_NUMERICHOST | NI_NUMERICSERV;
  printf("Tiny Web Server is listening on port %s\n", port);
  
  while(1){
    client_len = sizeof(client_addr);
    int connfd = Accept(0, sock, (struct sockaddr*) &client_addr, &client_len);
    if(connfd == -1) continue;
    Getnameinfo(0, (struct sockaddr*) &client_addr, client_len,client_host, MAXLINE, client_port, MAXLINE, flags);
    
    if (verbose > 1) printf("======================================================\n");
    if (verbose) printf("client (%s:%s) connected\n", client_host, client_port);
    serve(connfd);
    Close(0, connfd);
    if (verbose) printf("client (%s:%s) disconnected\n", client_host, client_port);
  }

}

/* Reap childs when terminated */
void sigchld_handler(int sig){
  int olderrno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0);
  errno = olderrno;
}

/**
 * Return 1 if the HTTP method is Implemented, otherwise 0.
*/
int check_method(const char* method){
  if (streq(method, "GET", 0)) return 1;
  if (streq(method, "POST", 0)) return 1;
  if (streq(method, "HEAD", 0)) return 1;
  return 0;
}

/**
 * Returns the HTTP status message for the given HTTP status code.
*/
const char* http_status(int httpcode){
  switch (httpcode){
    case 200: return "OK";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    default: return "Unknown Error";
  }
}

/*
 * Return MIME type for the given file
 */
const char* mime_type(const char* filename) {
  const char* ext = file_ext(filename);
  if (streq(ext, "html", 0)) return "text/html";
  if (streq(ext, "css", 0)) return "text/css";
  if (streq(ext, "js", 0)) return "text/javascript";

  if (streq(ext, "pdf", 0)) return "application/pdf";

  if (streq(ext, "gif", 0)) return "image/gif";
  if (streq(ext, "png", 0)) return "image/png";
  if (streq(ext, "jpg", 0)) return "image/jpeg";
  if (streq(ext, "ico", 0)) return "image/vnd.microsoft.icon";

  if (streq(ext, "mpg", 0)) return "video/mpeg";
  if (streq(ext, "mpeg", 0)) return "video/mpeg";
  if (streq(ext, "mp4", 0)) return "video/mp4";

  return "text/plain";
}

/**
 * Return file extension from file name
*/
const char* file_ext(const char *filename) {
  const char* dot = strrchr(filename, '.');
  if(!dot || dot == filename) return "";
  return dot + 1;
}

/**
 * Handle one HTTP request/response transaction
 */
void serve(int sock){
  request_header reqhead;
  rio_t rio;

  riob_init(&rio, sock);

  if (read_request_line(&rio, &reqhead) != 0) return;
  read_request_headers(&rio, &reqhead);

  if (streq(reqhead.method, "POST", 0)){
    // Read cgi-args from content body (All headers must be read first)
    Riob_read(0, &rio, reqhead.cgiargs, reqhead.content_length);
    reqhead.cgiargs[reqhead.content_length] = '\0';
  }

  int httpcode = write_response(sock, &reqhead);
  if (httpcode != 200) write_error(sock, httpcode, &reqhead);
}

/**
 * read HTTP request line and parse it into method, filename, cgiargs, is_dynamic fields of reqhead
 * return 0 on success, -1 on error
*/
int read_request_line(rio_t* rp, request_header* reqhead){
  char reqline[MAXLINE], uri[MAXLINE], version[MAXLINE];

  if (Riob_readline(0, rp, reqline, MAXLINE) <= 0) return -1;
  sscanf(reqline, "%s %s %s", reqhead->method, uri, version);
  if (verbose) printf("Request: %s", reqline);

  // Parse URI: uri = filename?cgiargs or filename
  strcpy(reqhead->cgiargs, "");
  char* separator = index(uri, '?');
	if (separator) {
    strcpy(reqhead->cgiargs, separator+1);
	  *separator = '\0';
	}
  strcpy(reqhead->filename, ROOT_PATH);
  strcat(reqhead->filename, uri);
  if (uri[strlen(uri)-1] == '/') strcat(reqhead->filename, DEFAULT_FILE);
  reqhead->is_dynamic = strneq(uri, "/cgi-bin/", 9, 1);

  return 0;
}

/**
 * read HTTP request headers, print them to the screen
 * reads the value of Content-length header into reqhead.
 */
void read_request_headers(rio_t *rp, request_header* reqhead){
  char header[MAXLINE];
  reqhead->content_length = 0;
  if (verbose > 1) printf("Request Headers:\n");
  do {
    if (Riob_readline(0, rp, header, MAXLINE) <= 0) break;
	  if (verbose > 1) printf("\t%s", header);
    if (strneq(header, "Content-Length:", 15, 0)) {
      sscanf(header + 15, "%d", &(reqhead->content_length));
    }
  } while(strcmp(header, "\r\n"));
}

/**
 * writes a response to the client, returns the HTTP Status Code for the response
*/
int write_response(int sock, request_header* reqhead){
  struct stat sbuf;

  if (!check_method(reqhead->method)) return 501;      // check the HTTP method
  if (stat(reqhead->filename, &sbuf) < 0) return 404;  // check if file exists
  if (!S_ISREG(sbuf.st_mode)) return 403;              // check if regular file

  if (reqhead->is_dynamic) {
    if (!(S_IXUSR & sbuf.st_mode)) return 403;      // check the excute permission
    return serve_dynamic(sock, reqhead);
  } else {
    if (!(S_IRUSR & sbuf.st_mode)) return 403;      // check the read permission
    return serve_static(sock, reqhead, sbuf.st_size);
  }
}

/**
 * Send an error response to the client.
*/
void write_error(int sock, int httpcode, request_header* reqhead) {
  switch (httpcode) {
  case 200: return;
  case 403:
    return error_response(sock, reqhead->method, reqhead->filename, 403, "Tiny couldn't access the file");
  case 404:
    return error_response(sock, reqhead->method, reqhead->filename, 404, "Tiny couldn't find this file");
  case 500:
    return error_response(sock, reqhead->method, "Internal error", 500, "Tiny encountered an internal error");
  case 501:
    return error_response(sock, reqhead->method, reqhead->method, 501, "Tiny does not implement this method");
  default:
    return error_response(sock, reqhead->method, "Unknown error", httpcode,"Tiny encountered an unknown error");
  }
}

/**
 * copy a file back to the client
 * returns HTTP Status Code
 */
int serve_static(int sock, request_header* reqhead, int filesize){
  char header[MAXBUF];
  
  /* Send response headers to client */
  int left = sizeof(header);

  left -= snprintf(header, left, "HTTP/1.0 200 OK\r\n");    
  left -= strncatf(header, left, "Server: Tiny Web Server\r\n");
  left -= strncatf(header, left, "Connection: close\r\n");
  left -= strncatf(header, left, "Content-length: %d\r\n", filesize);
  left -= strncatf(header, left, "Content-type: %s\r\n\r\n", mime_type(reqhead->filename));

  if (Rio_write(0, sock, header, strlen(header)) < 0) return 500;
  if (verbose > 1) printf("Response headers:\n%s", header);

  if (streq(reqhead->method, "HEAD", 0)) return 200;

  /* Send response body to client */
  int srcfd = Open(0, reqhead->filename, O_RDONLY, 0);
  char* src_ptr = Mmap(0, 0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
  Close(0, srcfd);
  Rio_write(0, sock, src_ptr, filesize);
  Munmap(0, src_ptr, filesize);

  return 200;
}

/**
 * run a CGI program on behalf of the client
 * returns HTTP Status Code
 */
int serve_dynamic(int sock, request_header* reqhead){
  char *emptylist[] = { NULL };
  /* Return first part of HTTP response */
  if (Rio_writef(0, sock, 100, "HTTP/1.0 200 OK\r\n") < 0) return 500;
  Rio_writef(0, sock, 100, "Server: Tiny Web Server\r\n");
  
  if (Fork(0) == 0) { /* Child */
    Signal(1, SIGPIPE, SIG_IGN);               /* Don't crash if client disconnected */
    /* Real server would set all CGI vars here */
    setenv("QUERY_STRING", reqhead->cgiargs, 1);
    setenv("REQUEST_METHOD", reqhead->method, 1);  
    Dup2(1, sock, STDOUT_FILENO);              /* Redirect stdout to client */ 
    Execve(1, reqhead->filename, emptylist, environ);   /* Run CGI program */ 
  }

  return 200;
}

/**
 * write an error response to the client
 * doesn't write the response body if method is HEAD
 */
void error_response(int sock, const char *method, const char *cause, int httpcode, const char *msg){
  char body[MAXBUF];
  const char* httpmsg = http_status(httpcode);
  /* Build the HTTP response body */
  int left = sizeof(body);

  left -= snprintf(body, left, "<html><head><title>Tiny Error</title></head>");
  left -= strncatf(body, left, "<body bgcolor=""ffffff"">\r\n");
  left -= strncatf(body, left, "%d: %s\r\n", httpcode, httpmsg);
  left -= strncatf(body, left, "<p>%s: %s</p>\r\n", msg, cause);
  left -= strncatf(body, left, "<hr><em>The Tiny Web server</em></body></html>\r\n");

  /* Write the HTTP response */
  Rio_writef(0, sock, MAXLINE, "HTTP/1.0 %d %s\r\n", httpcode, httpmsg);
  Rio_writef(0, sock, 100, "Content-type: text/html\r\n");
  Rio_writef(0, sock, 100, "Content-length: %d\r\n\r\n", (int)strlen(body));
  if (streq(method, "HEAD", 0)) return;
  Rio_write(0, sock, body, strlen(body));
}
