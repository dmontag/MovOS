#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

#define SCHEDULER_MAX_PROCESSES 16
#define CURRENT_PROCESS (&process_list[current_process_index])

extern int current_process_index;
extern int processes_loaded;
extern process_t process_list[SCHEDULER_MAX_PROCESSES];

void scheduler_start( void (*f)(void), void *stack );
void scheduler_tick();
int scheduler_create_process( void (*f)(void), void *stack );
void scheduler_clear_process( int index );

void sleep();

void process_exit();
void process_sleep();

#endif
