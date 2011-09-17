#ifndef LOCK_H
#define LOCK_H

#define LOCK_UNLOCKED 0
#define LOCK_LOCKED 1

typedef int lock_t;

void do_lock( lock_t *lockptr );
void do_unlock( lock_t *lockptr );

#endif
