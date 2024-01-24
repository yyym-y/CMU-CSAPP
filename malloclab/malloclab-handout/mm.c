/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
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
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* ------------------ my define ------------------ */
#define TSIZE 4 /* head and foot size (bytes) */
#define CHUNKSIZE (1 << 12) /* (bytes) */
/* Max function */
#define MAX(x, y) ((x) > (y)) ? (x) : (y)
/* head or foot content */
#define PACK(size, alloc) ((size) | (alloc))
/* read and write at address p*/
#define PUT(p, val) (*((unsigned int *)(p)) = val)
#define GET(p) (*((unsigned int*)(p)))
/* get bp size or alloc */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
/* get bp head and foot */
#define GET_HEAD(bp) ((char*)(bp - GET_SIZE(bp) + TSIZE)) // foot -> head
#define GET_FOOT(bp) ((char*)(bp + GET_SIZE(bp) - TSIZE)) // head -> foot
/* get next or prev bq*/
#define GET_PREV(bp) ((char*)(GET_HEAD(bp) - TSIZE)) // foot -> prev foot
#define GET_NEXT(bp) ((char*)(GET_FOOT(bp) + TSIZE)) // head -> next head
/* back <-> front */
#define B2F(bp) ((char*)(bp - TSIZE)) // back -> front
#define F2B(bp) ((char*)(bp + TSIZE)) // front -> back

/* ------------------ my function ------------------ */
static void* extend_heap(size_t bytes);
static void* coalesce(void* bp);
static void* find_fit(size_t size);
static void place(void* bp, size_t size);
static void printLink(void);
static void printTag(void* p, char* str);
static void mm_set(void* old_ptr, void* new_ptr, size_t old_size, size_t new_size);


static void* heap_listp;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    mem_init();
    if((heap_listp = (mem_sbrk(4 * TSIZE))) == (void *)-1) {
        fprintf(stderr, "ERROR: mm_init failed. mem_sbrk failed...\n");
        return -1;
    }
    //printf("\nheap_listp : %x\n", heap_listp);
    PUT(heap_listp, 0); /* init alignment padding */
    PUT(heap_listp + TSIZE, PACK(2 * TSIZE, 1)); /* init prologue header */
    //printf("+1SIZE : %d --\n", GET_SIZE(heap_listp + TSIZE));
    PUT(heap_listp + 2 * TSIZE, PACK(2 * TSIZE, 1)); /* init prologue footer */
    //printf("+2SIZE : %d --\n", GET_SIZE(heap_listp + 2 * TSIZE));
    PUT(heap_listp + 3 * TSIZE, PACK(0, 1)); /* init epilogue header */
    //printf("+3SIZE : %d --\n", GET_SIZE(heap_listp + 3 * TSIZE));
    /* move heap_listp to real cell */
    //printf("real ind : %x ---+\n", heap_listp + TSIZE);
    heap_listp += 2 * TSIZE;
    
    //printf("\nheap_listp -> size : %d, alloc %d----", GET_SIZE(B2F(heap_listp)), GET_ALLOC(B2F(heap_listp)));
    if(extend_heap(CHUNKSIZE) == NULL) {
        fprintf(stderr, "ERROR: mm_init failed. extend_heap failed...\n");
        return -1;
    }
    //printf("\nheap_listp -> size : %d, alloc %d----", GET_SIZE(B2F(heap_listp)), GET_ALLOC(B2F(heap_listp)));
    //printf("\n-----%x----\n", heap_listp);
    //printf("\n-----%x----\n", mem_heap_lo());
    //printf("\n-----%x----\n", mem_heap_hi());
    return 0;
}

static void* extend_heap(size_t bytes) {
    if(bytes < 0) return NULL;
    if(bytes < 8) bytes = 8;
    bytes = bytes % 8 ? (bytes / 8 + 1) * 8 : bytes;
    void* begin;
    if((begin = mem_sbrk(bytes)) == (void*)-1) {
        fprintf(stderr, "ERROR: extend_heap. mem_sbrk failed...\n");
        return NULL;
    }
    PUT(B2F(begin), PACK(bytes, 0));
    PUT(GET_FOOT(B2F(begin)), PACK(bytes, 0));
    PUT(GET_NEXT(B2F(begin)), PACK(0, 1));
    //printf("extend_heap end ~~\n");
    //printLink();
    void * tem = coalesce(begin);
    //printf("coalesce end ~\n");
    //printLink();
    return tem;
}

static void* coalesce(void* bp) {
    //printf("coalesce : %x--\n", bp);
    size_t prv_alloc = GET_ALLOC(B2F(B2F(bp)));
    size_t next_alloc = GET_ALLOC(GET_NEXT(B2F(bp)));
    size_t size = GET_SIZE(B2F(bp));

    //printTag(B2F(B2F(bp)), "pre_foot : ");
    //printTag(GET_HEAD(B2F(B2F(bp))), "pre_head ");
    //printTag(GET_NEXT(B2F(bp)), "next_head");
    
    if(prv_alloc && next_alloc) {
        //printf("0\n");
        return bp;
    }if(prv_alloc && !next_alloc) {
        //printf("1\n");
        size += GET_SIZE(GET_NEXT(B2F(bp)));
        PUT(B2F(bp), PACK(size, 0));
        PUT(GET_FOOT(B2F(bp)), PACK(size, 0));
        return bp;
    }
    if(!prv_alloc && next_alloc) {
        //printf("2\n");
        bp = GET_HEAD(B2F(B2F(bp)));
        //printf("tem0 : %x\n", bp);
        size += GET_SIZE(bp);
        PUT(bp, PACK(size, 0));
        PUT(GET_FOOT(bp), PACK(size, 0));
        return F2B(bp);
    }
    //printf("3\n");
    size += GET_SIZE(GET_NEXT(B2F(bp)));
    bp = GET_HEAD(B2F(B2F(bp)));
    size += GET_SIZE(bp);
    PUT(bp, PACK(size, 0));
    PUT(GET_FOOT(bp), PACK(size, 0));
    return F2B(bp);
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    //printf("malloc begin~~~ \n");
    //printLink();
    //printf("ori_size = %d---\n", size);
    if(size <= 0) return NULL;
    if(size < 8) size = 8;
    
    size = size % 8 ? (size / 8 + 1) * 8 : size;
    //printf("change_size = %d ---\n", size);
    char* bp;
    //printf("need ptr : %x---\n", F2B(heap_listp));
    //printf("size : %d, alloc %d --\n", GET_SIZE(F2B(heap_listp)), GET_ALLOC(F2B(heap_listp)));
    if((bp = find_fit(size)) != NULL) {
        place(bp, size);
        //printf("\n-----%x-----~1\n", bp);
        //printf("malloc end ~\n");
        //printLink();
        return bp;
    }
    size_t extend_size = MAX(size, CHUNKSIZE);
    //printf("extend_size : %d\n", extend_size);
    if((bp = extend_heap(extend_size)) == NULL) 
        return NULL;
    return mm_malloc(size);
    //place(bp, size);
    //printf("\n-----%x-----~2\n", bp);
    //printf("malloc end ~\n");
    //printLink();
    //return bp;
}

static void* find_fit(size_t size) {
    void* ptr = B2F(heap_listp);
    //printf("ptr : %x\n", ptr);
    //printf("search-begin -> size : %d, alloc %d----\n", GET_SIZE(ptr), GET_ALLOC(ptr));
    //printf("size : %d -\n", size);
    int i = 0;
    while(!(GET_SIZE(ptr) == 0 && GET_ALLOC(ptr) == 1)) {
        if(GET_SIZE(ptr) == size + 8 && GET_ALLOC(ptr) != 1) break;
        if(GET_SIZE(ptr) >= size + 16 && GET_ALLOC(ptr) != 1) break;
        ptr = GET_NEXT(ptr); i  ++;
        //if(i == 1000) break;
        //printf("%x-- %d %d\n", ptr, GET_SIZE(ptr), GET_ALLOC(ptr));
    }
   // printf("find ptr %x---\n", ptr);
    if((GET_SIZE(ptr) == 0 && GET_ALLOC(ptr) == 1))
        return NULL;
    return F2B(ptr);
}

static void place(void* bp, size_t size) {
    size_t ori_size = GET_SIZE(B2F(bp));
    if(ori_size == size + 8) {
        PUT(B2F(bp), PACK(ori_size, 1));
        PUT(GET_FOOT(B2F(bp)), PACK(ori_size, 1));
        return ;
    }
    size_t leave_size = ori_size - size - 8;
    PUT(GET_FOOT(B2F(bp)), PACK(leave_size, 0));
    PUT(B2F(bp), PACK(size + 8, 1));
    PUT(GET_FOOT(B2F(bp)), PACK(size + 8, 1));
    PUT(F2B(GET_FOOT(B2F(bp))), PACK(leave_size, 0));
    return;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    //printf("free %x %x\n", ptr, B2F(ptr));
    size_t size = GET_SIZE(B2F(ptr));
    PUT(B2F(ptr), PACK(size, 0));
    PUT(GET_FOOT(B2F(ptr)), PACK(size, 0));
    //printLink();
    //printf("----\n");
    coalesce(ptr);
    //printf("free~\n");
    //printLink();
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    if(ptr == NULL) 
        return mm_malloc(size);
    if(size == 0) {
        mm_free(ptr); return NULL;
    }
    size = size % 8 ? (size / 8 + 1) * 8 : size;
    if(size + 2 * TSIZE == GET_SIZE(B2F(ptr)))
        return ptr;
    
    void * new_ptr = mm_malloc(size);
    mm_set(ptr, new_ptr, GET_SIZE(ptr) - 2 * TSIZE, size);
    mm_free(ptr);
    return new_ptr;   
}

static void mm_set(void* old_ptr, void* new_ptr, size_t old_size, size_t new_size) {
    int i;
    for(i = 0 ; i < new_size && i < old_size ; i ++) {
        *((char*)(new_ptr + i)) = *((char*)(old_ptr + i));
    }
}

static void printLink(void) {
    void* ptr = B2F(heap_listp);
    int i = 0;
    while(!(GET_SIZE(ptr) == 0 && GET_ALLOC(ptr) == 1)) {
        printf("---------> %x : Size : %7d, Alloc : %d\n",
                 ptr, GET_SIZE(ptr), GET_ALLOC(ptr));
        void* foot = GET_FOOT(ptr);
        if(GET_ALLOC(foot) != GET_ALLOC(ptr) || GET_SIZE(foot) != GET_SIZE(ptr)) {
            printf("ERROR : HEAD != FOOT\n");
            printf("head : %d : %d \t foot : %d : %d", GET_SIZE(ptr), GET_ALLOC(ptr), GET_SIZE(foot), GET_ALLOC(foot));
            assert(0);
        }
        ptr = GET_NEXT(ptr);
        i ++;
        if(i == 700) {
            printf("ERROR \n"); break;
        }
    }
}

static void printTag(void* p, char* str) {
    printf("%s : %x, Size : %d, Alloc %d \n", str, p, GET_SIZE(p), GET_ALLOC(p));
    return ;
}










