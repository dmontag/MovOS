#ifndef SWITCH_H
#define SWITCH_H

#include "process.h"

void context_switch( process_t *current_process, process_t *next_process );
void setup_main_process( process_t *main );

#endif
