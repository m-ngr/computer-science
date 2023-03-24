#include "dtc.h"

static void run_threads(dtc_t* dtcont, int num){
  pthread_t tid;
  int rem = (dtcont->max - dtcont->total);
  num = (num < rem)? num : rem;

  for(int i = 0; i < num; ++i){
    Pthread_create(0, &tid, NULL, dtcont->thread, NULL);
  }
  dtcont->total += num;
  dtcont->available += num;
}

/* initialize dynamic threads controller, return 0 on success -1 on error */
int dtc_init(dtc_t* dtcont, void* (*thread) (void*), int min, int max, int step) {
  if (step < 1 || min < 1 || max < min) return -1;
  dtcont->min = min;
  dtcont->max = max;
  dtcont->step = step;
  dtcont->total = 0;
  dtcont->available = 0;
  dtcont->thread = thread;
  Sem_init(0, &dtcont->mutex, 0, 1);
  run_threads(dtcont, min);
  return 0;
}

/* destroy the dynamic threads controller */
void dtc_destroy(dtc_t *dtcont) {
  sem_destroy(&(dtcont->mutex));
}

/** 
 * Mark the calling thread as busy, run additional threads if required
 * Must be called from the target thread (dtcont->thread)
*/
void dtc_work(dtc_t *dtcont){
  Sem_wait(0, &dtcont->mutex);
  dtcont->available -= 1;
  if (dtcont->available == 0){
    run_threads(dtcont, dtcont->step);
  }
  Sem_post(0, &dtcont->mutex);
}

/**
 * Mark the calling thread as available, close it if no longer needed
 * Must be called from the target thread (dtcont->thread)
*/
void dtc_rest(dtc_t *dtcont){
  Sem_wait(0, &dtcont->mutex);
  if (dtcont->available >= dtcont->min){
    dtcont->total -= 1;
    Sem_post(0, &dtcont->mutex);
    pthread_exit(NULL);
  }
  dtcont->available += 1;
  Sem_post(0, &dtcont->mutex);
}
