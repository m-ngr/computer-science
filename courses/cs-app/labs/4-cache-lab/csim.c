/* Mahmoud Elnagar - 27/12/2022 */
#include "cachelab.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <ctype.h>

char* BIN_NAME = "csim";
const int M = 32; // address-width

int read_int(int* dst, char* src){
  char* p = src;
  if (*p == '-'||*p == '+') ++p;
  for(; *p != '\0'; ++p){
    if (!isdigit(*p)) return 0;
  }  
  *dst = atoi(src);
  return 1;
}

void print_exit(){
  fprintf(stderr, "Usage: %s -s <s> -E <E> -b <b> -t <tracefile>\n", BIN_NAME);
  exit(EXIT_FAILURE);
}

void get_args(int argc, char *argv[], int* sp, int* ep, int* bp, char** fp){
  BIN_NAME = argv[0];
  if (argc < 9) print_exit(argv[0]);
  int opt;

  while ((opt = getopt(argc, argv, "s:E:b:t:")) != -1) {
    switch (opt) {
    case 's':
      if (!read_int(sp, optarg)) print_exit();
      break;
    case 'E':
      if (!read_int(ep, optarg)) print_exit();
      break;
    case 'b':
      if (!read_int(bp, optarg)) print_exit();
      break;
    case 't':
      if (!(*fp = optarg)) print_exit();
      break;
    default:
      print_exit();
    }
  }
}

void verify_input(int s, int e, int b){
  if (s < 0){
    printf("Failed: s should be non-negative number.\n");
    exit(EXIT_FAILURE);
  }

  if (e < 1){
    printf("Failed: E should be a postive number.\n");
    exit(EXIT_FAILURE);
  }

  if (b < 0){
    printf("Failed: b should be non-negative number.\n");
    exit(EXIT_FAILURE);
  }

  if ((s + b) >= M){
    printf("Failed: s+b should be less than %d.\n", M);
    exit(EXIT_FAILURE);
  }
  
}

typedef struct MemoryOperations{
  char type;
  unsigned address;
  struct MemoryOperations* next;
} MemOps_t, * MemOps;

MemOps make_memops(char type, unsigned address){
  MemOps ops = malloc(sizeof(MemOps_t));
  if (!ops) return NULL;
  ops->type = type;
  ops->address = address;
  ops->next = NULL;
  return ops;
}

void destroy_memops(MemOps ops){
  while(ops){
    MemOps next = ops->next;
    free(ops);
    ops = next;
  }
}

MemOps read_memory_file(char* file){
  FILE *fin;
  if (!(fin = fopen(file, "r"))) {
    printf("Failed: Could not open file %s.\n", file);
    exit(EXIT_FAILURE);
  }

  char type;
  unsigned address;
  MemOps head = NULL, tail = NULL;

  while(fscanf(fin," %c %x,%*d", &type, &address) == 2) {
    MemOps obj = make_memops(type, address);
    if (!head) head = obj; else tail->next = obj;
    tail = obj;
  }

  fclose(fin);
  return head;
}

unsigned get_tag(MemOps ops, int t){
  return (ops->address) >> (M - t);
}

int get_set(MemOps ops, int t, int b){
  return ((ops->address) << t) >> (t + b);
}

typedef struct CacheLine{
  int valid;
  unsigned tag;
  unsigned time;
} CacheLine, * CacheSet, ** Cache;

void init_cache(int S, int E, CacheLine cache[S][E]){
  for (int i = 0; i < S; ++i){
    for(int j = 0; j < E; ++j){
      cache[i][j].valid = 0;
      cache[i][j].tag = 0;
      cache[i][j].time = 0;
    }
  }
}

void add_cycle(int S, int E, CacheLine cache[S][E]){
  for (int i = 0; i < S; ++i){
    for(int j = 0; j < E; ++j){
      cache[i][j].time += 1;
    }
  }
}

int is_hit(int E, CacheSet set, unsigned tag){
  for(int i = 0; i < E; ++i){
    if(set[i].valid && (set[i].tag == tag)){
      set[i].time = 0;
      return 1;
    }
  }
  return 0;
}

int load(int E, CacheSet set, unsigned tag){
  /* return 1 if eviction is required, 0 otherwise*/
  unsigned lru = 0, time_stamp = set[0].time;

  for(int i = 0; i < E; ++i){
    if(set[i].valid == 0){
      set[i].valid = 1;
      set[i].tag = tag;
      set[i].time = 0;
      return 0; // no-eviction
    }

    if(set[i].time > time_stamp){
      time_stamp = set[i].time;
      lru = i;
    } 
  }

  set[lru].tag = tag;
  set[lru].time = 0;
  return 1; // eviction
}

void process(MemOps ops, int s, int E, int b){
  int hits = 0, misses = 0, evicts = 0;
  int t = M - (s + b); // tag-bits
  int S = (1 << s); // S = 2^s ; Set-Size
  CacheLine cache[S][E];
  init_cache(S,E,cache);

  for(MemOps mop = ops; mop != NULL; mop = mop->next){
    if (mop->type == 'I') continue;
    if (mop->type == 'M') hits += 1;

    CacheSet set = cache[get_set(mop, t, b)];
    unsigned tag = get_tag(mop, t);

    if (is_hit(E,set,tag)){
      hits += 1;
    }else{
      misses += 1;
      evicts += load(E,set,tag);
    }

    add_cycle(S,E,cache);
  }

  printSummary(hits, misses, evicts);
}

int main(int argc, char *argv[]){
  int s = 0,E = 0,b = 0;
  char* file = NULL;

  get_args(argc, argv, &s, &E, &b, &file);
  verify_input(s,E,b);
  MemOps ops = read_memory_file(file);
  
  process(ops, s, E, b);

  destroy_memops(ops);
  return 0;
}
