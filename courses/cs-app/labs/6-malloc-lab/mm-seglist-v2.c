/*
 * mm-seglist-v2.c - segregated list based malloc package v2.
 * 
 * Perf index = 55 (util) + 40 (thru) = 95/100
 * 
 * Block Format: Minimum size is 16 bytes.
 *  - Free-Block Format: [Header - Pred - Succ - (Empty) - Footer]
 *  - Allocated-Block Format: [Header - Payload - Footer]
 *  - Header/Footer: 1-word holds size of the block and allocation-bit at LSB
 *  - Pred: 1-word holds the address of the predecessor free-block.
 *  - Succ: 1-word holds the address of the successor free-block.
 * 
 * List Format: array of 8 explicit-free lists
 * Heap Format: [seglist-array[8] | Head-Block[1] | Regular-Blocks ...| Tail-Block[1]]
 *  - Head/Tail: 1-word allocated block of zero size.
 * 
 * Placement Policy: using best-fit algorithm.
 * Split Policy: split if remainder is greater than 16 bytes
 * Coalescing Policy: bi-direction coalescing for free-blocks and allocated-blocks
 * 
 * realloc:
 *  - if new-size is greater than current-size: try to coalesce with nearby free-blocks
 *    to get more size, if fails then request a new-block with new-size from memory, 
 *    copy content, then free the old-block.
 *  - if the new-size is less than current size by 64 byte at least then split.
 * 
 * NOTE: Coalescing is not strict, there are some free blocks not coalesced just to keep
 *       a variety of sizes in the seglist.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* =============================================================================== */

team_t team = {
  /* Team name (My github profile ^_^)*/
  "https://github.com/m-ngr",
  /* First member's full name */
  "Mahmoud Elnagar",
  /* First member's email address */
  "mahmoud.elnagar1698@gmail.com",
  /* Second member's full name (leave blank if none) */
  "",
  /* Second member's email address (leave blank if none) */
  ""
};

/* ========================== Function Prototype =============================== */

inline static void* resize_block(void*, size_t);
inline static void* reallocate_block(void*, size_t);
inline static void* extend_heap(size_t);
inline static void* first_fit(void*, size_t);
inline static void* best_fit(void*, size_t);
inline static void* find_fit(size_t);
inline static void* place(void*, size_t);
inline static void* coalesce(void*);
inline static void link_block(void*);
inline static void unlink_block(void*);
inline static int seg_index(size_t);

/* ========================== Compilation Flags =============================== */

// #define DEBUG                 /* uncomment to turn-on heap checker */

#ifdef DEBUG
  static void mm_check(int);
  static void check_seglist(int, int);
  static int check_free_list(int, int);
  #define heap_check(line) (mm_check(line))
#else
  #define heap_check(line)
#endif

/* ================================ Macros ===================================== */

#define WSIZE 4                           /* Word size in bytes */
#define DSIZE 8                           /* Double word size in bytes */
#define CHUNKSIZE (1<<8)                  /* Minimum heap allocation size */
#define MIN_BLOCK_SIZE 16                 /* Minimum block size */
#define ALIGNMENT 8                       /* Payload Alignment */
#define SEGLIST_NUM 8                     /* The Number of lists in the seglist */
#define WTYPE u_int32_t                   /* Word type */
#define BYTE char                         /* Byte type */

/* ------------------------------ Basic Utility ------------------------------- */

/* Move the address ptr by offset bytes */
#define MOVE_BYTE(ptr, offset) ((WTYPE *)((BYTE *)(ptr) + (offset)))
/* Move the address ptr by offset words */
#define MOVE_WORD(ptr, offset) ((WTYPE *)(ptr) + (offset))
/* Read a word from address ptr */
#define READ_WORD(ptr) (*(WTYPE *)(ptr))
/* Write a word value to address ptr */
#define WRITE_WORD(ptr, value) (*(WTYPE *)(ptr) = (value))
/* rounds up size (in bytes) to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
/* Get the maximum of x and y */
#define MAX(x, y) (((x) > (y))? (x) : (y))
/* Get the minimum of x and y */
#define MIN(x, y) (((x) < (y))? (x) : (y))

/* ----------------------- Header/Footer Macros ---------------------------- */

/* Pack the size and allocated-bit into a word */
#define PACK(size, alloc) ((size) | (alloc))
/* Read the size from header/footer word at address Hptr */
#define READ_SIZE(Hptr) (READ_WORD(Hptr) & ~0x7)
/* Read the allocated-bit from header/footer word at address Hptr */
#define READ_ALLOC(Hptr) (READ_WORD(Hptr) & 0x1)
/* Write the size and allocation-bit to the word at address Hptr */
#define WRITE(Hptr, size, alloc) (WRITE_WORD((Hptr), PACK((size), (alloc))))
/* Write the size to the word at address Hptr */
#define WRITE_SIZE(Hptr, size) (WRITE((Hptr), (size), READ_ALLOC(Hptr)))
/* Write allocation-bit to the word at address Hptr */
#define WRITE_ALLOC(Hptr, alloc) (WRITE((Hptr), READ_SIZE(Hptr), (alloc)))

/* ---------------------------- Payload Macros ------------------------------ */

/* Get the header-word pointer from the payload pointer pp */
#define HEADER(pp) (MOVE_WORD(pp, -1))
/* Get the footer-word pointer from the payload pointer pp */
#define FOOTER(pp) (MOVE_BYTE(pp, PAYLOAD_SIZE(pp)))
/* Get next block payload pointer from pp (current payload pointer) */
#define NEXT_BLOCK(pp) (MOVE_BYTE(pp, BLOCK_SIZE(pp)))
/* Get previous block payload pointer from pp (current payload pointer) */
#define PREV_BLOCK(pp) (MOVE_BYTE(pp, - READ_SIZE(MOVE_WORD(pp, -2))))
/* Read the block size at the payload pp */
#define BLOCK_SIZE(pp) (READ_SIZE(HEADER(pp)))
/* Read the payload size at pp */
#define PAYLOAD_SIZE(pp) (BLOCK_SIZE(pp) - DSIZE)
/* Check if the block at the payload pp is free */
#define IS_FREE(pp) (!(READ_ALLOC(HEADER(pp))))

/* Sets the size and allocation-bit to header/footer of block at pp */
#define SET_INFO(pp, size, alloc)\
  ((WRITE(HEADER(pp),(size),(alloc))), \
   (WRITE(FOOTER(pp),(size),(alloc))))

/* Sets the size to header/footer of block at pp */
#define SET_SIZE(pp, size)\
  ((WRITE_SIZE(HEADER(pp),(size))), \
   (WRITE_SIZE(FOOTER(pp),(size))))

/* Sets the allocation-bit to header/footer of block at pp */
#define SET_ALLOC(pp, alloc)\
  ((WRITE_ALLOC(HEADER(pp),(alloc))), \
   (WRITE_ALLOC(FOOTER(pp),(alloc))))

/* Get the predecessor payload address */
#define GET_PRED(pp) ((WTYPE *)(READ_WORD(pp)))
/* Get the successor payload address */
#define GET_SUCC(pp) ((WTYPE *)(READ_WORD(MOVE_WORD(pp, 1))))
/* Set the predecessor payload address to pred_pp */
#define SET_PRED(pp, pred_pp) (WRITE_WORD(pp, ((WTYPE) pred_pp)))
/* Set the successor payload address to succ_pp */
#define SET_SUCC(pp, succ_pp) (WRITE_WORD(MOVE_WORD(pp, 1), ((WTYPE) succ_pp)))

/* ======================= Private Global Variables ============================== */

// private global variable
static WTYPE** seglist;       /* array of free-list pointers */

/* =========================== Public Functions ================================== */

/* 
 * Initialize the malloc package.
 * return 0 on success, -1 on error.
 */
int mm_init(void) {
  /* Create the initial empty heap */
  void* heap = mem_sbrk((SEGLIST_NUM + 2) * WSIZE); /* seglist + head + tail */
  if (heap == (void*)-1) return -1;

  seglist = heap;
  heap = MOVE_WORD(heap, SEGLIST_NUM);
  // initailize the seglist
  for(int i = 0; i < SEGLIST_NUM; ++i){
    seglist[i] = NULL;
  }

  WRITE(heap, 0, 1);                          /* Head Word */
  WRITE(MOVE_WORD(heap, 1), 0, 1);            /* Tail Word */

  /* Extend the empty heap with a free block of CHUNKSIZE bytes */
  if (extend_heap(CHUNKSIZE) == NULL) return -1;
  heap_check(__LINE__);
  return 0;
}

/* 
 *  Allocate an aligned block of memory of at least size bytes
 *  Return address of allocated block on success, NULL on error.
 */
void* mm_malloc(size_t size) {
  if (size == 0) return NULL;
  void* pp;                             /* Payload Pointer */
  size = ALIGN(size + DSIZE);           /* Add header and footer words */
  size = MAX(size, MIN_BLOCK_SIZE);

  /* Search the free list for a fit */
  if ((pp = find_fit(size)) == NULL) {
    /* No fit found, request a block from the memory */
    pp = extend_heap(MAX(size, CHUNKSIZE));
    if (pp == NULL) return NULL;
  }

  pp = place(pp, size);
  heap_check(__LINE__);
  return pp;
}

/*
 * Free the allocated block at ptr, and coalesce it.
 */
void mm_free(void* ptr) {
  SET_ALLOC(ptr, 0);
  link_block(ptr);
  coalesce(ptr);
  heap_check(__LINE__);
}

/*
 * # ptr = NULL : allocate block of the given size.
 * # size = 0 : free the given block, return NULL.
 * # else: resize the allocated block at ptr.
 * 
 * Return address of the reallocated block, NULL on error.
 */
void* mm_realloc(void* ptr, size_t size) {
  if (ptr == NULL){
    return mm_malloc(size);
  }else if (size == 0){
    mm_free(ptr);
    return NULL;
  }else{
    ptr = resize_block(ptr, size);
    heap_check(__LINE__);
    return ptr;
  }
}

/* =========================== Private Functions ================================== */

/*
 * Resize the allocated block at pp to have size bytes
 * Return address of the resized block, NULL on error.
 */
static void* resize_block(void* pp, size_t size) {
  size_t asize = MAX(MIN_BLOCK_SIZE, ALIGN(size + DSIZE));
  size_t bsize = BLOCK_SIZE(pp);
  size_t csize = bsize - asize;

  if (asize > bsize) {
    pp = coalesce(pp);
    bsize = BLOCK_SIZE(pp);
    if (asize > bsize) return reallocate_block(pp, size);
    return pp;
  }
  
  // Split only if the fragment is large enough.
  if (csize >= (4 * MIN_BLOCK_SIZE)){
    SET_INFO(pp, asize, 1);
    void* fp = NEXT_BLOCK(pp);
    SET_INFO(fp, csize, 0);
    link_block(fp);
  }

  return pp;
}

/*
 * Allocate block of the given size, copy content, free old block
 * Return address of the new block, NULL on error.
 */
static void* reallocate_block(void* ptr, size_t size) {
  void *newptr = mm_malloc(size);
  if (newptr == NULL) return NULL;
  size_t copy_size = MIN(PAYLOAD_SIZE(ptr), size);
  memcpy(newptr, ptr, copy_size);
  mm_free(ptr);
  return newptr;
}

/**
 * Add free block with aligned size to the end of the heap.
 * Return address of the added free block, NULL on error.
*/
void* extend_heap(size_t size) {
  WTYPE* pp;
  size = ALIGN(size);
  if ((long)(pp = mem_sbrk(size)) == -1) return NULL;

  SET_INFO(pp, size, 0);                      /* Initialize a free block */
  link_block(pp);
  WRITE(HEADER(NEXT_BLOCK(pp)), 0, 1);        /* New Tail Word */

  return pp;
}

/* Find the first block greater than or equal to size
 * Return address of the first-fit, NULL if no-fit.
*/
static void* first_fit(void* free_list, size_t size) {
  for (void* pp = free_list; pp != NULL ; pp = GET_SUCC(pp)) {
    if (size <= BLOCK_SIZE(pp)) return pp;
  }
  return NULL;
}

/* Find the smallest block greater than or equal to size
 * Return address of the best-fit, NULL if no-fit.
*/
static void* best_fit(void* free_list, size_t size) {
  void* pp;
  void* best = NULL;
  size_t best_size = __SIZE_MAX__;

  for (pp = free_list; pp != NULL ; pp = GET_SUCC(pp)) {
    size_t curr_size = BLOCK_SIZE(pp);
    if ((size <= curr_size) && (curr_size < best_size)){
      best = pp;
    }
  }

  return best;
}

/**
 * Find a free block with size greater than or equal to size.
 * Return address of a fit-block, NULL if no fit.
*/
static void* find_fit(size_t size) {
  for(int i = seg_index(size); i < SEGLIST_NUM; ++i){
    void* fit = best_fit(seglist[i], size);
    if (fit != NULL) return fit;
  }
  return NULL;
}

/**
 * Allocate the free block at pp.
 * Split the block if the remainder is greater than MIN_BLOCK_SIZE.
 * Returns the address of the allocated block payload
*/
static void* place(void *pp, size_t size) {
  size_t bsize = BLOCK_SIZE(pp);
  size_t csize = bsize - size;

  unlink_block(pp);
  if (csize < MIN_BLOCK_SIZE){
    SET_ALLOC(pp, 1);
  }else{
    SET_INFO(pp, csize, 0);
    link_block(pp);
    pp = NEXT_BLOCK(pp);
    SET_INFO(pp, size, 1);
  }

  return pp;
}

/**
 * Coalesce the current block with its free previous and/or next blocks.
 * Return the address of the coalesced block.
*/
static void* coalesce(void *pp) {
  void* cpp = pp;                            /* coalesced payload pointer */
  void* prev_footer = MOVE_WORD(pp, -2);
  void* next_header = HEADER(NEXT_BLOCK(pp));
  
  size_t prev_alloc = READ_ALLOC(prev_footer);
  size_t next_alloc = READ_ALLOC(next_header);
  size_t curr_alloc = !IS_FREE(pp);
  size_t size = BLOCK_SIZE(pp);

  if (prev_alloc && next_alloc) return pp;

  if (!curr_alloc) unlink_block(pp);

  if (!next_alloc) {
    size += READ_SIZE(next_header);
    unlink_block(MOVE_WORD(next_header, 1));
  }

  if (!prev_alloc) {
    size += READ_SIZE(prev_footer);
    cpp = PREV_BLOCK(pp);
    unlink_block(cpp);
    if (curr_alloc) memmove(cpp, pp, PAYLOAD_SIZE(pp));
  } 

  SET_INFO(cpp, size, curr_alloc);
  if (!curr_alloc) link_block(cpp);

  return cpp;
}

/**
 * Add the block at pp to the free-list
*/
static void link_block(void* pp){
  int index = seg_index(BLOCK_SIZE(pp));
  WTYPE* list = seglist[index];
  if (list) SET_PRED(list, pp);
  SET_SUCC(pp, list);
  SET_PRED(pp, NULL);
  seglist[index] = pp;
}

/**
 * Remove the block at pp from the free-list 
*/
static void unlink_block(void* pp) {
  int index = seg_index(BLOCK_SIZE(pp));
  WTYPE* pred_pp = GET_PRED(pp);
  WTYPE* succ_pp = GET_SUCC(pp);
  if (pred_pp) SET_SUCC(pred_pp, succ_pp);
  if (succ_pp) SET_PRED(succ_pp, pred_pp);
  if (pp == seglist[index]) seglist[index] = succ_pp;
}

/**
 * Returns the index of the seglist that should contain blocks of the given size 
*/
static int seg_index(size_t size){
  if (size <= MIN_BLOCK_SIZE) return 0;
  if (size <= (2 * MIN_BLOCK_SIZE)) return 1;
  if (size <= (4 * MIN_BLOCK_SIZE)) return 2;
  if (size <= (8 * MIN_BLOCK_SIZE)) return 3;
  if (size <= (16 * MIN_BLOCK_SIZE)) return 4;
  if (size <= (64 * MIN_BLOCK_SIZE)) return 5;
  if (size <= (256 * MIN_BLOCK_SIZE)) return 6;
  return 7;
}

/* ========================== Debugging Functions =============================== */

#ifdef DEBUG
/** 
 * Heap Consistency Checker, checks for:
 * - head and tail words of the heap
 * - block header and footer equality
 * - block size >= minimum size
 * - payload alignment
 * - coalescing (no contiguous free blocks)
 * - total heap size
 * - seglist consistency.
*/
static void mm_check(int line){
  WTYPE* ptr = MOVE_WORD(mem_heap_lo(), SEGLIST_NUM);
  WTYPE* end_ptr = MOVE_BYTE(ptr, mem_heapsize()) - (SEGLIST_NUM + 1); 
  // Check head word (size = 0, allocated)
  if (READ_SIZE(ptr) != 0){
    printf("Error at %d: head-word size = %u\n",line, READ_SIZE(ptr));
  }

  if (READ_ALLOC(ptr) != 1){
    printf("Error at %d: head-word is not allocated\n",line);
  }

  // Check tail word (size = 0, allocated)
  if (READ_SIZE(end_ptr) != 0){
    printf("Error at %d: tail-word size = %u\n",line, READ_SIZE(end_ptr));
  }

  if (READ_ALLOC(end_ptr) != 1){
    printf("Error at %d: tail-word is not allocated\n",line);
  }

  size_t heap_size = (SEGLIST_NUM + 2) * WSIZE;
  int free_count = 0;
  int prev_free = 0;

  // Check regular blocks
  for (ptr = MOVE_WORD(ptr, 2); ptr < end_ptr; ptr = NEXT_BLOCK(ptr)){
    // check header and footer equality
    if (READ_WORD(HEADER(ptr)) != READ_WORD(FOOTER(ptr))){
      printf("Error at %d: at block %p => header = %u, footer = %u\n",
        line, ptr, READ_WORD(HEADER(ptr)), READ_WORD(FOOTER(ptr)));
    }
    // check that block size >= minimum size
    if (BLOCK_SIZE(ptr) < MIN_BLOCK_SIZE){
      printf("Error at %d: block %p has size < min size, (%u < %u)\n",
        line, ptr, BLOCK_SIZE(ptr), MIN_BLOCK_SIZE);
    }
    // check payload alignment
    if((unsigned) ptr % ALIGNMENT){
      printf("Error at %d: block %p is not aligned to %d\n",
        line, ptr, ALIGNMENT);
    }
    // check coalescing.
    if (prev_free && IS_FREE(ptr)){
      printf("NOTE at %d: two contiguous free blocks not coalesced\n", line);
    }

    if(IS_FREE(ptr)) ++free_count;
    prev_free = IS_FREE(ptr);
    heap_size += BLOCK_SIZE(ptr);
  }
  // check total heap size
  if (heap_size != mem_heapsize()){
    printf("Error at %d: heap size not accurate, %u should be %u\n",
     line, heap_size, mem_heapsize());
  }
  // check seglist consistency
  check_seglist(line, free_count);
}

/** 
 * Seglist Consistency Checker, checks for:
 * - seglist pointer
 * - each free-list consistency
 * - the free-block count in seglist matches the free_count (free-blocks in the heap)
*/
static void check_seglist(int line, int free_count){
  int count = 0;
  // checks the seglist pointer
  if (seglist != mem_heap_lo()){
    printf("Error at %d: Seglist pointer doesn't point to heap start address.\n",
      line);
  }
  // check each free-list in the seglist
  for(int i = 0; i < SEGLIST_NUM; ++i){
    count += check_free_list(line, i);
  }
  // check the free-block count in seglist matches the free_count
  if (count != free_count){
    printf("Error at %d: %d missing free-blocks from the seglist.\n",
      line, (free_count - count));
  }
}

/** 
 * Free-List Consistency Checker, checks for:
 * - blocks are free.
 * - free-blocks are in heap range.
 * - the predecessor consistency.
 * Return the number of blocks in the free-list
*/
static int check_free_list(int line, int li){
  void* start_ptr = MOVE_WORD(mem_heap_lo(), SEGLIST_NUM);
  void* end_ptr = MOVE_BYTE(start_ptr, mem_heapsize()) - (SEGLIST_NUM + 1); 
  void* pred_pp = NULL;
  int count = 0;

  for(void* pp = seglist[li]; pp != NULL; pp = GET_SUCC(pp)){
    // check if block is free
    if (!IS_FREE(pp)){
      printf("Error at %d: Seglist[%d] contains an allocated-block %p.\n",
        line, li, pp);
    }
    // check if free-block in heap range
    if (pp <= start_ptr || pp >= end_ptr){
      printf("Error at %d: Seglist[%d] contains a free-block %p out of the heap range.\n",
        line, li, pp);
    }
    // check the predecessor pointer
    if (pred_pp != GET_PRED(pp)){
      printf("Error at %d: in Seglist[%d], inconsistant predecessor link at %p.\n",
        line, li, pp );
    }

    ++count;
    pred_pp = pp;
  }

  return count;
}

#endif
