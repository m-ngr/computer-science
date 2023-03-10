#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define MAXLINE 100
typedef struct addrinfo AI;

AI* get_info(const char* host){
  AI hints, *info;
  memset(&hints, 0, sizeof(AI));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_CANONNAME;

  int gai_code = getaddrinfo(host, NULL, &hints, &info);
  if (gai_code != 0){
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(gai_code));
    exit(-1);
  }

  return info;
}

void print_info(AI* info){
  char ip[MAXLINE];
  int flags = NI_NUMERICHOST;
  int gni_code = getnameinfo(info->ai_addr, info->ai_addrlen, ip, MAXLINE, NULL, 0, flags);

  if (gni_code != 0){
    fprintf(stderr, "getnameinfo error: %s\n", gai_strerror(gni_code));
    exit(-1);
  }

  if (info->ai_canonname) printf("Canonical Name: %s\n", info->ai_canonname);
  printf("IP Address: %s\n", ip);
}

int main(int argc, char* argv[]){
  if (argc < 2){
    fprintf(stderr, "usage: %s <hostname>\n", argv[0]);
    exit(-1);
  }

  const char* host = argv[1];
  AI* info = get_info(host);
  for(AI* p = info; p; p = p->ai_next) print_info(p);
  freeaddrinfo(info);
  return 0;
}
