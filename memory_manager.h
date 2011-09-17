#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "lock.h"
#include "process.h"

#define PG_XLAT(dir,page,offs) ((((dir)&0x3FF)<<22)|(((page)&0x3FF)<<12)|((offs)&0xFFF))
#define PG_DIR(addr) (((addr)&0xFFC00000)>>22)
#define PG_PAGE(addr) (((addr)&0x3FF000)>>12)
#define PG_OFFS(addr) ((addr)&0xFFF)
#define TOTAL_PAGE_COUNT 1048576  // 2^20 = 2^(32-12) where 4096 = 12 bits
#define PAGE_SIZE 4096

#define PAGE_COUNT 1024
#define PAGE_TABLE_COUNT 1024

#define MEMORY_START 0x800000
#define LOAD_BASE 0x40000000


enum {
    PAGE_FREE, 
    PAGE_IN_USE
};

typedef struct page_table_struct {
    unsigned int page[PAGE_COUNT];
} page_table_t;

void memory_manager_start();
void memory_manager_clear_page_table( page_table_t *pt );
void *memory_manager_allocate_page();
void *memory_manager_allocate_process( int *dir, int pages_needed );

extern unsigned int memory_end;
extern void *kernel_end;
extern page_table_t **page_dir;

#endif
