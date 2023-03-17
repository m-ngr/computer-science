/**
 * cisbuf.h: client info shared buffer package 
*/

#ifndef __CISBUF_H__
#define __CISBUF_H__

#include "mcsapp.h"

typedef struct{
  int sock;
  struct sockaddr_storage address;        /* Enough space for any address */
  socklen_t length;                       /* Address Length */
} client_info;

/* client info shared buffer type */
typedef struct {
  client_info* buf;
  int size;
  int front;
  int rear;
  sem_t mutex;       /* Protects accesses to buf */
  sem_t slots;       /* Counts available slots */
  sem_t items;       /* Counts available items */
} cisbuf_t;

void cisbuf_init(cisbuf_t* cis, int n);
void cisbuf_destroy(cisbuf_t *cis);
void cisbuf_insert(cisbuf_t *cis, client_info* item);
void cisbuf_remove(cisbuf_t *cis, client_info* item);

#endif /* __CISBUF_H__ */