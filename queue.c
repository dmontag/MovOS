#include "queue.h"

#define QUEUE_WRAP( pos ) (( pos ) % MAX_QUEUE_SIZE )

int queue_is_full( queue_t *queue ) {
    int ret;
    ret = QUEUE_WRAP( queue->head + 1 ) == queue->tail;
    return ret;
}

int queue_is_empty( queue_t *queue ) {
    int ret;
    ret = ( queue->tail == queue->head );
    return ret;
}

void queue_init( queue_t *queue ) {
    queue->head = 0;
    queue->tail = 0;
    queue->operation_lock = LOCK_UNLOCKED;
}

void queue_push( int data, queue_t *queue ) {
    int done = 0;

    do_lock( &queue->operation_lock );

    while ( queue_is_full( queue ) ) {    
        do_unlock( &queue->operation_lock );
        __asm__("hlt");
        do_lock( &queue->operation_lock );
    }
    queue->data[queue->head] = data;
    queue->head = QUEUE_WRAP( queue->head + 1 );
    done = 1;

    do_unlock( &queue->operation_lock );
}

int queue_pull( queue_t *queue ) {
    int ret = 0x12344321;
    int done = 0;
    
    do_lock( &queue->operation_lock );

    while ( queue_is_empty( queue ) ) {
        do_unlock( &queue->operation_lock );
        __asm__("hlt");
        do_lock( &queue->operation_lock );
    }
    ret = queue->data[queue->tail];
    queue->tail = QUEUE_WRAP( queue->tail + 1 );
    done = 1;

    do_unlock( &queue->operation_lock );
    
    return ret;
}
