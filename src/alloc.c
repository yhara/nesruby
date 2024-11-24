//#include "alloc.h"
//#include "debug.h"
//#include "shims.h"
//
//static void on_oom()
//{
//  put_str(NTADR_A(0,0),"OOM");
//}
//#define MRBC_OUT_OF_MEMORY on_oom
//
//void mrbc_init_alloc(void *ptr, unsigned int size)
//
//#define MRBC_ALLOC_FLI_BIT_WIDTH 9	// ~~~~~~~~~~~
//
//#define MRBC_ALLOC_MEMSIZE_T	uint16_t
//
//typedef struct USED_BLOCK {
//  MRBC_ALLOC_MEMSIZE_T size;		//!< block size, header included
//} USED_BLOCK;
//
//typedef struct FREE_BLOCK {
//  MRBC_ALLOC_MEMSIZE_T size;		//!< block size, header included
//  struct FREE_BLOCK *next_free;
//  struct FREE_BLOCK *prev_free;
//  struct FREE_BLOCK *top_adrs;		//!< dummy for calculate sizeof(FREE_BLOCK)
//} FREE_BLOCK;
//
//typedef struct MEMORY_POOL {
//  MRBC_ALLOC_MEMSIZE_T size;
//
//  // free memory bitmap
//  uint16_t free_fli_bitmap;
//  uint8_t  free_sli_bitmap[MRBC_ALLOC_FLI_BIT_WIDTH +1+1];
//						// +1=bit_width, +1=sentinel
//  uint8_t  pad[3]; // for alignment compatibility on 16bit and 32bit machines
//
//  // free memory block index
//  FREE_BLOCK *free_blocks[SIZE_FREE_BLOCKS +1];	// +1=sentinel
//} MEMORY_POOL;
//
//void * mrbc_raw_alloc(unsigned int size)
//{
//  MEMORY_POOL *pool = memory_pool;
//  MRBC_ALLOC_MEMSIZE_T alloc_size = size + sizeof(USED_BLOCK);
//
//  // align 4 byte
//  alloc_size += (-alloc_size & 3);
//
//  // check minimum alloc size.
//  if( alloc_size < MRBC_MIN_MEMORY_BLOCK_SIZE ) alloc_size = MRBC_MIN_MEMORY_BLOCK_SIZE;
//
//  FREE_BLOCK *target;
//  unsigned int fli, sli;
//  unsigned int index = calc_index(alloc_size);
//
//  // At first, check only the beginning of the same size block.
//  // because it immediately responds to the pattern in which
//  // same size memory are allocated and released continuously.
//  target = pool->free_blocks[index];
//  if( target && BLOCK_SIZE(target) >= alloc_size ) {
//    fli = FLI(index);
//    sli = SLI(index);
//    goto FOUND_TARGET_BLOCK;
//  }
//
//  // and then, check the next (larger) size block.
//  target = pool->free_blocks[++index];
//  fli = FLI(index);
//  sli = SLI(index);
//  if( target ) goto FOUND_TARGET_BLOCK;
//
//  // check in SLI bitmap table.
//  uint16_t masked = pool->free_sli_bitmap[fli] & ((MSB_BIT1_SLI >> sli) - 1);
//  if( masked != 0 ) {
//    sli = NLZ_SLI( masked );
//    goto FOUND_FLI_SLI;
//  }
//
//  // check in FLI bitmap table.
//  masked = pool->free_fli_bitmap & ((MSB_BIT1_FLI >> fli) - 1);
//  if( masked != 0 ) {
//    fli = NLZ_FLI( masked );
//    sli = NLZ_SLI( pool->free_sli_bitmap[fli] );
//    goto FOUND_FLI_SLI;
//  }
//
//  // Change strategy to First-fit.
//  target = pool->free_blocks[--index];
//  while( target ) {
//    if( BLOCK_SIZE(target) >= alloc_size ) {
//      remove_free_block( pool, target );
//      goto SPLIT_BLOCK;
//    }
//    target = target->next_free;
//  }
//
//  // else out of memory
//  MRBC_OUT_OF_MEMORY();
//  return NULL;  // ENOMEM
//
//
// FOUND_FLI_SLI:
//  index = (fli << MRBC_ALLOC_SLI_BIT_WIDTH) + sli;
//  assert( index < SIZE_FREE_BLOCKS );
//  target = pool->free_blocks[index];
//  assert( target != NULL );
//
// FOUND_TARGET_BLOCK:
//  assert(BLOCK_SIZE(target) >= alloc_size);
//
//  // remove free_blocks index
//  pool->free_blocks[index] = target->next_free;
//  if( target->next_free == NULL ) {
//    pool->free_sli_bitmap[fli] &= ~(MSB_BIT1_SLI >> sli);
//    if( pool->free_sli_bitmap[fli] == 0 ) pool->free_fli_bitmap &= ~(MSB_BIT1_FLI >> fli);
//  }
//  else {
//    target->next_free->prev_free = NULL;
//  }
//
// SPLIT_BLOCK: {
//    FREE_BLOCK *release = split_block(target, alloc_size);
//    if( release != NULL ) {
//      SET_PREV_USED(release);
//      add_free_block( pool, release );
//    } else {
//      FREE_BLOCK *next = PHYS_NEXT(target);
//      SET_PREV_USED(next);
//    }
//  }
//
//  SET_USED_BLOCK(target);
//
//  return (uint8_t *)target + sizeof(USED_BLOCK);
//}
//
//void * mrbc_alloc(const struct VM *vm, unsigned int size)
//{
//  void *ptr = mrbc_raw_alloc(size);
//  if( ptr == NULL ) return NULL;	// ENOMEM
//
//  return ptr;
//}