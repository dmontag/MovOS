#ifndef KERNEL_H
#define KERNEL_H

extern unsigned long long os_uptime;

void c_mode();
void kernel_setup();
void kernel_main();
extern void print_esp();

#endif
