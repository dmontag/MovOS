#ifndef PROCESS_H
#define PROCESS_H

#include "memory_manager.h"

#define REG_EAX 0
#define REG_EBX 1
#define REG_ECX 2
#define REG_EDX 3
#define REG_ESI 4
#define REG_EDI 5
#define REG_EBP 6
#define REG_ESP 7
#define REG_EIP  8
#define REG_FLAGS 9
#define REG_CS 10
#define REG_CR3 11

enum {
    PROCESS_STATUS_READY,
    PROCESS_STATUS_SLEEP,
    PROCESS_STATUS_DEAD, 
    PROCESS_STATUS_BLANK
};

typedef struct process_struct {
    unsigned int regs[12];
    unsigned int id;
    unsigned int status;
} process_t;

#endif
