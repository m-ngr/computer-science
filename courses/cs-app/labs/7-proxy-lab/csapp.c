/* 
 * csapp.c - Functions for the CS:APP3e book (Modified)
 */

#include "csapp.h"

/************************** 
 * Error-handling functions
 **************************/

/**
 * Unix-style error - print formatted string and errno message
 * returns errno if quit = 0, otherwise exits with exit code = errno
*/
int unix_error(int quit, const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, ": %s\n", strerror(errno));
  if (quit) exit(errno);
  return errno;
}

/**
 * Posix-style error - print formatted string and error message
 * returns if quit = 0, otherwise exits with exit code = code
*/
void posix_error(int quit, int code, const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, ": %s\n", strerror(code));
  if (quit) exit(code);
}

/**
 * Getaddrinfo-style error - print formatted string and gai error message
 * returns if quit = 0, otherwise exits with exit code = code
*/
void gai_error(int quit, int code, const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, ": %s\n", gai_strerror(code));
  if (quit) exit(code);
}

/**
 * Application error - print formatted string
 * returns if quit = 0, otherwise exits with exit code = quit
*/
void app_error(int quit, const char* format, ...){
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  if (quit) exit(quit);
}

/*********************************************
 * Wrappers for Unix process control functions
 ********************************************/

pid_t Fork(int quit) {
  pid_t pid = fork();
  if (pid < 0) unix_error(quit, "Fork error");
  return pid;
}

void Execve(int quit, const char *filename, char *const argv[], char *const envp[]) {
  if (execve(filename, argv, envp) < 0) unix_error(quit, "Execve error");
}

pid_t Wait(int quit, int *status) {
  pid_t pid = wait(status);
  if (pid < 0) unix_error(quit, "Wait error");
  return pid;
}

pid_t Waitpid(int quit, pid_t pid, int *iptr, int options) {
  pid_t retpid = waitpid(pid, iptr, options);
  if (retpid < 0) unix_error(quit, "Waitpid error");
  return retpid;
}

void Kill(int quit, pid_t pid, int signum) {
  if (kill(pid, signum) < 0) unix_error(quit, "Kill error");
}

unsigned int Sleep(int quit, unsigned int secs) {
  unsigned int rc = sleep(secs);
  if (rc < 0) unix_error(quit, "Sleep error");
  return rc;
}
 
void Setpgid(int quit, pid_t pid, pid_t pgid) {
  if (setpgid(pid, pgid) < 0) unix_error(quit, "Setpgid error");
}

/************************************
 * Wrappers for Unix signal functions 
 ***********************************/

handler_t *Signal(int quit, int signum, handler_t *handler) {
  struct sigaction action, old_action;

  action.sa_handler = handler;  
  sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
  action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

  if (sigaction(signum, &action, &old_action) < 0) {
    unix_error(quit, "Signal error");
  }

  return (old_action.sa_handler);
}

void Sigprocmask(int quit, int how, const sigset_t *set, sigset_t *oldset) {
  if (sigprocmask(how, set, oldset) < 0) unix_error(quit, "Sigprocmask error");
}

void Sigemptyset(int quit, sigset_t *set) {
  if (sigemptyset(set) < 0) unix_error(quit, "Sigemptyset error");
}

void Sigfillset(int quit, sigset_t *set) { 
  if (sigfillset(set) < 0) unix_error(quit, "Sigfillset error");
}

void Sigaddset(int quit, sigset_t *set, int signum) {
  if (sigaddset(set, signum) < 0) unix_error(quit, "Sigaddset error");
}

void Sigdelset(int quit, sigset_t *set, int signum) {
  if (sigdelset(set, signum) < 0) unix_error(quit, "Sigdelset error");
}

int Sigismember(int quit, const sigset_t *set, int signum) {
  int rc = sigismember(set, signum);
  if (rc < 0) unix_error(quit, "Sigismember error");
  return rc;
}

int Sigsuspend(int quit, const sigset_t *set) {
  int rc = sigsuspend(set); /* always returns -1 */
  if (errno != EINTR) unix_error(quit, "Sigsuspend error");
  return rc;
}

/*************************************************************
 * The Sio (Signal-safe I/O) package - simple reentrant output
 * functions that are safe for signal handlers.
 *************************************************************/

/* Private sio functions */

/* sio_reverse - Reverse a string (from K&R) */
static void sio_reverse(char s[]) {
  int c, i, j;
  for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

/* sio_ltoa - Convert long to base b string (from K&R) */
static void sio_ltoa(long v, char s[], int b) {
  int c, i = 0;
  int neg = v < 0;
  if (neg) v = -v;
  
  do {  
    s[i++] = ((c = (v % b)) < 10)  ?  c + '0' : c - 10 + 'a';
  } while ((v /= b) > 0);

  if (neg) s[i++] = '-';

  s[i] = '\0';
  sio_reverse(s);
}

/* sio_strlen - Return length of string (from K&R) */
static size_t sio_strlen(char s[]) {
  int i = 0;
  while (s[i] != '\0') ++i;
  return i;
}

/* Public Sio functions */

/* Put string */
ssize_t sio_puts(char s[]) {
  return write(STDOUT_FILENO, s, sio_strlen(s));
}
/* Put long */
ssize_t sio_putl(long v) {
  char s[128];
  sio_ltoa(v, s, 10); /* Based on K&R itoa() */
  return sio_puts(s);
}
/* Put error message and exit */
void sio_error(char s[]) {
  sio_puts(s);
  _exit(1);
}

/*******************************
 * Wrappers for the SIO routines
 ******************************/

ssize_t Sio_putl(long v) {
  ssize_t n;
  if ((n = sio_putl(v)) < 0) sio_error("Sio_putl error");
  return n;
}

ssize_t Sio_puts(char s[]) {
  ssize_t n;
  if ((n = sio_puts(s)) < 0) sio_error("Sio_puts error");
  return n;
}

/********************************
 * Wrappers for Unix I/O routines
 ********************************/

int Open(int quit, const char *pathname, int flags, mode_t mode) {
  int rc = open(pathname, flags, mode);
  if (rc < 0) unix_error(quit, "Open error");
  return rc;
}

ssize_t Read(int quit, int fd, void *buf, size_t count) {
  ssize_t rc = read(fd, buf, count);
  if (rc < 0) unix_error(quit, "Read error");
  return rc;
}

ssize_t Write(int quit, int fd, const void *buf, size_t count) {
  ssize_t rc = write(fd, buf, count);
  if (rc < 0) unix_error(quit, "Write error");
  return rc;
}

off_t Lseek(int quit, int fildes, off_t offset, int whence) {
  off_t rc = lseek(fildes, offset, whence);
  if (rc < 0) unix_error(quit, "Lseek error");
  return rc;
}

int Close(int quit, int fd) {
  int rc = close(fd);
  if (rc < 0) unix_error(quit, "Close error");
  return rc;
}

int Select(int quit, int  n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
  int rc = select(n, readfds, writefds, exceptfds, timeout);
  if (rc < 0) unix_error(quit, "Select error");
  return rc;
}

int Dup2(int quit, int fd1, int fd2) {
  int rc = dup2(fd1, fd2);
  if (rc < 0) unix_error(quit, "Dup2 error");
  return rc;
}

void Stat(int quit, const char *filename, struct stat *buf) {
  if (stat(filename, buf) < 0) unix_error(quit, "Stat error");
}

void Fstat(int quit, int fd, struct stat *buf) {
  if (fstat(fd, buf) < 0) unix_error(quit, "Fstat error");
}

/*********************************
 * Wrappers for directory function
 *********************************/

DIR *Opendir(int quit, const char *name) {
  DIR *dirp = opendir(name); 
  if (!dirp) unix_error(quit, "opendir error");
  return dirp;
}

struct dirent *Readdir(int quit, DIR *dirp) {
  struct dirent *dep;  
  errno = 0;
  dep = readdir(dirp);
  if ((dep == NULL) && (errno != 0)) unix_error(quit, "readdir error");
  return dep;
}

int Closedir(int quit, DIR *dirp) {
  int rc = closedir(dirp);
  if (rc < 0) unix_error(quit, "closedir error");
  return rc;
}

/***************************************
 * Wrappers for memory mapping functions
 ***************************************/

void *Mmap(int quit, void *addr, size_t len, int prot, int flags, int fd, off_t offset) {
  void *ptr = mmap(addr, len, prot, flags, fd, offset);
  if (ptr == ((void *) -1)) unix_error(quit, "mmap error");
  return(ptr);
}

void Munmap(int quit, void *start, size_t length) {
  if (munmap(start, length) < 0) unix_error(quit, "munmap error");
}

/***************************************************
 * Wrappers for dynamic storage allocation functions
 ***************************************************/

void *Malloc(int quit, size_t size) {
  void *p = malloc(size);
  if (p == NULL) unix_error(quit, "Malloc error");
  return p;
}

void *Realloc(int quit, void *ptr, size_t size) {
  void *p = realloc(ptr, size);
  if (p == NULL) unix_error(quit, "Realloc error");
  return p;
}

void *Calloc(int quit, size_t nmemb, size_t size) {
  void *p = calloc(nmemb, size);
  if (p == NULL) unix_error(quit, "Calloc error");
  return p;
}

/******************************************
 * Wrappers for the Standard I/O functions.
 ******************************************/

void Fclose(int quit, FILE *fp) {
  if (fclose(fp) != 0) unix_error(quit, "Fclose error");
}

FILE *Fdopen(int quit, int fd, const char *type) {
  FILE *fp = fdopen(fd, type);
  if (fp == NULL) unix_error(quit, "Fdopen error");
  return fp;
}

char *Fgets(int quit, char *ptr, int n, FILE *stream) {
  char *rptr = fgets(ptr, n, stream);
  if ((rptr == NULL) && ferror(stream)) app_error(quit, "Fgets error\n");
  return rptr;
}

FILE *Fopen(int quit, const char *filename, const char *mode) {
  FILE *fp = fopen(filename, mode);
  if (fp == NULL) unix_error(quit, "Fopen error");
  return fp;
}

void Fputs(int quit, const char *ptr, FILE *stream) {
  if (fputs(ptr, stream) == EOF) unix_error(quit, "Fputs error");
}

size_t Fread(int quit, void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t n = fread(ptr, size, nmemb, stream);
  if ((n < nmemb) && ferror(stream)) unix_error(quit, "Fread error");
  return n;
}

void Fwrite(int quit, const void *ptr, size_t size, size_t nmemb, FILE *stream) {
  if (fwrite(ptr, size, nmemb, stream) < nmemb) unix_error(quit, "Fwrite error");
}

/**************************** 
 * Sockets interface wrappers
 ****************************/

int Socket(int quit, int domain, int type, int protocol) {
  int rc = socket(domain, type, protocol);
  if (rc < 0) unix_error(quit, "Socket error");
  return rc;
}

void Setsockopt(int quit, int s, int level, int optname, const void *optval, int optlen) {
  int rc = setsockopt(s, level, optname, optval, optlen);
  if (rc < 0) unix_error(quit, "Setsockopt error");
}

void Bind(int quit, int sockfd, struct sockaddr *my_addr, int addrlen) {
  if (bind(sockfd, my_addr, addrlen) < 0) unix_error(quit, "Bind error");
}

int Listen(int quit, int s, int backlog) {
  int rc = listen(s,  backlog);
  if (rc < 0) unix_error(quit, "Listen error");
  return rc;
}

int Accept(int quit, int s, struct sockaddr *addr, socklen_t *addrlen) {
  int rc = accept(s, addr, addrlen);
  if (rc < 0) unix_error(quit, "Accept error");
  return rc;
}

void Connect(int quit, int sockfd, struct sockaddr *serv_addr, int addrlen) {
  if (connect(sockfd, serv_addr, addrlen) < 0) unix_error(quit, "Connect error");
}

/*******************************
 * Protocol-independent wrappers
 *******************************/

int Getaddrinfo(int quit, const char *node, const char *service, 
                  const struct addrinfo *hints, struct addrinfo **res) {
  int rc = getaddrinfo(node, service, hints, res);
  if (rc != 0) gai_error(quit, rc, "Getaddrinfo error (%s:%s)", node, service);
  return rc;
}

int Getnameinfo(int quit, const struct sockaddr *sa, socklen_t salen, char *host, 
                 size_t hostlen, char *serv, size_t servlen, int flags) {

  int rc = getnameinfo(sa, salen, host, hostlen, serv, servlen, flags);
  if (rc != 0) gai_error(quit, rc, "Getnameinfo error");
  return rc;
}

void Inet_ntop(int quit, int af, const void *src, char *dst, socklen_t size) {
  if (!inet_ntop(af, src, dst, size)) unix_error(quit, "Inet_ntop error");
}

void Inet_pton(int quit, int af, const char *src, void *dst) {
  int rc;
  rc = inet_pton(af, src, dst);
  if (rc == 0) app_error(quit, "inet_pton error: invalid dotted-decimal address\n");
  else if (rc < 0) unix_error(quit, "Inet_pton error");
}

/*******************************************
 * DNS interface wrappers. 
 *
 * NOTE: These are obsolete because they are not thread safe. Use
 * getaddrinfo and getnameinfo instead
 ***********************************/

struct hostent *Gethostbyname(int quit, const char *name) {
  struct hostent *p = gethostbyname(name);
  if (p == NULL) app_error(quit, "Gethostbyname error\n");
  return p;
}

struct hostent *Gethostbyaddr(int quit, const char *addr, int len, int type) {
  struct hostent *p = gethostbyaddr(addr, len, type);
  if (p == NULL) app_error(quit, "Gethostbyaddr error\n");
  return p;
}

/************************************************
 * Wrappers for Pthreads thread control functions
 ************************************************/

void Pthread_create(int quit, pthread_t *tidp, pthread_attr_t *attrp,
                      void * (*routine)(void *), void *argp) {
  int rc = pthread_create(tidp, attrp, routine, argp);
  if (rc != 0) posix_error(quit, rc, "Pthread_create error");
}

void Pthread_cancel(int quit, pthread_t tid) {
  int rc = pthread_cancel(tid);
  if (rc != 0) posix_error(quit, rc, "Pthread_cancel error");
}

void Pthread_join(int quit, pthread_t tid, void **thread_return) {
  int rc = pthread_join(tid, thread_return);
  if (rc != 0) posix_error(quit, rc, "Pthread_join error");
}

void Pthread_detach(int quit, pthread_t tid) {
  int rc = pthread_detach(tid);
  if (rc != 0) posix_error(quit, rc, "Pthread_detach error");
}

/*******************************
 * Wrappers for Posix semaphores
 *******************************/

void Sem_init(int quit, sem_t *sem, int pshared, unsigned int value) {
  if (sem_init(sem, pshared, value) < 0) unix_error(quit, "Sem_init error");
}

void Sem_wait(int quit, sem_t *sem) {
  if (sem_wait(sem) < 0) unix_error(quit, "sem_wait error");
}

void Sem_post(int quit, sem_t *sem) {
  if (sem_post(sem) < 0) unix_error(quit, "sem_post error");
}

void P(int quit, sem_t *sem) { Sem_wait(quit, sem); }

void V(int quit, sem_t *sem) { Sem_post(quit, sem); }

/****************************************
 * The Rio package - Robust I/O functions
 ****************************************/

/**
 * Robustly read n bytes from fd into usrbuf (unbuffered)
 * Return:
 * - positive number: the number of bytes read into usrbuf
 * - zero: end of file (EOF)
 * - negative number (-1): error, errno is set
 */
ssize_t rio_read(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nread;
  char *bufp = usrbuf;
  
  while (nleft > 0) {
    if ((nread = read(fd, bufp, nleft)) < 0) {
      if (errno == EINTR)  /* Interrupted by sig handler return */
        nread = 0;         /* and call read() again */
      else
        return -1;         /* errno set by read() */ 
    } 
    else if (nread == 0) break;              /* EOF */

    nleft -= nread;
    bufp += nread;
  }

  return (n - nleft);         /* Return >= 0 */
}

/**
 * Robustly write n bytes from usrbuf into fd (unbuffered)
 * Return number of bytes written (n) on success, -1 on error (errno is set)
 */
ssize_t rio_write(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nwritten;
  char *bufp = usrbuf;

  while (nleft > 0) {
    if ((nwritten = write(fd, bufp, nleft)) <= 0) {
      if (errno == EINTR)         /* Interrupted by sig handler return */
        nwritten = 0;             /* and call write() again */
      else
        return -1;                /* errno set by write() */
    }
    nleft -= nwritten;
    bufp += nwritten;
  }

  return n;
}

static ssize_t rio_writef_args(int fd, size_t n, const char* format, va_list args){
  if (n == 0) n = MAXLINE;
  char buf[n];
  int fstr_len = vsnprintf(buf, n, format, args);
  return rio_write(fd, buf, fstr_len);
}

/**
 * Write the formatted string of maximum length (n) to file descriptor (fd).
 * return the length of the formatted string, -1 on error (errno is set)
 * - if n = 0, the default buffer size (MAXLINE) is used.
*/
ssize_t rio_writef(int fd, size_t n, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int rc = rio_writef_args(fd, n, format, args);
  va_end(args);
  return rc;
}

/* 
 * riob_read_buffer - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, riob_read_buffer() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
static ssize_t riob_read_buffer(rio_t *rp, char *usrbuf, size_t n) {
  int cnt;

  /* Refill if buf is empty */
  while (rp->rio_cnt <= 0) {      
	  rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
	  if (rp->rio_cnt < 0) {
	    if (errno != EINTR) return -1;    /* Continue if Interrupted by sig handler */
	  }
	  else if (rp->rio_cnt == 0) return 0; /* EOF */
	  else rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
  }

  /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
  cnt = (rp->rio_cnt < n)? rp->rio_cnt : n;
  memcpy(usrbuf, rp->rio_bufptr, cnt);
  rp->rio_bufptr += cnt;
  rp->rio_cnt -= cnt;
  return cnt;
}

/*
 * Associate descriptor (fd) with a rio read buffer (rp) and reset buffer.
 */
void riob_init(rio_t *rp, int fd) {
  rp->rio_fd = fd;  
  rp->rio_cnt = 0;  
  rp->rio_bufptr = rp->rio_buf;
}

/**
 * Robustly read n bytes from rp into usrbuf (buffered)
 * Return:
 * - positive number: the number of bytes read into usrbuf
 * - zero: end of file (EOF)
 * - negative number (-1): error, errno is set
 */
ssize_t riob_read(rio_t *rp, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nread;
  char *bufp = usrbuf;
    
  while (nleft > 0) {
    nread = riob_read_buffer(rp, bufp, nleft);
	  if (nread < 0) return -1;       /* errno set by read() */ 
	  else if (nread == 0) break;     /* EOF */
	  nleft -= nread;
	  bufp += nread;
  }
  
  return (n - nleft);         /* return >= 0 */
}

/**
 * Robustly read a text line from rp into usrbuf (buffered)
 * Return:
 * - positive number: the length of string read into usrbuf
 * - zero: end of file (EOF)
 * - negative number (-1): error, errno is set
 */
ssize_t riob_readline(rio_t *rp, void *usrbuf, size_t maxlen) {
  int n, rc;
  char c, *bufp = usrbuf;

  for (n = 1; n < maxlen; n++) {
    rc = riob_read_buffer(rp, &c, 1);
    if (rc == 1) {
      *bufp++ = c;
      if (c == '\n') {
        n++;
        break;
      }
    } else if (rc == 0) {
      if (n == 1)
        return 0; /* EOF, no data read */
      else
        break;    /* EOF, some data was read */
    } else return -1;	  /* Error */
  }

  *bufp = 0;
  return n-1;
}

/**********************************
 * Wrappers for robust I/O routines
 **********************************/

ssize_t Rio_read(int quit, int fd, void *usrbuf, size_t n){
  ssize_t rc = rio_read(fd, usrbuf, n);
  if (rc < 0) unix_error(quit, "Rio_read error");
  return rc;
}

ssize_t Rio_write(int quit, int fd, void *usrbuf, size_t n){
  ssize_t rc = rio_write(fd, usrbuf, n);
  if (rc != n) unix_error(quit, "Rio_write error");
  return rc;
}

ssize_t Rio_writef(int quit, int fd, size_t n, const char *format, ...){
  va_list args;
  va_start(args, format);
  int rc = rio_writef_args(fd, n, format, args);
  va_end(args);
  if (rc < 0) unix_error(quit, "Rio_writef error");
  return rc;
}

ssize_t Riob_read(int quit, rio_t *rp, void *usrbuf, size_t n){
  ssize_t rc = riob_read(rp, usrbuf, n);
  if (rc < 0) unix_error(quit, "Riob_read error");
  return rc;
}

ssize_t Riob_readline(int quit, rio_t *rp, void *usrbuf, size_t maxlen){
  ssize_t rc = riob_readline(rp, usrbuf, maxlen);
  if (rc < 0) unix_error(quit, "Riob_readline error");
  return rc;
}

/******************************** 
 * Client/server helper functions
 ********************************/

/*
 * open_client - Open connection to server at <host, port> and
 *     return a socket descriptor ready for reading and writing. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns: 
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
int open_client(const char* host, const char* port){
  int clientfd;
  AI hints, *info, *p;

  memset(&hints, 0, sizeof(AI));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV;

  if (Getaddrinfo(0, host, port, &hints, &info) != 0) return -2;

  for(p = info; p; p = p->ai_next){
    clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (clientfd == -1) continue;
    if (connect(clientfd, p->ai_addr, p->ai_addrlen) == 0) break; /* Success */
    if (Close(0, clientfd) < 0) return -1;
  }

  freeaddrinfo(info);
  if (p == NULL) return -1;
  
  return clientfd;
}

/*  
 * open_listenfd - Open and return a listening socket on port. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns: 
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
int open_server(const char* port){
  int serverfd, optval = 1;
  AI hints, *info, *p;

  memset(&hints, 0, sizeof(AI));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE | AI_NUMERICSERV;

  if (Getaddrinfo(0, NULL, port, &hints, &info) != 0) return -2;

  for(p = info; p; p = p->ai_next){
    serverfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (serverfd == -1) continue;
    /* Eliminates "Address already in use" error from bind */
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));
    if (bind(serverfd, p->ai_addr, p->ai_addrlen) == 0) break; /* Success */
    if (Close(0, serverfd) < 0) return -1;
  }

  freeaddrinfo(info);
  if (p == NULL) return -1;

  if (Listen(0, serverfd, BACKLOG) == -1){
    Close(0, serverfd);
    return -1;
  }

  return serverfd;
}

/****************************************************
 * Wrappers for reentrant protocol-independent helpers
 ****************************************************/

int Open_client(int quit, const char* host, const char* port) {
  int sock = open_client(host, port);
  if (sock == -1) unix_error(quit, "Open_client error");
  return sock;
}

int Open_server(int quit, const char* port) {
  int sock = open_server(port);
  if (sock == -1) unix_error(quit, "Open_server error");
  return sock;
}

/********************
 * String Utils
 *******************/

/**
 * Append the formatted string of maximum length (len) to dest.
 * return the length of the formatted string.
 * - if len = 0, the default buffer size (MAXLINE) is used.
*/
int strncatf(char* dest, size_t len, const char* format, ...){
  va_list args;
  va_start(args, format);
  if (len == 0) len = MAXLINE;
  char buf[len];
  int fstr_len = vsnprintf(buf, len, format, args);
  va_end(args);
  strcat(dest, buf);
  return fstr_len;
}

/**
 * Returns 1 if (s1 = s2), 0 otherwise.
 * sensitive flag is used to indicate case sensitivity
*/
int streq(const char* s1, const char* s2, int sensitive){
  return sensitive? (!strcmp(s1, s2)) : (!strcasecmp(s1, s2));
}

/**
 * Returns 1 if (s1 = s2) upto the first n chars, 0 otherwise.
 * sensitive flag is used to indicate case sensitivity
*/
int strneq(const char* s1, const char* s2, int n, int sensitive){
  return sensitive? (!strncmp(s1, s2, n)) : (!strncasecmp(s1, s2, n));
}
