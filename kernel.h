#ifndef KERNEL_H
#define KERNEL_H

extern unsigned long long os_uptime;

int test_int;
void c_mode();
void kernel_setup();
void kernel_main();
void kernel_tick();
extern void print_esp();

#endif
