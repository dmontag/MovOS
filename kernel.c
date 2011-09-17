
#include "kernel.h"
#include "scheduler.h"
#include "memory_manager.h"
#include "std.h"
#include "time.h"
#include "queue.h"

lock_t keyboard_lock;
queue_t keyboard_queue;

void time_process() {
    print( "Printing uptime: ", 3, 3, 14 );
    print_int( 0xDEADBEEF, 4, 4, 14 );
    while ( 1 ) {
        print_int( bcd_time(), 5, 5, 14 );
        print_int( bcd_date(), 6, 5, 14 );
        print_int( time_2k(), 7, 5, 14 );
        sleep();
    }
    print( "done ", 6, 6, 14 );
}

void process1() {
    print( "ABCDEFGHI\n", 0xFF, 0xFF, 22 );
    while ( 1 ) {
        sleep();
    }
}

void process2() {
    print( "123456789\n", 0xFF, 0xFF, 23 );
    while ( 1 ) {
        sleep();
    }
}

void print_uptime_at( char *msg, char row ) {
    char uptime[17] = {0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0};
    unsigned long long num;
    while ( 1 ) {
        num = os_uptime;
        uptime[8] = 0x30 + ( ( num & 0xF0000000 ) >> 28 );
        uptime[9] = 0x30 + ( ( num & 0x0F000000 ) >> 24 );
        uptime[10] = 0x30 + ( ( num & 0x00F00000 ) >> 20 );
        uptime[11] = 0x30 + ( ( num & 0x000F0000 ) >> 16 );
        uptime[12] = 0x30 + ( ( num & 0x0000F000 ) >> 12 );
        uptime[13] = 0x30 + ( ( num & 0x00000F00 ) >> 8 );
        uptime[14] = 0x30 + ( ( num & 0x000000F0 ) >> 4 );
        uptime[15] = 0x30 + ( ( num & 0x0000000F ) >> 0 );
        num = num >> 32;
        uptime[0] = 0x30 + ( ( num & 0xF0000000 ) >> 28 );
        uptime[1] = 0x30 + ( ( num & 0x0F000000 ) >> 24 );
        uptime[2] = 0x30 + ( ( num & 0x00F00000 ) >> 20 );
        uptime[3] = 0x30 + ( ( num & 0x000F0000 ) >> 16 );
        uptime[4] = 0x30 + ( ( num & 0x0000F000 ) >> 12 );
        uptime[5] = 0x30 + ( ( num & 0x00000F00 ) >> 8 );
        uptime[6] = 0x30 + ( ( num & 0x000000F0 ) >> 4 );
        uptime[7] = 0x30 + ( ( num & 0x0000000F ) >> 0 );
        uptime[16] = 0;
        print( msg, row, 0x00, 0x0F );
        print( uptime, 0xFF, 0xFF, 0x0F );
        sleep();
    }
}

void process3() { print_uptime_at( "Process 3: ", 0x0A ); }
void process4() { print_uptime_at( "Process 4: ", 0x0B ); }
void process5() { print_uptime_at( "Process 5: ", 0x0C ); }
void process6() { print_uptime_at( "Process 6: ", 0x0D ); }

queue_t my_queue;

void process7() {
    while ( 1 ) {
        print_int( queue_pull( &my_queue ), 0, 0, 14 );
    }
}
void process8() {
    int counter = 1;
    while ( 1 ) {
        queue_push( counter, &my_queue );
        sleep();
        counter += 1;
    }
}

void c_mode() {
    kernel_setup();
}

void kernel_setup() {

    print_clear_screen();
    print( "MovOS Kernel v0.5c\n", 0x00, 0x00, 0x0F );
    print( "Copyright (c) 2008, http://movnet.org/projects/movos\n\n", 
        0xFF, 0xFF, 0x0F );

    memory_manager_start();
    queue_init( &keyboard_queue );
    scheduler_start( kernel_main, (void *)0x007FFFF0 );
    
    while ( 1 ) {
        sleep();
    }   
    
}

void keyboard_process() {

    int result;

    while ( 1 ) {
        result = queue_pull( &keyboard_queue );
        *(char *)(0xB8020) = (char)result;
        print_int( result, 2, 0, 13 );
    }
}

void kernel_main() {

    scheduler_create_process( process1, (void *)0x003FFFFF );
    scheduler_create_process( process2, (void *)0x003F7FFF );
    
    scheduler_create_process( process3, (void *)0x003EFFFF );
    scheduler_create_process( process4, (void *)0x003EF7FF );
    scheduler_create_process( process5, (void *)0x003EEFFF );
    scheduler_create_process( process6, (void *)0x003EE7FF );
    
    queue_init( &my_queue );
    scheduler_create_process( process7, (void *)0x003EDFFF );
    scheduler_create_process( process8, (void *)0x003ED7FF );
    
    scheduler_create_process( time_process, (void *)0x003ECFFF );
    scheduler_create_process( keyboard_process, (void *)0x003EC7FF );
    
    //memory_manager_allocate_process( 10 );
    
    process_exit();
    
}

void kernel_key_pressed() {

    unsigned char status, scancode;
    
    __asm__( 
        "inb $0x64, %%al\n"
        "movb %%al, %0\n"
        "inb $0x60, %%al\n"
        "movb %%al, %1\n"
        : "=r"( status ), "=r"( scancode )  // output
        :                                   // input
        : "%al"                             // clobbered register
    );
    
    *(char *)(0xB8000) = status;
    *(char *)(0xB8002) = scancode;
    
    /*
    if ( keyboard_queue.operation_lock == LOCK_UNLOCKED && 
            !queue_is_full( &keyboard_queue ) ) {
        queue_push( ( status << 16 ) & scancode, &keyboard_queue );
    }
    */
    
}

void kernel_tick() {
    scheduler_tick();
}

