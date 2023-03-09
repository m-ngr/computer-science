#ifndef __CACHE_H__
#define __CACHE_H__

#include "csapp.h"

#define MAXKEY 8192
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

typedef struct CacheNode {
  char key[MAXKEY];
  void* data;
  int size;
  struct CacheNode* next;
  struct CacheNode* prev;
} cache_node;

typedef struct{
  cache_node* head;
  cache_node* tail;
  int size;
  sem_t mutex;
} cache_t;

typedef struct{
  char key[MAXKEY];
  char data[MAX_OBJECT_SIZE];
  int size;
} cache_object;

void cache_init(cache_t* cache);
void cache_destroy(cache_t* cache);
void cache_set(cache_t* cache, cache_object* object);
int cache_get(cache_t* cache, cache_object* object);

void cache_setkey(cache_object* object, const char* format, ...);
void cache_write(cache_object* object, void* buf, int n);
void cache_reset(cache_object* object);

#endif