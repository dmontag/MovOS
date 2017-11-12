
#include "kernel.h"
#include "scheduler.h"
#include "memory_manager.h"
#include "std.h"
#include "time.h"
#include "queue.h"

lock_t keyboard_lock;
queue_t keyboard_queue;

int test_int = 0x88227733;

void c_mode() {
    kernel_setup();
}

void kernel_setup() {

    print_clear_screen();
    print( "MovOS Kernel v0.6\n", 0x00, 0x00, 0x0F );
    print( "Copyright (c) 2008-2017, http://github.com/dmontag/movos\n\n", 
        0xFF, 0xFF, 0x0F );


    //memory_manager_start();
    queue_init( &keyboard_queue );
    scheduler_start( kernel_main, (void *)0x007FFFF0 );
    
    while ( 1 ) {
        sleep();
    }   
    
}

void time_process() {
    while ( 1 ) {
        print_int( bcd_time(), 23, 0, 14 );
        print_int( bcd_date(), 23, 9, 14 );
        // print_int( time_2k(), 7, 5, 14 );
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

queue_t my_queue;
queue_t producer_ident;

void queue_consumer() {
    print( "Consumer using single queue from producers", 4, 0, 6 );
    print( "Consumer", 5, 0, 7 );
    while ( 1 ) {
        int value = queue_pull( &my_queue );
        int ident = value & 0xF;
        int counter = value >> 4; 
        print_decimal_int( counter, 6 + ident, 0, ident );
    }
}
void queue_producer() {
    int counter = 1;
    int ident = queue_pull( &producer_ident );
    int offset = ident * 10;

    print( "Prod #", 5, offset, 7 );
    print_decimal_int( ident, 5, offset+6, 7 );

    while ( 1 ) {
        print_decimal_int( counter, 6, offset, ident);
        queue_push( (counter << 4) | (ident & 0xF), &my_queue );
        sleep();
        counter += ident;
    }
}

void keyboard_process() {

    int result;

    while ( 1 ) {
        result = queue_pull( &keyboard_queue );
        print_decimal_int( result & 0xFFFF, 24, 0, 8 );
    }
}

void kernel_main() {

    // print( "Main process running\n", 0x02, 0x00, 0x0F );

    queue_init( &my_queue );
    queue_init( &producer_ident );

    scheduler_create_process( queue_consumer, (void *)0x003EDFFF );

    long producer_stack = 0x003ED7FF;
    int num_producers = 6;
    for (int i = 1; i < num_producers+1; i++) {
        queue_push(i, &producer_ident);
        scheduler_create_process( queue_producer, (void *)producer_stack );
        producer_stack -= 800;
    }
    
    scheduler_create_process( time_process, (void *)0x003EAFFF );
    scheduler_create_process( keyboard_process, (void *)0x003EA7FF );
    
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
    
    // //*(char *)(0xB8000) = status;
    // //*(char *)(0xB8002) = scancode;
           
    print_decimal_int( scancode, 23, 40, 7 );
    print_decimal_int( status, 23, 50, 7 );
    
    // if ( keyboard_queue.operation_lock == LOCK_UNLOCKED && 
    //         !queue_is_full( &keyboard_queue ) ) {
    //     queue_push( ( status << 16 ) & scancode, &keyboard_queue );
    // }
    
    
}

void kernel_tick() {
    scheduler_tick();
}

