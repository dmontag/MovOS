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
    queue->push_lock = LOCK_UNLOCKED;
    queue->pull_lock = LOCK_LOCKED;
    queue->operation_lock = LOCK_UNLOCKED;
}

void queue_push( int data, queue_t *queue ) {
    int done = 0;

    while ( !done ) {    
        do_lock( &queue->operation_lock );
        if ( queue_is_full( queue ) ) {
            do_unlock( &queue->operation_lock );
            do_lock( &queue->push_lock );
            do_lock( &queue->operation_lock );
        }
        if ( !queue_is_full( queue ) ) {
            queue->data[queue->head] = data;
            queue->head = QUEUE_WRAP( queue->head + 1 );
            done = 1;
        }
        do_unlock( &queue->operation_lock );
    }
    
    do_unlock( &queue->pull_lock );
}

int queue_pull( queue_t *queue ) {
    int ret = 0x12344321;
    int done = 0;
    
    while ( !done ) {
        do_lock( &queue->operation_lock );
        if ( queue_is_empty( queue ) ) {
            do_unlock( &queue->operation_lock );
            do_lock( &queue->pull_lock );
            do_lock( &queue->operation_lock );
        } 
        if ( !queue_is_empty( queue ) ) {
            ret = queue->data[queue->tail];
            queue->tail = QUEUE_WRAP( queue->tail + 1 );
            done = 1;
        }
        do_unlock( &queue->operation_lock );
    }
    
    do_unlock( &queue->push_lock );
    
    return ret;
}
