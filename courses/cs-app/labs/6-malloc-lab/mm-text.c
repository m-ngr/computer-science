/*
 * mm-text.c - textbook CS:APPe3 implicit free list example malloc package.
 * 
 * Perf index = 44 (util) + 11 (thru) = 55/100
 * 
 * Block Format: [Header - Payload - Footer], min size is 16 byte.
 * Header/Footer: 1-word holds size of the block and allocation bit at LSB
 * List Format: implicit free list
 *    [Empty-Block[1] | Head-Block[2] | Regular-Blocks ...| Tail-Block[1]]
 * Placement Policy: using first-fit algorithm.
 * Split Policy: always split if remainder is greater than 16 bytes
 * Coalescing Policy: bi-direction coalescing after each call to mm_free
 * 
 * realloc: simple inefficient version implemented by allocating new-block,
 *      copy data from old-block to the new-block, then free the old-block
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
  /* Team name */
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

/* private globals */
static void* heap_listp; /* a pointer to heap-list header node */
static void* extend_heap(size_t words);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);
static void* coalesce(void *bp);
static void* resize_block(void *ptr, size_t size);

/* constants and macros */ 

#define WSIZE 4           /* Word and header/footer size (bytes) */
#define DSIZE 8           /* Double word size (bytes) */
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */

/* get the maximum of x and y */
#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read a word from address p */
#define GET(p) (*(unsigned int *)(p))
/* write a word to address p */
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
/* Read the allocated bit from address p */
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header */
#define HDRP(bp) ((char *)(bp) - WSIZE)
/* Given block ptr bp, compute address of its footer */
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next block */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
/* Given block ptr bp, compute address of previous block */
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* 
 * Initialize the malloc package.
 * return 0 on success, -1 on error.
 */
int mm_init(void) {
  /* Create the initial empty heap */
  if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1) return -1;

  PUT(heap_listp, 0);                           /* Alignment padding */
  PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1));  /* Prologue header */
  PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1));  /* Prologue footer */
  PUT(heap_listp + (3*WSIZE), PACK(0, 1));      /* Epilogue header */

  heap_listp += (2*WSIZE);

  /* Extend the empty heap with a free block of CHUNKSIZE bytes */
  if (extend_heap(CHUNKSIZE/WSIZE) == NULL) return -1;

  return 0;
}

/* 
 *  Allocate an aligned block of memory
 *  Return address of allocated block on success, NULL on error.
 */
void* mm_malloc(size_t size) {
  size_t asize;       /* Adjusted block size */
  size_t extendsize;  /* Amount to extend heap if no fit */
  char *bp;

  /* Ignore spurious requests */
  if (size == 0) return NULL;

  /* Adjust block size to include overhead and alignment reqs. */
  if (size <= DSIZE) {
    asize = 2*DSIZE;
  } else {  
    asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);
  }

  /* Search the free list for a fit */
  if ((bp = find_fit(asize)) != NULL) {
    place(bp, asize);
    return bp;
  }

  /* No fit found. Get more memory and place the block */
  extendsize = MAX(asize,CHUNKSIZE);
  if ((bp = extend_heap(extendsize/WSIZE)) == NULL) return NULL;
  place(bp, asize);
  return bp;
}

/*
 * Free the allocated block at ptr, and coalesce it.
 */
void mm_free(void *ptr) {
  size_t size = GET_SIZE(HDRP(ptr));
  PUT(HDRP(ptr), PACK(size, 0));
  PUT(FTRP(ptr), PACK(size, 0));
  coalesce(ptr);
}

/*
 * # ptr = NULL : allocate block of the given size.
 * # size = 0 : free the given block, return NULL.
 * # else: resize the given block
 * Return address of new block, NULL if no new block is allocated.
 */
void* mm_realloc(void *ptr, size_t size) {
  if (ptr == NULL){
    return mm_malloc(size);
  }else if (size == 0){
    mm_free(ptr);
    return NULL;
  }else{
    return resize_block(ptr, size);
  }
}

/*
 * resize the allocated block at ptr to size (Allocate-Copy-Free)
 * Return address of the resized block, NULL on error.
 */
static void* resize_block(void *ptr, size_t size) {
  void *oldptr = ptr;
  void *newptr;
  size_t copySize;

  newptr = mm_malloc(size);
  if (newptr == NULL) return NULL;

  copySize = GET_SIZE(HDRP(oldptr));
  if (size < copySize) copySize = size;
  memcpy(newptr, oldptr, copySize);
  mm_free(oldptr);
  return newptr;
}

/**
 * Add free block with even words size to the end of the heap.
 * Return address of the free block, NULL on error.
*/
static void* extend_heap(size_t words) {
  char *bp;
  size_t size;

  /* Allocate an even number of words to maintain alignment */
  size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
  if ((long)(bp = mem_sbrk(size)) == -1) return NULL;

  /* Initialize free block header/footer and the epilogue header */
  PUT(HDRP(bp), PACK(size, 0));         /* Free block header */
  PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */

  /* Coalesce if the previous block was free */
  return coalesce(bp);
}


/**
 * Find a free block with size greater than or equal to asize.
 * Return address of a fit-block, NULL if no fit.
*/
static void* find_fit(size_t asize) {
  /* First-fit search */
  void *bp;

  for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
    if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
      return bp;
    }
  }

  return NULL; /* No fit */
}

/**
 * Convert the free block at bp to an allocated block.
 * Split the block if the remainder is greater than 16 bytes.
*/
static void place(void *bp, size_t asize) {
  size_t csize = GET_SIZE(HDRP(bp));

  if ((csize - asize) >= (2*DSIZE)) {
    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
    bp = NEXT_BLKP(bp);
    PUT(HDRP(bp), PACK(csize-asize, 0));
    PUT(FTRP(bp), PACK(csize-asize, 0));
  } else {
    PUT(HDRP(bp), PACK(csize, 1));
    PUT(FTRP(bp), PACK(csize, 1));
  }
}

/**
 * coalesce the current block with its free previous and/or next blocks.
 * Return the address of the coalesced free block.
*/
static void* coalesce(void *bp) {
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));

  if (prev_alloc && next_alloc) {             /* Case 1 */
    return bp;
  } 
  
  else if (prev_alloc && !next_alloc) {       /* Case 2 */
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size,0));
  } 
  
  else if (!prev_alloc && next_alloc) {       /* Case 3 */
    size += GET_SIZE(HDRP(PREV_BLKP(bp)));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
  } 
  
  else {                                      /* Case 4 */
    size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
    GET_SIZE(FTRP(NEXT_BLKP(bp)));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
    bp = PREV_BLKP(bp);
  }

  return bp;
}
