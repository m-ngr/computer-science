#include "cache.h"

// =========================================== Function prototype ====================================================

static void link_front(cache_t*, cache_node*);
static void unlink_node(cache_t*, cache_node*);
static void insert_cache(cache_t*, const char*, const void*, int);
static int remove_tail(cache_t*);
static void free_cache(cache_t*, int);

// =========================================== Public Functions =======================================================

/**
 * Initialize the cache. Done once before any usage.
*/
void cache_init(cache_t* cache) {
  cache->head = NULL;
  cache->tail = NULL;
  cache->size = 0;
  sem_init(&(cache->mutex), 0, 1);
}

/**
 * Destroy the cache. Done once, cache can no longer be used.
*/
void cache_destroy(cache_t* cache) {
  cache_node *curr = cache->head, *next;

  while (curr){
    next = curr->next;
    if (curr->data) free(curr->data);
    free(curr);
    curr = next;
  }

  sem_destroy(&(cache->mutex));
}

/**
 * Insert the given cache object into cache if (n > MAX_OBJECT_SIZE).
*/
void cache_set(cache_t* cache, cache_object* object) {
  if ((object->size > MAX_OBJECT_SIZE) || (object->size < 1)) return;
  sem_wait(&(cache->mutex));
  if ((cache->size + object->size) > MAX_CACHE_SIZE) {
    free_cache(cache, object->size);
  }
  insert_cache(cache, object->key, object->data, object->size);
  sem_post(&(cache->mutex));
}

/**
 * If key is found, load object into data and length into n
 * Returns 1 if found, 0 if not found.
*/
int cache_get(cache_t* cache, cache_object* object) {
  for (cache_node* curr = cache->head; curr; curr = curr->next) {
    if (strneq(curr->key, object->key, MAXKEY, 0)) {
      memcpy(object->data, curr->data, curr->size);
      object->size = curr->size;
      if (cache->head != curr) {
        sem_wait(&(cache->mutex));
        unlink_node(cache, curr);
        link_front(cache, curr);
        sem_post(&(cache->mutex));
      }
      return 1;
    }
  }
  return 0;
}

/**
 * set the object key to the formatted string
*/
void cache_setkey(cache_object* object, const char* format, ...){
  va_list args;
  va_start(args, format);
  vsnprintf(object->key, MAXKEY, format, args);
  va_end(args);
}

/**
 * write n bytes from buf to object
*/
void cache_write(cache_object* object, void* buf, int n){
  if ((object->size + n) < MAX_OBJECT_SIZE) {
    memcpy((object->data) + (object->size), buf, n);
  }
  object->size += n;
}

/**
 * reset the cache object
*/
void cache_reset(cache_object* object){
  strcpy(object->key, "");
  object->size = 0;
}

// =========================================== Private Functions =======================================================
/** Link node to the front of the cache */
static void link_front(cache_t* cache, cache_node* node){
  if (cache->head) cache->head->prev = node;
  node->next = cache->head;
  node->prev = NULL;
  cache->head = node;
  if (cache->tail == NULL) cache->tail = node;
}

/** unlink node from the cache (node is not deleted) */
static void unlink_node(cache_t* cache, cache_node* node){
  cache_node *next = node->next;
  cache_node *prev = node->prev;
  if (next) next->prev = prev;
  if (prev) prev->next = next;
  if (cache->head == node) cache->head = next; 
  if (cache->tail == node) cache->tail = prev; 
  node->next = node->prev = NULL;
}

/** insert {key: data} into the front of the cache */
static void insert_cache(cache_t* cache, const char* key, const void* data, int n){
  cache_node *new_node = malloc(sizeof(cache_node));
  new_node->data = malloc(n);
  new_node->size = n;
  strncpy(new_node->key, key, MAXKEY);
  memcpy(new_node->data, data, n);
  link_front(cache, new_node);
  cache->size += n;
}

/**
 * Remove the tail cache node
 * return size of the removed node, -1 if no node is removed.
*/
static int remove_tail(cache_t* cache){
  if (cache->tail == NULL) return -1;
  cache_node* target = cache->tail;
  unlink_node(cache, target);
  int size = target->size;
  cache->size -= size;
  if (target->data) free(target->data);
  free(target);
  return size;
}

/** Free at least size bytes from the cache */
static void free_cache(cache_t* cache, int size){
  while (size > 0){
    int f_size = remove_tail(cache);
    if (f_size < 0) return;
    size -= f_size;
  }
}
