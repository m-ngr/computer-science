/**
 * dtc.h: dynamic threads controller package 
*/

#ifndef __DTC_H__
#define __DTC_H__

#include "mcsapp.h"

/* Dynamic Threads Controller type */
typedef struct {
  int min;                    /* minimum number of threads */
  int max;                    /* maximum number of threads */
  int total;                  /* total number of threads */
  int available;              /* number of threads ready to be used */
  int step;                   /* number of threads to add when all threads are busy */
  void* (*thread) (void*);
  sem_t mutex;
} dtc_t;

int dtc_init(dtc_t* dtcont, void* (*thread) (void*), int min, int max, int step);
void dtc_destroy(dtc_t *dtcont);
void dtc_work(dtc_t *dtcont);
void dtc_rest(dtc_t *dtcont);

#endif /* __DTC_H__ */