#ifndef QUEUE_H
#define QUEUE_H

#include "lock.h"

#define MAX_QUEUE_SIZE 16

typedef struct queue_struct {
    lock_t operation_lock;
    int data[MAX_QUEUE_SIZE];
    int head;
    int tail;
} queue_t;

int queue_is_full( queue_t *queue );
int queue_is_empty( queue_t *queue );
void queue_init( queue_t *queue );
void queue_push( int data, queue_t *queue );
int queue_pull( queue_t *queue );

#endif
