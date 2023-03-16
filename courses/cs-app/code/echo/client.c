#include "./lib/mcsapp.h"

int echo(int sock){
  rio_t rio;
  char buf[MAXLINE];
  riob_init(&rio, sock);

  while (fgets(buf, MAXLINE, stdin) != NULL) {
    Rio_write(1, sock, buf, strlen(buf));
    if (Riob_readline(1, &rio, buf, sizeof(buf)) == 0) return -1;
    fputs(buf, stdout);
  }

  return 0;
}

int main(int argc, char* argv[]){
  if(argc != 3) app_error(1, "usage: %s <host> <port>\n", argv[0]);
  Signal(1, SIGPIPE, SIG_IGN);                /* Don't crash if client disconnected */

  const char* host = argv[1];
  const char* port = argv[2];
  int sock = Open_client(1, host, port);

  if (echo(sock) != 0) {
    fprintf(stderr, "Error: connection closed by the server\n");
  }

  close(sock);
}
