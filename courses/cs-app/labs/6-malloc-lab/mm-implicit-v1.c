/*
 * mm-implicit-v1.c - implicit free list based malloc package.
 * 
 * Block Format: [Header - Payload - Footer], min size is 16 byte.
 * Header/Footer: 1-word holds size of the block and allocation bit at LSB
 * List Format: implicit free list
 *    [Head-Block[1] | Regular-Blocks ...| Tail-Block[1]]
 * Placement Policy: using first-fit algorithm.
 * Split Policy: always split if remainder is greater than 16 bytes
 * Coalescing Policy: bi-direction coalescing after each call to mm_free
 * 
 * realloc: split if new size is less than the block size by 16 bytes
 *          allocate-copy-free if new size is greater than block size
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
inline static void* expand_block(void*, size_t);
inline static void* extend_heap(size_t);
inline static void* first_fit(size_t);
inline static void* best_fit(size_t);
inline static void* find_fit(size_t);
inline static void place(void*, size_t);
inline static void* coalesce(void*);

/* ========================== Compilation Flags =============================== */

// #define DEBUG                 /* uncomment to turn-on heap checker */

#ifdef DEBUG
  static void mm_check(int line);
  #define heap_check(line) (mm_check(line))
#else
  #define heap_check(line)
#endif

/* ================================ Macros ===================================== */

#define WSIZE 4                           /* Word size in bytes */
#define DSIZE 8                           /* Double word size in bytes */
#define CHUNKSIZE (1<<12)                 /* Minimum heap allocation size */
#define MIN_BLOCK_SIZE 16                 /* Minimum block size */
#define ALIGNMENT 8                       /* Payload Alignment */
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
#define MAX(x, y) ((x) > (y)? (x) : (y))
/* Get the minimum of x and y */
#define MIN(x, y) ((x) < (y)? (x) : (y))

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
#define FOOTER(pp) (MOVE_BYTE(pp, (BLOCK_SIZE(pp) - DSIZE)))
/* Get next block payload pointer from pp (current payload pointer) */
#define NEXT_BLOCK(pp) (MOVE_BYTE(pp, BLOCK_SIZE(pp)))
/* Get previous block payload pointer from pp (current payload pointer) */
#define PREV_BLOCK(pp) (MOVE_BYTE(pp, - READ_SIZE(MOVE_WORD(pp, -2))))
/* Read the block size at the payload pp */
#define BLOCK_SIZE(pp) (READ_SIZE(HEADER(pp)))
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

/* ======================= Private Global Variables ============================== */

// private global variable
static WTYPE* heap_listp;     /* pointer to first block payload */

/* =========================== Public Functions ================================== */

/* 
 * Initialize the malloc package.
 * return 0 on success, -1 on error.
 */
int mm_init(void) {
  /* Create the initial empty heap */
  if ((heap_listp = mem_sbrk(DSIZE)) == (WTYPE*)-1) return -1;

  WRITE(heap_listp, 0, 1);                    /* Head Word */
  WRITE(heap_listp + 1, 0, 1);                /* Tail Word */

  heap_listp += 2;

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

  /* Search the free list for a fit */
  if ((pp = find_fit(size)) == NULL) {
    /* No fit found, request a block from the memory */
    pp = extend_heap(MAX(size, CHUNKSIZE));
    if (pp == NULL) return NULL;
  }

  place(pp, size);
  heap_check(__LINE__);
  return pp;
}

/*
 * Free the allocated block at ptr, and coalesce it.
 */
void mm_free(void* ptr) {
  SET_ALLOC(ptr, 0);
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
    return resize_block(ptr, size);
  }
}

/* =========================== Private Functions ================================== */

/*
 * Resize the allocated block at pp to have size bytes
 * Return address of the resized block, NULL on error.
 */
static void* resize_block(void* pp, size_t size) {
  size_t bsize = BLOCK_SIZE(pp);
  size = ALIGN(size + DSIZE);                 /* Add header and footer words */

  if (size > bsize) return expand_block(pp, size);

  if (bsize >= (size + MIN_BLOCK_SIZE)){
    SET_INFO(pp, size, 1);
    pp = NEXT_BLOCK(pp);
    SET_INFO(pp, bsize-size, 0);
  }

  heap_check(__LINE__);
  return pp;
}

/*
 * Allocate block of the given size, copy content, free old block
 * Return address of the new block, NULL on error.
 */
static void* expand_block(void* ptr, size_t size) {
  void *newptr = mm_malloc(size);
  if (newptr == NULL) return NULL;
  size_t copy_size = MIN(BLOCK_SIZE(ptr), size);
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

  SET_INFO(pp, size, 0);                     /* Initialize a free block */
  WRITE(HEADER(NEXT_BLOCK(pp)), 0, 1);        /* New Tail Word */

  if (IS_FREE(PREV_BLOCK(pp))) return coalesce(pp);
  return pp;
}

/* Find the first block greater than or equal to size
 * Return address of the first-fit, NULL if no-fit.
*/
static void* first_fit(size_t size) {
  void* pp;

  for (pp = heap_listp; BLOCK_SIZE(pp) > 0; pp = NEXT_BLOCK(pp)) {
    if (IS_FREE(pp) && (size <= BLOCK_SIZE(pp))) return pp;
  }

  return NULL;
}

/* Find the smallest block greater than or equal to size
 * Return address of the best-fit, NULL if no-fit.
*/
static void* best_fit(size_t size) {
  void* pp;
  void* best = NULL;
  size_t best_size = __SIZE_MAX__;

  for (pp = heap_listp; BLOCK_SIZE(pp) > 0; pp = NEXT_BLOCK(pp)) {
    size_t curr_size = BLOCK_SIZE(pp);
    if (IS_FREE(pp) && (size <= curr_size) && (curr_size < best_size)){
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
  return first_fit(size);
}

/**
 * Allocate the free block at pp.
 * Split the block if the remainder is greater than MIN_BLOCK_SIZE.
*/
static void place(void *pp, size_t size) {
  size_t bsize = BLOCK_SIZE(pp);

  if (bsize < (size + MIN_BLOCK_SIZE)){
    SET_ALLOC(pp, 1);
  }else{
    SET_INFO(pp, size, 1);
    pp = NEXT_BLOCK(pp);
    SET_INFO(pp, bsize-size, 0);
  }
}

/**
 * Coalesce the current block with its free previous and/or next blocks.
 * Return the address of the coalesced free-block.
*/
static void* coalesce(void *pp) {
  void* prev_footer = MOVE_WORD(pp, -2);
  void* next_header = HEADER(NEXT_BLOCK(pp));
  size_t prev_alloc = READ_ALLOC(prev_footer);
  size_t next_alloc = READ_ALLOC(next_header);
  size_t size = BLOCK_SIZE(pp);

  if (prev_alloc && next_alloc) return pp;
  
  if (!next_alloc) size += READ_SIZE(next_header);

  if (!prev_alloc) {
    size += READ_SIZE(prev_footer);
    pp = PREV_BLOCK(pp);
  } 

  SET_INFO(pp, size, 0);
  return pp;
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
*/
static void mm_check(int line){
  WTYPE* ptr = mem_heap_lo();
  WTYPE* end_ptr = MOVE_BYTE(ptr, mem_heapsize()) - 1;
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

  size_t heap_size = DSIZE;
  int prev_free = 0;

  // Check regular blocks
  for (ptr = heap_listp; ptr < end_ptr; ptr = NEXT_BLOCK(ptr)){
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
      printf("Error at %d: two contiguous free blocks not coalesced\n", line);
    }

    prev_free = IS_FREE(ptr);
    heap_size += BLOCK_SIZE(ptr);
  }
  // check total heap size
  if (heap_size != mem_heapsize()){
    printf("Error at %d: heap size not accurate, %u should be %u\n",
     line, heap_size, mem_heapsize());
  }
}

#endif
