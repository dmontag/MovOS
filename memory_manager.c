
#include "memory_manager.h"
#include "scheduler.h"
#include "std.h"
#include "bitmap.h"

lock_t memory_manager_lock = LOCK_UNLOCKED;

void *memory_start = (void *)MEMORY_START;
unsigned int total_memory = 0;
bitmap_t page_map;
int num_pages;

// Initialize memory manager stuff.
void memory_manager_start() {
    int i;
    
    // Check how much memory we have.
    total_memory = ( (unsigned int)memory_end - (unsigned int)memory_start - 
        0x1000 ) & 0xFFFFF000;
    
    num_pages = total_memory / PAGE_SIZE;
    
    // Set up map of pages. Initialize all pages to free.
    page_map.map = (char *)&kernel_end;
    bitmap_zero( &page_map, TOTAL_PAGE_COUNT );
    
    // Reserve OS pages in page map.
    for ( i = 0; i < ( (unsigned int)memory_start / PAGE_SIZE ); i++ ) {
        bitmap_set( &page_map, i );
    }
    
}

// Returns pointer to page allocated for page directory.
void *memory_manager_initialize_process() {
    return memory_manager_allocate_page();
}

// Not finished. Problems with pages not being mapped to a page directory 
// when being written to. Consider using a bootup function running as the 
// process to map in everything.
void *memory_manager_allocate_process( int *dir, int pages_needed ) {

    int *page_directory = (int *)memory_manager_allocate_page();
    int i, pos_in_dir, pages_left_in_table;
    void *page;
    int *table;
    int *offs = (int *)LOAD_BASE;
    //__asm__("cli;hlt;");
    pos_in_dir = 0;
    pages_left_in_table = 0;
    
    for ( i = 0; i < pages_needed; i++ ) {
        *(char *)(0xB8000) += 1;
        if ( pages_left_in_table == 0 ) {
            // Allocate a page table.
            table = (int *)memory_manager_allocate_page();
            pages_left_in_table = PAGE_COUNT;
            // Reference it from the page directory.
            page_directory[pos_in_dir++] = ( (int)table & 0xFFFFF000 ) | 3;
        }
        
        page = memory_manager_allocate_page();
        print_int( (int)page, 0xFF, 0xFF, 21 );
        print( " ", 0xFF, 0xFF, 0 );
        table[pages_left_in_table--] = ( (int)page & 0xFFFFF000 ) | 3;
    }
    
    // Map in kernel. (should be replaced with interrupts)
    //for ( i = 0; i < ( (unsigned int)memory_start / PAGE_SIZE ); i++ ) {
    //    page_directory[
    //}
    
    return (void *)page_directory;

}

// Allocates a full directory entry worth of memory (4M), and sets up the 
// page table for the chunk at the beginning of it.
void memory_manager_allocate_chunk_at( void *virt_addr ) {
    
    int i;
    void *page;
    void *base;
    
    base = memory_manager_allocate_page();
    
    for ( i = 0; i < PAGE_COUNT-1; i++ ) {
        
        page = memory_manager_allocate_page();
        
        
        
        
    }
    
}

// Allocate a page.
void *memory_manager_allocate_page() {
    
    int page_index;
    void *page;
    
    do_lock( &memory_manager_lock );
    
    page_index = bitmap_find_free( &page_map, TOTAL_PAGE_COUNT );
    bitmap_set( &page_map, page_index );
    
    page = (void *)( page_index * PAGE_SIZE );
    
    do_unlock( &memory_manager_lock );
    
    return page;
    
}

// Deallocate a page.
void memory_manager_deallocate_page( void *page ) {
    
    int page_index;
    
    do_lock( &memory_manager_lock );
    
    page_index = (unsigned int)page / PAGE_SIZE;
    bitmap_unset( &page_map, page_index );
    
    do_unlock( &memory_manager_lock );
    
}

// Count main memory
int check_mem() {

	int *ptr;
	int pos = 0;
	
	ptr = (int *)memory_start;
	
	print( "Checking available memory: -", 0xFF, 0xFF, 0x0F );
	
	while ( 1 ) {

        //print_move_cursor( 0, -1 );
		switch ( pos ) {
            case 0:
                print( "/", 0xFF, 0xFF, 0x0F ); break;
            case 1:
                print( "|", 0xFF, 0xFF, 0x0F ); break;
            case 2: 
                print( "\\", 0xFF, 0xFF, 0x0F ); break;
            case 3:
                print( "-", 0xFF, 0xFF, 0x0F ); pos = 0; break;
        }
        pos++;

		*ptr = 0xdeadbeef;
		if ( *ptr != 0xdeadbeef ) break;
		ptr += 0x1000;
		
	}
	
    print_move_cursor( 0, -1 );
	print( "done\n", 0xFF, 0xFF, 0x0F );
	
	return (int)ptr;
	
}
