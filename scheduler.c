
#include "scheduler.h"
#include "process.h"
#include "switch.h"
#include "lock.h"
#include "std.h"

lock_t scheduler_lock = LOCK_UNLOCKED;
int current_process_index = -1;
int processes_loaded = 0;
process_t process_list[SCHEDULER_MAX_PROCESSES];

// Takes initial process parameters as arguments.
void scheduler_start( void (*f)(void), void *stack ) {
    
    int main_id;
    int i;
    
    for ( i = 0; i < SCHEDULER_MAX_PROCESSES; i++ ) {
        scheduler_clear_process( i );
    }
    
    scheduler_create_process( f, stack );
    
    __asm__( "sti" );
    
}

void scheduler_tick() {
    
    int next_process_index, i;
    process_t *current;
    process_t *next;
    process_t temp;
    
    if ( current_process_index != -1 ) {
        
        // Find next thread that is ready to run.
        for ( 
            next_process_index = 
                ( current_process_index + 1 ) % SCHEDULER_MAX_PROCESSES;
            process_list[next_process_index].status != PROCESS_STATUS_READY;
            next_process_index = 
                ( next_process_index + 1 ) % SCHEDULER_MAX_PROCESSES 
        );
        
        if ( next_process_index != current_process_index ) {
            current = &process_list[current_process_index];
            next = &process_list[next_process_index];
            current_process_index = next_process_index;
            context_switch( current, next );
        }

    } else {
        
        for ( 
            next_process_index = 0;
            ( process_list[next_process_index].status != PROCESS_STATUS_READY )
                && ( next_process_index < SCHEDULER_MAX_PROCESSES );
            next_process_index = next_process_index + 1
        );
        
        if ( next_process_index < SCHEDULER_MAX_PROCESSES ) {
            next = &process_list[next_process_index];
            current_process_index = next_process_index;
            context_set( next );
        }
        
    
    }
    
}

int scheduler_create_process( void (*f)(void), void *stack ) {
    
    int i;
    int page_table;
    
    do_lock( &scheduler_lock );
    
    for ( i = 0; i < SCHEDULER_MAX_PROCESSES; i++ ) {
        if ( process_list[i].id == 0 ) {
            
            process_list[i].id = i + 1;
            process_list[i].status = PROCESS_STATUS_READY;
            process_list[i].regs[REG_ESP] = (unsigned int)stack;
            process_list[i].regs[REG_CS] = 0x8;
            process_list[i].regs[REG_EIP] = (unsigned int)f;
            process_list[i].regs[REG_CR3] = (unsigned int)&page_dir;
            
            // Start out with the interrupt flag set.
            process_list[i].regs[REG_FLAGS] = 0x200; 
            
            processes_loaded++;
            break;
            
        }
    }

    do_unlock( &scheduler_lock );
    
    return i;
    
}

void scheduler_clear_process( int index ) {
    do_lock( &scheduler_lock );
    process_list[index].id = 0;
    process_list[index].status = PROCESS_STATUS_BLANK;
    do_unlock( &scheduler_lock );
}

void process_exit() {
    do_lock( &scheduler_lock );
    CURRENT_PROCESS->status = PROCESS_STATUS_DEAD;
    do_unlock( &scheduler_lock );
    sleep();
}

void process_sleep() {
    do_lock( &scheduler_lock );
    CURRENT_PROCESS->status = PROCESS_STATUS_SLEEP;
    do_unlock( &scheduler_lock );
    sleep();
}

inline void sleep() {
    __asm__( "hlt" );
}

