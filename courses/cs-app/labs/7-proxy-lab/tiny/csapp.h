/*
 * csapp.h - prototypes and definitions for the CS:APP3e book (Modified)
 */

#ifndef __CSAPP_H__
#define __CSAPP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>

/* Default file permissions are DEF_MODE & ~DEF_UMASK */
#define DEF_MODE   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define DEF_UMASK  S_IWGRP|S_IWOTH

/* Simplifies calls to bind(), connect(), and accept() */
typedef struct addrinfo AI;
typedef struct sockaddr SA;
typedef struct sockaddr_in SIN;

/* Persistent state for the robust I/O (Rio) package */
#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* Descriptor for this internal buf */
    int rio_cnt;               /* Unread bytes in internal buf */
    char *rio_bufptr;          /* Next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;

/* External variables */
extern int h_errno;    /* Defined by BIND for DNS errors */ 
extern char **environ; /* Defined by libc */

/* Misc constants */
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define BACKLOG 1024   /* Second argument to listen() */

/* Our own error-handling functions */

int unix_error(int quit, const char* format, ...);
void posix_error(int quit, int code, const char* format, ...);
void gai_error(int quit, int code, const char* format, ...);
void app_error(int quit, const char* format, ...);

/* Process control wrappers */

pid_t Fork(int quit);
void Execve(int quit, const char *filename, char *const argv[], char *const envp[]);
pid_t Wait(int quit, int *status);
pid_t Waitpid(int quit, pid_t pid, int *iptr, int options);
void Kill(int quit, pid_t pid, int signum);
unsigned int Sleep(int quit, unsigned int secs);
void Setpgid(int quit, pid_t pid, pid_t pgid);

/* Signal wrappers */

typedef void handler_t(int);
handler_t *Signal(int quit, int signum, handler_t *handler);
void Sigprocmask(int quit, int how, const sigset_t *set, sigset_t *oldset);
void Sigemptyset(int quit, sigset_t *set);
void Sigfillset(int quit, sigset_t *set);
void Sigaddset(int quit, sigset_t *set, int signum);
void Sigdelset(int quit, sigset_t *set, int signum);
int Sigismember(int quit, const sigset_t *set, int signum);
int Sigsuspend(int quit, const sigset_t *set);

/* Sio (Signal-safe I/O) routines */

ssize_t sio_puts(char s[]);
ssize_t sio_putl(long v);
void sio_error(char s[]);

/* Sio wrappers */

ssize_t Sio_puts(char s[]);
ssize_t Sio_putl(long v);

/* Unix I/O wrappers */

int Open(int quit, const char *pathname, int flags, mode_t mode);
ssize_t Read(int quit, int fd, void *buf, size_t count);
ssize_t Write(int quit, int fd, const void *buf, size_t count);
off_t Lseek(int quit, int fildes, off_t offset, int whence);
int Close(int quit, int fd);
int Select(int quit, int  n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int Dup2(int quit, int fd1, int fd2);
void Stat(int quit, const char *filename, struct stat *buf);
void Fstat(int quit, int fd, struct stat *buf) ;

/* Directory wrappers */

DIR *Opendir(int quit, const char *name);
struct dirent *Readdir(int quit, DIR *dirp);
int Closedir(int quit, DIR *dirp);

/* Memory mapping wrappers */

void *Mmap(int quit, void *addr, size_t len, int prot, int flags, int fd, off_t offset);
void Munmap(int quit, void *start, size_t length);

/* Standard I/O wrappers */

void Fclose(int quit, FILE *fp);
FILE *Fdopen(int quit, int fd, const char *type);
char *Fgets(int quit, char *ptr, int n, FILE *stream);
FILE *Fopen(int quit, const char *filename, const char *mode);
void Fputs(int quit, const char *ptr, FILE *stream);
size_t Fread(int quit, void *ptr, size_t size, size_t nmemb, FILE *stream);
void Fwrite(int quit, const void *ptr, size_t size, size_t nmemb, FILE *stream);

/* Dynamic storage allocation wrappers */

void *Malloc(int quit, size_t size);
void *Realloc(int quit, void *ptr, size_t size);
void *Calloc(int quit, size_t nmemb, size_t size);

/* Sockets interface wrappers */

int Socket(int quit, int domain, int type, int protocol);
void Setsockopt(int quit, int s, int level, int optname, const void *optval, int optlen);
void Bind(int quit, int sockfd, struct sockaddr *my_addr, int addrlen);
int Listen(int quit, int s, int backlog);
int Accept(int quit, int s, struct sockaddr *addr, socklen_t *addrlen);
void Connect(int quit, int sockfd, struct sockaddr *serv_addr, int addrlen);

/* Protocol independent wrappers */

int Getaddrinfo(int quit, const char *node, const char *service, 
                 const struct addrinfo *hints, struct addrinfo **res);
int Getnameinfo(int quit, const struct sockaddr *sa, socklen_t salen, char *host, 
                 size_t hostlen, char *serv, size_t servlen, int flags);
void Inet_ntop(int quit, int af, const void *src, char *dst, socklen_t size);
void Inet_pton(int quit, int af, const char *src, void *dst); 

/* DNS wrappers */

struct hostent *Gethostbyname(int quit, const char *name);
struct hostent *Gethostbyaddr(int quit, const char *addr, int len, int type);

/* Pthreads thread control wrappers */

void Pthread_create(int quit, pthread_t *tidp, pthread_attr_t *attrp, 
		    void * (*routine)(void *), void *argp);
void Pthread_join(int quit, pthread_t tid, void **thread_return);
void Pthread_cancel(int quit, pthread_t tid);
void Pthread_detach(int quit, pthread_t tid);

/* POSIX semaphore wrappers */

void Sem_init(int quit, sem_t *sem, int pshared, unsigned int value);
void P(int quit, sem_t *sem);
void V(int quit, sem_t *sem);
void Sem_wait(int quit, sem_t *sem);
void Sem_post(int quit, sem_t *sem);

/* Rio (Robust I/O) package */

ssize_t rio_read(int fd, void *usrbuf, size_t n);
ssize_t rio_write(int fd, void *usrbuf, size_t n);
ssize_t rio_writef(int fd, size_t n, const char *format, ...);
void riob_init(rio_t *rp, int fd);
ssize_t riob_read(rio_t *rp, void *usrbuf, size_t n);
ssize_t riob_readline(rio_t *rp, void *usrbuf, size_t maxlen);

/* Wrappers for Rio package */

ssize_t Rio_read(int quit, int fd, void *usrbuf, size_t n);
ssize_t Rio_write(int quit, int fd, void *usrbuf, size_t n);
ssize_t Rio_writef(int quit, int fd, size_t n, const char *format, ...);
ssize_t Riob_read(int quit, rio_t *rp, void *usrbuf, size_t n);
ssize_t Riob_readline(int quit, rio_t *rp, void *usrbuf, size_t maxlen);

/* Reentrant protocol-independent client/server helpers */

int open_client(const char *host, const char *port);
int open_server(const char *port);

/* Wrappers for reentrant protocol-independent client/server helpers */

int Open_client(int quit, const char *host, const char *port);
int Open_server(int quit, const char *port);

/* string utils */

int strncatf(char* dest, size_t len, const char* format, ...);
int streq(const char* s1, const char* s2, int sensitive);
int strneq(const char* s1, const char* s2, int n, int sensitive);

#endif /* __CSAPP_H__ */