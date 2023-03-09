/*
 * adder.c - a minimal CGI program that adds two numbers together
 */

#include "./csapp.h"

int main(void) {
  char *buf, *p, *method;
  char content[MAXLINE];
  int n1=0, n2=0;
  
  /* Extract the two arguments */
  if ((buf = getenv("QUERY_STRING")) != NULL) {
    p = strchr(buf, '&');
    *p = '\0';
    sscanf(buf, "n1=%d", &n1);
    sscanf(p+1, "n2=%d", &n2);
  }

  method = getenv("REQUEST_METHOD");  
  /* Make the response body */
  int left = sizeof(content);
  left -= snprintf(content, left, "<html><head><title>Adder</title></head><body>");
  left -= strncatf(content, left, "<h2>The answer is: %d + %d = %d</h2>\r\n", n1, n2, n1 + n2);
  left -= strncatf(content, left, "</body></html>\r\n");
  
  /* Generate the HTTP response */
  printf("Connection: close\r\n");
  printf("Content-length: %d\r\n", (int)strlen(content));
  printf("Content-type: text/html\r\n\r\n");
  if(!streq(method, "HEAD", 0)) printf("%s", content);
  fflush(stdout);
  exit(0);
}
