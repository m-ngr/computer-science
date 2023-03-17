#include "cisbuf.h"

/* Create an empty, bounded, client info shared FIFO buffer with n slots */
void cisbuf_init(cisbuf_t* cis, int n) {
  cis->buf = Calloc(0, n, sizeof(client_info)); 
  cis->size = n;
  cis->front = cis->rear = 0;
  Sem_init(0, &cis->mutex, 0, 1);
  Sem_init(0, &cis->slots, 0, n);
  Sem_init(0, &cis->items, 0, 0);
}

/* Clean up buffer cis */
void cisbuf_destroy(cisbuf_t *cis) {
  free(cis->buf);
  sem_destroy(&(cis->mutex));
  sem_destroy(&(cis->slots));
  sem_destroy(&(cis->items));
}

/* Insert item onto the rear of shared buffer cis */
void cisbuf_insert(cisbuf_t *cis, client_info* item) {
  Sem_wait(0, &cis->slots);
  Sem_wait(0, &cis->mutex);      
  client_info* slot = &(cis->buf[(++cis->rear)%(cis->size)]);
  memcpy(slot, item, sizeof(client_info));
  Sem_post(0, &cis->mutex);
  Sem_post(0, &cis->items);
}

/* Remove and return the first item from buffer cis */
void cisbuf_remove(cisbuf_t *cis, client_info* item) {
  Sem_wait(0, &cis->items);
  Sem_wait(0, &cis->mutex);
  client_info* slot = &(cis->buf[(++cis->front)%(cis->size)]);
  memcpy(item, slot, sizeof(client_info));
  Sem_post(0, &cis->mutex);
  Sem_post(0, &cis->slots);
}
